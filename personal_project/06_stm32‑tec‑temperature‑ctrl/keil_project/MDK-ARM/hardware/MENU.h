#ifndef __MENU_H
#define __MENU_H

#include "stm32l0xx_hal.h"

/* 编码器每度对应的脉冲数 */
#define ENCODER_STEPS_PER_DEGREE  4

/* 进入调节模式所需的编码器脉冲阈值 */
#define ENCODER_ENTER_THRESHOLD   8

/* 设定按键引脚（编码器按键，PA8） */
#define SET_BUTTON_PORT           GPIOA
#define SET_BUTTON_PIN            GPIO_PIN_8

/* STOP 按键引脚（PA15，默认高电平，按下低电平） */
#define STOP_BUTTON_PORT          GPIOA
#define STOP_BUTTON_PIN           GPIO_PIN_15

/* 蜂鸣器频率分频值 */
#define BUZZER_FREQ_DIV   10

/* 初始化编码器及定时器 */
void MENU_Init(void);

/* 在主循环中周期性调用，处理编码器旋转、按键和STOP按键 */
void MENU_Process(void);

/* 获取当前调节状态和闪烁标志 */
uint8_t MENU_IsAdjustMode(void);
float   MENU_GetAdjustTemp(void);
uint8_t MENU_GetBlinkState(void);

#endif

