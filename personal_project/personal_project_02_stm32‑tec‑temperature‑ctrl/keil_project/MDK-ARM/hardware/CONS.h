#ifndef CONS_H
#define CONS_H
#include "main.h"
/* ====================== PWM 参数 ====================== */
#define PWM_PERIOD 55555
/* ====================== PID 参数 ====================== */
/* 制冷参数 */
#define COOL_KP_COARSE 18.5f
#define COOL_KI_COARSE 1.8f
#define COOL_KD_COARSE 12.5f
#define COOL_KP_FINE 6.0f
#define COOL_KI_FINE 7.2f
#define COOL_KD_FINE 6.5f

/* 精调切换阈值 */
#define COARSE_THRESH 2.2f
#define FINE_EXIT_OVERSHOOT 4.0f
/* 制热参数 */
#define HEAT_KP_COARSE 3.8f
#define HEAT_KI_COARSE 2.2f
#define HEAT_KD_COARSE 8.0f
#define FF_GAIN 0.55f 
#define FF_HS_GAIN 0.115f


#define HEAT_KP_FINE 1.2f     // 原1.0 → 1.2（P更 aggressive）
#define HEAT_KI_FINE 0.65f    // 原0.5 → 0.65（I稍快，但别超过0.7）
#define HEAT_KD_FINE 7.0f     // 原6.2 → 7.0（D加强阻尼）


#define HEAT_STABLE_ERR_BAND 0.75f
/* 积分死区阈值 */
#define HEAT_INT_DEADZONE_POS 0.02f // 正误差死区（原0.15，缩小以消除静差）
#define HEAT_INT_DEADZONE_NEG 0.02f // 负误差死区（对称）
/* 积分衰减系数 */
#define HEAT_INT_NEG_DECAY_FINE 0.33f
#define HEAT_INT_NEG_DECAY_COARSE 1.3f
/* fine 模式下积分限幅 */
#define HEAT_FINE_INT_MAX 7.5f // fine 模式下积分上限
#define HEAT_FINE_INT_MIN -7.5f // fine 模式下积分下限
/* 公共参数 */
#define HEAT_SINK_FACTOR 0.01f
#define HEAT_SINK_COMP_LIMIT 0.4f
#define AMBIENT_FACTOR 0.02f
#define MODE_HYSTERESIS 1.20f
#define BALANCE_DELTA_HS 0.3f
#define BALANCE_ERROR 0.25f
#define FINE_MODE_LOCK_TIME 26000
#define HS_BIAS 2.0f
#define OPPOSITE_DEADBAND 1.8f
#define AGGRESSIVE_COOL_DELTA 10.0f
#define COOL_MILD_KP 6.0f
#define COOL_MILD_KI 0.8f
#define COOL_MILD_KD 2.0f
#define COOL_FF_HS_GAIN 0.3f
#define MILD_COOL_HS_DELTA 6.0f
#define COOL_MIN_DUTY_NORMAL 18.0f
#define COOL_MIN_MILD_DUTY 8.0f
/* 温和加热模式参数 */
#define MILD_HEAT_HS_DELTA 12.0f
#define HEAT_MILD_KP 0.85f
#define HEAT_MILD_KI 0.50f
#define HEAT_MILD_KD 6.0f
#define HEAT_MILD_FF_GAIN 0.075f
#define HEAT_MILD_OVERSHOOT_CUT 1.2f
#define HEAT_MILD_MAX_DUTY 40.0f
typedef enum {
    MODE_IDLE,
    MODE_HEAT,
    MODE_COOL
} ControlMode_t;
void CONS_Init(void);
void CONS_TempControl(float target_temp, float current_temp, float ambient_temp, float heat_sink_temp);
ControlMode_t CONS_GetCurrentMode(void);
float CONS_GetFilteredTemp(void);
#endif
