// ad7124.h
#ifndef AD7124_H
#define AD7124_H

#include "stm32l0xx_hal.h"
#include <stdint.h>

extern SPI_HandleTypeDef hspi1; // 假设已在main中定义

void AD7124_Init(void);
void AD7124_Write(uint8_t addr, uint8_t* data, uint8_t len);
void AD7124_Read(uint8_t addr, uint8_t* data, uint8_t len);
uint8_t AD7124_ReadID(void);

#endif

