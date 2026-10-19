// ad7124.c
#include "ad7124.h"

// CS引脚定义
#define AD7124_CS_PORT GPIOA
#define AD7124_CS_PIN GPIO_PIN_4

// 函数：拉低CS
static void AD7124_CS_Low(void) {
    HAL_GPIO_WritePin(AD7124_CS_PORT, AD7124_CS_PIN, GPIO_PIN_RESET);
}

// 函数：拉高CS
static void AD7124_CS_High(void) {
    HAL_GPIO_WritePin(AD7124_CS_PORT, AD7124_CS_PIN, GPIO_PIN_SET);
}

// 初始化函数：复位AD7124并等待
void AD7124_Init(void) {
    uint8_t reset_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    AD7124_CS_Low();
    HAL_SPI_Transmit(&hspi1, reset_data, 8, HAL_MAX_DELAY);
    AD7124_CS_High();
    HAL_Delay(10); // 等待复位完成
}

// 写函数：向AD7124写入寄存器
void AD7124_Write(uint8_t addr, uint8_t* data, uint8_t len) {
    uint8_t comm = (0 << 7) | (0 << 6) | (addr & 0x3F);  // WEN=0, R/W=0, RS=addr
    AD7124_CS_Low();
    HAL_SPI_Transmit(&hspi1, &comm, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
    AD7124_CS_High();
}

// 读函数：从AD7124读取寄存器
void AD7124_Read(uint8_t addr, uint8_t* data, uint8_t len) {
    uint8_t comm = (0 << 7) | (1 << 6) | (addr & 0x3F);  // WEN=0, R/W=1, RS=addr
    AD7124_CS_Low();
    HAL_SPI_Transmit(&hspi1, &comm, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, data, len, HAL_MAX_DELAY);
    AD7124_CS_High();
}

// 读取ID函数
uint8_t AD7124_ReadID(void) {
    uint8_t id = 0;
    AD7124_Read(0x05, &id, 1);  // ID寄存器地址0x05
    return id;
}
