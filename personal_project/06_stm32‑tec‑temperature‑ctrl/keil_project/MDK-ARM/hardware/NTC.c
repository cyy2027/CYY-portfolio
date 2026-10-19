#include "ntc.h"
#include "adc.h"
#include <math.h>
#include <string.h>   // 用于 memcpy

/* 硬件参数同上 */
#define VREF              3.3f
#define ADC_RESOLUTION    4095.0f
#define R_FIXED           10000.0f
#define T0                298.15f
#define R0                10000.0f






static uint32_t adc_buffer[4][FILTER_LENGTH];   // 滑动平均环形缓冲
static uint8_t  adc_index[4] = {0};
static float    ema_temp[4] = {0.0f};           // 每个通道的 EMA 滤波温度
static uint8_t  initialized = 0;

static void InitFilter(uint32_t val1, uint32_t val2, uint32_t val3, uint32_t val4)
{
    for (int i = 0; i < FILTER_LENGTH; i++) {
        adc_buffer[0][i] = val1;
        adc_buffer[1][i] = val2;
        adc_buffer[2][i] = val3;
        adc_buffer[3][i] = val4;
    }
    for (int ch = 0; ch < 4; ch++) {
        ema_temp[ch] = 25.0f;   // 初始合理值
    }
    initialized = 1;
}

/* 简单中值滤波（去掉极端值） */
static uint32_t MedianFilter(uint32_t *buf, uint8_t size)
{
    uint32_t temp[MEDIAN_BUF_SIZE];
    memcpy(temp, buf, size * sizeof(uint32_t));
    
    // 简单冒泡排序取中位数
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-i-1; j++) {
            if (temp[j] > temp[j+1]) {
                uint32_t t = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = t;
            }
        }
    }
    return temp[size/2];
}

/* 更新滑动平均 + 中值 + 返回滤波后的ADC值 */
static uint32_t UpdateAndAverage(uint8_t channel, uint32_t new_val)
{
    adc_buffer[channel][adc_index[channel]] = new_val;
    adc_index[channel] = (adc_index[channel] + 1) % FILTER_LENGTH;

    uint32_t median_buf[MEDIAN_BUF_SIZE];
    uint8_t cnt = 0;
    for (int i = 0; i < FILTER_LENGTH && cnt < MEDIAN_BUF_SIZE; i++) {
        median_buf[cnt++] = adc_buffer[channel][i];
    }
    uint32_t median = MedianFilter(median_buf, cnt);

    uint64_t sum = 0;
    for (int i = 0; i < FILTER_LENGTH; i++) {
        sum += adc_buffer[channel][i];
    }
    uint32_t avg = (uint32_t)(sum / FILTER_LENGTH);

    return (avg + median) / 2;   // 平均值与中值再平均，更稳
}

void NTC_Init(void)
{
    // 可选：HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
}

void NTC_ReadTemperatures(float *temp1, float *temp2, float *temp3, float *temp4, uint16_t B_value)
{
    uint32_t adc_raw[4] = {0};
    uint32_t adc_filtered[4];
    float voltages[4];
    float resistances[4];
    float temperatures[4];

    HAL_ADC_Stop(&hadc);
    if (HAL_ADC_Start(&hadc) != HAL_OK) {
        *temp1 = *temp2 = *temp3 = *temp4 = 0.0f;
        return;
    }

    uint32_t timeout = 100;
    for (int i = 0; i < 4; i++) {
        if (HAL_ADC_PollForConversion(&hadc, timeout) == HAL_OK) {
            adc_raw[i] = HAL_ADC_GetValue(&hadc);
        } else {
            HAL_ADC_Stop(&hadc);
            *temp1 = *temp2 = *temp3 = *temp4 = 0.0f;
            return;
        }
    }
    HAL_ADC_Stop(&hadc);

    if (!initialized) {
        InitFilter(adc_raw[0], adc_raw[1], adc_raw[2], adc_raw[3]);
    }

    // 增强滤波：滑动平均 + 中值
    for (int i = 0; i < 4; i++) {
        adc_filtered[i] = UpdateAndAverage(i, adc_raw[i]);
    }

    // 计算温度（使用强滤波后的ADC值）
    for (int i = 0; i < 4; i++) {
        voltages[i] = (adc_filtered[i] / ADC_RESOLUTION) * VREF;

        if (voltages[i] <= 0.001f || voltages[i] >= VREF - 0.001f) {
            temperatures[i] = -273.15f;
        } else {
            resistances[i] = R_FIXED * voltages[i] / (VREF - voltages[i]);
            float ln_r = logf(resistances[i] / R0);
            float inv_t = 1.0f / T0 + (1.0f / B_value) * ln_r;
            float temp_k = 1.0f / inv_t;
            temperatures[i] = temp_k - 273.15f;
        }

        // 最终温度 EMA 平滑（强烈推荐保留）
        ema_temp[i] = EMA_ALPHA * temperatures[i] + (1.0f - EMA_ALPHA) * ema_temp[i];
    }

    *temp1 = ema_temp[0];
    *temp2 = ema_temp[1];
    *temp3 = ema_temp[2];
    *temp4 = ema_temp[3];
}

