#include "MENU.h"
#include "tim.h"
#include "gpio.h"
#include "OLED.h"
#include <stdlib.h>

/* 外部变量 */
extern float target_temperature;
extern volatile uint8_t system_stopped;

/* 内部菜单状态 */
static enum {
    MENU_NORMAL,
    MENU_ADJUST
} menu_mode = MENU_NORMAL;

static float   adjust_temp      = 0.0f;
static int32_t pulse_accum      = 0;
static uint32_t last_blink_time = 0;
static uint8_t  blink_state     = 0;

/* 编码器按键状态机（PA8） */
static enum {
    BUTTON_IDLE,
    BUTTON_PRESS_DETECTED,
    BUTTON_PRESS_CONFIRMED,
    BUTTON_RELEASE_DETECTED
} button_state = BUTTON_IDLE;

static uint32_t press_time       = 0;
static uint8_t  button_processing = 0;

/* STOP按键状态机（PA15） */
static enum {
    STOP_IDLE,
    STOP_PRESS_DETECTED,
    STOP_PRESS_CONFIRMED,
    STOP_RELEASE_WAIT
} stop_button_state = STOP_IDLE;

static uint32_t stop_press_time = 0;

/* 蜂鸣器 */
volatile uint8_t buzzer_active    = 0;
static uint32_t buzzer_start_time = 0;

/* ------------------------------------------------------------------ */
void MENU_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}

/* ------------------------------------------------------------------ */
void MENU_Process(void)
{
    uint32_t now = HAL_GetTick();

    /* ====== PA15 STOP 按键状态机 ====== */
    uint8_t stop_read = HAL_GPIO_ReadPin(STOP_BUTTON_PORT, STOP_BUTTON_PIN);

    switch (stop_button_state) {
        case STOP_IDLE:
            if (stop_read == GPIO_PIN_RESET) {
                stop_button_state = STOP_PRESS_DETECTED;
                stop_press_time   = now;
            }
            break;

        case STOP_PRESS_DETECTED:
            if (stop_read == GPIO_PIN_RESET) {
                if (now - stop_press_time >= 20) {          /* 消抖 20ms */
                    stop_button_state = STOP_PRESS_CONFIRMED;

                    /* --- 执行 STOP 逻辑 --- */
                    system_stopped = 1;

                    /* 如果正在调节模式，退出并放弃修改 */
                    if (menu_mode == MENU_ADJUST) {
                        menu_mode = MENU_ADJUST;            /* 保持原值，不写入 */
                        menu_mode = MENU_NORMAL;
                        pulse_accum = 0;
                        __HAL_TIM_SET_COUNTER(&htim2, 0);
                    }

                    /* 短鸣提示 */
                    buzzer_active     = 1;
                    buzzer_start_time = now;
                }
            } else {
                /* 抖动，回到IDLE */
                stop_button_state = STOP_IDLE;
            }
            break;

        case STOP_PRESS_CONFIRMED:
            /* 等待松开 */
            if (stop_read == GPIO_PIN_SET) {
                stop_button_state = STOP_RELEASE_WAIT;
                stop_press_time   = now;
            }
            break;

        case STOP_RELEASE_WAIT:
            if (stop_read == GPIO_PIN_SET) {
                if (now - stop_press_time >= 20) {          /* 松开消抖 */
                    stop_button_state = STOP_IDLE;
                }
            } else {
                /* 还没松开，继续等 */
                stop_button_state = STOP_PRESS_CONFIRMED;
            }
            break;
    }

    /* ====== PA8 编码器按键状态机 ====== */
    uint8_t button_read = HAL_GPIO_ReadPin(SET_BUTTON_PORT, SET_BUTTON_PIN);

    switch (button_state) {
        case BUTTON_IDLE:
            if (button_read == GPIO_PIN_RESET) {
                button_state = BUTTON_PRESS_DETECTED;
                press_time   = now;
            }
            break;

        case BUTTON_PRESS_DETECTED:
            if (button_read == GPIO_PIN_RESET) {
                if (now - press_time >= 20) {               /* 消抖 20ms */
                    button_state      = BUTTON_PRESS_CONFIRMED;
                    button_processing = 1;

                    if (menu_mode == MENU_ADJUST) {
                        /* 确认写入目标温度 */
                        target_temperature = adjust_temp;
                        menu_mode          = MENU_NORMAL;

                        /* 按键确认目标后，如果之前是STOP状态则恢复运行 */
                        if (system_stopped) {
                            system_stopped = 0;
                        }
                    }

                    pulse_accum = 0;
                    __HAL_TIM_SET_COUNTER(&htim2, 0);
                    buzzer_active     = 1;
                    buzzer_start_time = now;
                }
            } else {
                button_state = BUTTON_IDLE;                 /* 抖动，回IDLE */
            }
            break;

        case BUTTON_PRESS_CONFIRMED:
            if (button_read == GPIO_PIN_SET) {
                button_state = BUTTON_RELEASE_DETECTED;
                press_time   = now;
            }
            break;

        case BUTTON_RELEASE_DETECTED:
            if (button_read == GPIO_PIN_SET) {
                if (now - press_time >= 20) {
                    button_state      = BUTTON_IDLE;
                    button_processing = 0;
                    pulse_accum       = 0;
                    __HAL_TIM_SET_COUNTER(&htim2, 0);
                }
            } else {
                button_state = BUTTON_PRESS_CONFIRMED;
            }
            break;
    }

    /* ====== 编码器旋转处理 ====== */
    if (!button_processing) {
        int16_t diff = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
        if (diff != 0) {
            __HAL_TIM_SET_COUNTER(&htim2, 0);
            pulse_accum += diff;

            /* 普通模式下：积累足够脉冲才进入调节模式 */
            if (menu_mode == MENU_NORMAL) {
                int32_t abs_accum = (pulse_accum < 0) ? -pulse_accum : pulse_accum;
                if (abs_accum >= ENCODER_ENTER_THRESHOLD) {
                    menu_mode       = MENU_ADJUST;
                    adjust_temp     = target_temperature;   /* 以当前目标为起点 */
                    last_blink_time = now;
                    blink_state     = 0;
                    pulse_accum     = 0;
                }
            }

            /* 调节模式下：旋转改变预览温度 */
            if (menu_mode == MENU_ADJUST) {
                while (pulse_accum >= ENCODER_STEPS_PER_DEGREE) {
                    adjust_temp += 1.0f;
                    if (adjust_temp > 52.0f) adjust_temp = 52.0f;
                    pulse_accum -= ENCODER_STEPS_PER_DEGREE;
                }
                while (pulse_accum <= -ENCODER_STEPS_PER_DEGREE) {
                    adjust_temp -= 1.0f;
                    if (adjust_temp < 0.0f) adjust_temp = 0.0f;
                    pulse_accum += ENCODER_STEPS_PER_DEGREE;
                }
            }
        }
    }

    /* ====== 闪烁控制 ====== */
    if (menu_mode == MENU_ADJUST) {
        if (now - last_blink_time >= 100) {
            last_blink_time = now;
            blink_state     = !blink_state;
        }
    }

    /* ====== 蜂鸣器自动关闭 ====== */
    if (buzzer_active && (now - buzzer_start_time >= 50)) {
        buzzer_active = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
    }
}

/* ------------------------------------------------------------------ */
uint8_t MENU_IsAdjustMode(void) { return (menu_mode == MENU_ADJUST); }
float   MENU_GetAdjustTemp(void) { return adjust_temp; }
uint8_t MENU_GetBlinkState(void) { return blink_state; }

