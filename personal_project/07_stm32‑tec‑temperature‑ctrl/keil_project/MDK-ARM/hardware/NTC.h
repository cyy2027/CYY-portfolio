#ifndef NTC_H
#define NTC_H

#include "main.h"

/* 硬件参数 */
#define VREF              3.3f
#define ADC_RESOLUTION    4095.0f
#define R_FIXED           10000.0f
#define T0                298.15f
#define R0                10000.0f

/* ==================== 增强滤波参数 ==================== */
/* 滑动平均窗口（原来8，现在加大到32，强烈推荐） */
#define FILTER_LENGTH     32

/* 每个通道额外指数移动平均系数（EMA） */
#define EMA_ALPHA         0.08f   // 0.08~0.18 可调，越小越平滑（推荐先用0.12）

/* 中值滤波辅助缓冲大小（去极值） */
#define MEDIAN_BUF_SIZE   5

/* 函数声明 */
void NTC_Init(void);
void NTC_ReadTemperatures(float *temp1, float *temp2, float *temp3, float *temp4, uint16_t B_value);

#endif

