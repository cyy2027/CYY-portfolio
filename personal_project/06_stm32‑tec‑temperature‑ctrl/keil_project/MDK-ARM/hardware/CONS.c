#include "cons.h"
#include "tim.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"
#define DT 0.02f
extern volatile uint8_t system_stopped;
float current_heat_duty = 0.0f;
float current_cool_duty = 0.0f;
static float heat_integral = 0.0f;
static float cool_integral = 0.0f;
static ControlMode_t current_mode = MODE_IDLE;
static float filtered_temp = 0.0f;
static float prev_ctrl_temp = 0.0f;
static uint8_t fine_mode_active = 0;
static uint32_t fine_mode_enter_time = 0;
/* ------------------------------------------------------------------ */
static void SetHeatDuty(float duty)
{
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 100.0f) duty = 100.0f;
    uint32_t pulse = (uint32_t)((duty / 100.0f) * (PWM_PERIOD + 1.0f) + 0.5f);
    __HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_1, pulse);
    current_heat_duty = duty;
}
static void SetCoolDuty(float duty)
{
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 100.0f) duty = 100.0f;
    uint32_t pulse = (uint32_t)((duty / 100.0f) * (PWM_PERIOD + 1.0f) + 0.5f);
    __HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_2, pulse);
    current_cool_duty = duty;
}
/* ------------------------------------------------------------------ */
void CONS_Init(void)
{
    HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_2);
    SetHeatDuty(0.0f);
    SetCoolDuty(0.0f);
    heat_integral = cool_integral = 0.0f;
    current_mode = MODE_IDLE;
    filtered_temp = 0.0f;
    prev_ctrl_temp = 0.0f;
    fine_mode_active = 0;
    fine_mode_enter_time = 0;
}
float CONS_GetFilteredTemp(void) { return filtered_temp; }
ControlMode_t CONS_GetCurrentMode(void) { return current_mode; }
void CONS_TempControl(float target_temp, float current_temp,
                      float ambient_temp, float heat_sink_temp)
{
    if (system_stopped) {
        SetHeatDuty(0.0f);
        SetCoolDuty(0.0f);
        current_mode = MODE_IDLE;
        return;
    }
		
		
		
		
		
		

	/*
		
		//诊断输出，每2秒一次 
    static uint32_t last_diag = 0;
    uint32_t diag_now = HAL_GetTick();
    if (diag_now - last_diag >= 3000) {
        last_diag = diag_now;
        char diag[200];
        snprintf(diag, sizeof(diag),
            "DIAG: tgt=%.1f ct=%.3f hs=%.3f amb=%.3f "
            "raw_err=%.3f filt=%.3f "
            "fine=%d hi=%.3f ci=%.3f mode=%d "
            "hd=%.1f cd=%.1f\r\n",
            target_temp, current_temp, heat_sink_temp, ambient_temp,
            target_temp - current_temp, filtered_temp,
            fine_mode_active, heat_integral, cool_integral,
            (int)current_mode, current_heat_duty, current_cool_duty);
        HAL_UART_Transmit(&huart2, (uint8_t*)diag, strlen(diag), 100);
    }
		
	*/
		
		
		
		
		
		
		
    /* --- 目标温度限幅 --- */
    if (target_temp > 55.0f) target_temp = 55.0f;
    if (target_temp < 0.0f) target_temp = 0.0f;
    /* --- 一阶低通滤波 --- */
    if (filtered_temp == 0.0f) filtered_temp = current_temp;
    else filtered_temp = 0.12f * current_temp + 0.88f * filtered_temp;
    float ctrl_temp = filtered_temp;
    if (prev_ctrl_temp == 0.0f) prev_ctrl_temp = ctrl_temp;
    float d_pv = (ctrl_temp - prev_ctrl_temp) / DT;
    prev_ctrl_temp = ctrl_temp;
    float raw_error = target_temp - ctrl_temp;
    float abs_error = fabsf(raw_error);
    uint32_t now = HAL_GetTick();
    /* --- 自然平衡区 --- */
    if (fabsf(heat_sink_temp - target_temp) < BALANCE_DELTA_HS &&
        fabsf(raw_error) < BALANCE_ERROR)
    {
        SetHeatDuty(0.0f);
        SetCoolDuty(0.0f);
        heat_integral = 0.0f;
        cool_integral = 0.0f;
        current_mode = MODE_IDLE;
        return;
    }
    /* --- 强力制冷标志 --- */
    uint8_t aggressive_cool = (target_temp < heat_sink_temp - AGGRESSIVE_COOL_DELTA);
    /* --- 精细模式进入/退出 --- */
    if (!fine_mode_active && abs_error <= COARSE_THRESH && !aggressive_cool) {
        fine_mode_active = 1;
        fine_mode_enter_time = now;
        heat_integral *= 0.65f;
        cool_integral *= 0.65f;
    } else if (fine_mode_active && (now - fine_mode_enter_time) >= FINE_MODE_LOCK_TIME) {
        if (abs_error > FINE_EXIT_OVERSHOOT || aggressive_cool) {
            fine_mode_active = 0;
        }
    }
    /* --- 前馈补偿误差修正 --- */
    float heat_sink_comp = (heat_sink_temp - target_temp) * HEAT_SINK_FACTOR;
    float ambient_comp = (heat_sink_temp - ambient_temp) * AMBIENT_FACTOR;
    float comp = heat_sink_comp + ambient_comp;
    comp = fmaxf(-HEAT_SINK_COMP_LIMIT, fminf(HEAT_SINK_COMP_LIMIT, comp));
    float compensated_error = raw_error - comp;
    /* --- 模式切换逻辑 --- */
    uint8_t want_heat = 0, want_cool = 0;
    if (current_mode == MODE_HEAT) {
        if (raw_error < -MODE_HYSTERESIS) want_cool = 1;
        else want_heat = 1;
    } else if (current_mode == MODE_COOL) {
        if (raw_error > MODE_HYSTERESIS) want_heat = 1;
        else want_cool = 1;
    } else {
        if (raw_error > 0.3f) want_heat = 1;
        else if (raw_error < -0.3f) want_cool = 1;
    }
    if (heat_sink_temp < target_temp - HS_BIAS) {
        if (raw_error < -OPPOSITE_DEADBAND) want_cool = 1;
        else want_heat = 1;
    } else if (heat_sink_temp > target_temp + HS_BIAS) {
        if (raw_error > OPPOSITE_DEADBAND) want_heat = 1;
        else want_cool = 1;
    }
    if (want_heat) goto do_heat;
    else if (want_cool) goto do_cool;
    else {
        if (fabsf(raw_error) < 0.30f) {
            SetHeatDuty(0.0f);
            SetCoolDuty(0.0f);
            current_mode = MODE_HEAT;
            return;
        }
        if (raw_error > 0.0f) goto do_heat;
        else goto do_cool;
    }
/* ========================= 制热分支 ========================= */
do_heat:
{
	
	
	
	


    if (current_mode != MODE_HEAT) {
        cool_integral *= 0.05f;   // 更强衰减
        cool_integral = 0.0f;     // ← 直接清零（关键！）
    }

	
	
	
	
	
	
    float hs_delta = target_temp - heat_sink_temp;
    uint8_t mild_heat = 0;
    if (hs_delta > 0 && hs_delta < MILD_HEAT_HS_DELTA && fabsf(raw_error) < 3.0f) {
        mild_heat = 1;
    }
    float overshoot_cut_th = mild_heat ? HEAT_MILD_OVERSHOOT_CUT : 1.2f;
    if (raw_error < -overshoot_cut_th) {
        heat_integral *= 0.05f;
        cool_integral = 0.0f;
        goto do_cool;
    }
    float kp, ki, kd, ff_gain_use = FF_GAIN, ff_hs_gain_use = FF_HS_GAIN;
    if (mild_heat) {
        kp = HEAT_MILD_KP;
        ki = HEAT_MILD_KI;
        kd = HEAT_MILD_KD;
        ff_hs_gain_use = HEAT_MILD_FF_GAIN;
    } else {
        kp = fine_mode_active ? HEAT_KP_FINE : HEAT_KP_COARSE;
        ki = fine_mode_active ? HEAT_KI_FINE : HEAT_KI_COARSE;
        kd = fine_mode_active ? HEAT_KD_FINE : HEAT_KD_COARSE;
    }
    if (current_mode != MODE_HEAT) {
        cool_integral *= 0.10f;
    }
    /* 前馈计算 */
    float temp_over_ambient = fmaxf(target_temp - ambient_temp, 0.0f);
    float hs_delta_ff = fmaxf(target_temp - heat_sink_temp, 0.0f);
    float feedforward = ff_gain_use * temp_over_ambient + ff_hs_gain_use * hs_delta_ff;
    if (feedforward > 48.0f) feedforward = 48.0f;
    float err = compensated_error;
    float ff_headroom = 72.0f - feedforward;
    if (ff_headroom < 15.0f) ff_headroom = 15.0f;
    float max_integral = ff_headroom * 0.65f / ki;
/* --- 积分更新：fine模式小误差完全冻结 --- */
uint8_t freeze_heat_int = 0;
if (fine_mode_active && !mild_heat && fabsf(err) < 0.01f) {
    freeze_heat_int = 1;
}





if (!freeze_heat_int) {
    if (err > HEAT_INT_DEADZONE_POS) {
        float ig;
        if (!fine_mode_active && err > 4.0f) ig = 0.65f;
        else if (err < 0.8f) ig = 0.55f;
        else ig = 0.85f;
        heat_integral += err * DT * ig;
    }
    else if (err < -HEAT_INT_DEADZONE_NEG) {
        float decay = fine_mode_active ? HEAT_INT_NEG_DECAY_FINE : HEAT_INT_NEG_DECAY_COARSE;
        heat_integral += err * DT * decay;
    }
    else {
        heat_integral *= 0.9998f;
    }

    /* 极慢衰减：只在deadzone内且mild_heat时执行 */
    if (mild_heat && fabsf(err) < 0.08f) {
        heat_integral *= 0.92f;
    }
}




    /* 积分限幅：fine 模式使用更紧的限幅防止震荡 */
    if (fine_mode_active && !mild_heat) {
        if (heat_integral > HEAT_FINE_INT_MAX) heat_integral = HEAT_FINE_INT_MAX;
        if (heat_integral < HEAT_FINE_INT_MIN) heat_integral = HEAT_FINE_INT_MIN;
    } else {
        if (heat_integral > max_integral) heat_integral = max_integral;
        if (heat_integral < -18.0f) heat_integral = -18.0f;
    }
    /* mild_heat 模式下进一步限幅 */
    if (mild_heat) {
        if (heat_integral > 4.0f) heat_integral = 4.0f;
        if (heat_integral < -3.5f) heat_integral = -3.5f;
    }
    float pid_out = kp * err + ki * heat_integral - kd * d_pv * 0.85f;
    float duty = feedforward + pid_out;
    if (duty > 72.0f) duty = 72.0f;
    if (duty < 0.0f) duty = 0.0f;
    if (mild_heat && duty > HEAT_MILD_MAX_DUTY) duty = HEAT_MILD_MAX_DUTY;
		
		

		
		
/* 输出平滑 */
static float last_duty = 0.0f;
if (fabsf(duty - last_duty) > 5.0f && fabsf(err) < 2.0f) {
    duty = last_duty + (duty - last_duty) * 0.45f;
}
last_duty = duty;

/* === 关键修改在这里 === */
if (mild_heat) {
    SetHeatDuty(duty);
} else {
    if (duty > 0.1f) SetHeatDuty(duty);
    else if (duty > 0.0f) SetHeatDuty(0.1f);
    else SetHeatDuty(0.0f);
}

/* 新增：必须清零cool duty，否则残留！ */
SetCoolDuty(0.0f);

current_mode = MODE_HEAT;
return;
}
/* ========================= 制冷分支 ========================= */
do_cool:
{
	
	
	

	
	
	
	
	
    if (current_mode != MODE_COOL) {
        heat_integral *= 0.05f;
        cool_integral = 0.0f;
    }
		
		
		
		
		
		
		
		
    if (raw_error < -4.0f) {
        SetCoolDuty(100.0f);
        SetHeatDuty(0.0f);
        cool_integral = 0.0f;
        current_mode = MODE_COOL;
        return;
    }
    float hs_diff = heat_sink_temp - target_temp;
    uint8_t mild_mode = 0;
    if (hs_diff < MILD_COOL_HS_DELTA && fabsf(raw_error) < 2.0f) {
        mild_mode = 1;
    }
    float kp, ki, kd;
    if (mild_mode) {
        kp = COOL_MILD_KP;
        ki = COOL_MILD_KI;
        kd = COOL_MILD_KD;
    } else {
        kp = fine_mode_active ? COOL_KP_FINE : COOL_KP_COARSE;
        ki = fine_mode_active ? COOL_KI_FINE : COOL_KI_COARSE;
        kd = fine_mode_active ? COOL_KD_FINE : COOL_KD_COARSE;
    }
    float cool_ff = 0.0f;
    if (hs_diff > 0.0f) {
        cool_ff = COOL_FF_HS_GAIN * hs_diff;
        if (cool_ff > 30.0f) cool_ff = 30.0f;
    }
    float p_term = kp * (-raw_error);
    float d_term = kd * d_pv;
    uint8_t freeze_integral = 0;
    if (mild_mode && fabsf(raw_error) < 0.2f) {
        freeze_integral = 1;
    }
    if (!freeze_integral) {
        float ki_factor = mild_mode ? 1.0f : 2.5f;
        if (fabsf(raw_error) < 5.5f) {
            cool_integral += (-ki * raw_error) * DT * ki_factor;
        } else {
            cool_integral *= 0.91f;
        }
        if (mild_mode) {
            if (cool_integral > 15.0f) cool_integral = 15.0f;
            if (cool_integral < -5.0f) cool_integral = -5.0f;
        } else {
            if (cool_integral > 82.0f) cool_integral = 82.0f;
            if (cool_integral < -18.0f) cool_integral = -18.0f;
        }
    }
    float duty = p_term + cool_integral + d_term + cool_ff;
    if (duty > 100.0f) duty = 100.0f;
    if (duty < 0.0f) duty = 0.0f;
    if (mild_mode && (heat_sink_temp > target_temp + 0.2f) && (raw_error < -0.1f)) {
        if (duty < COOL_MIN_MILD_DUTY) {
            duty = COOL_MIN_MILD_DUTY;
        }
    }
    if (!mild_mode) {
        if (duty > 0.0f && duty < COOL_MIN_DUTY_NORMAL) duty = COOL_MIN_DUTY_NORMAL;
    }
    static float last_cool_duty = 0.0f;
    float max_change = 95.0f * DT;
    if (duty - last_cool_duty > max_change) duty = last_cool_duty + max_change;
    if (last_cool_duty - duty > max_change) duty = last_cool_duty - max_change;
    last_cool_duty = duty;
    SetCoolDuty(duty);
    SetHeatDuty(0.0f);
    current_mode = MODE_COOL;
    return;
}
}
