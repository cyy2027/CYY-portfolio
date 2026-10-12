#include "INA226.h"

extern I2C_HandleTypeDef hi2c1;   // 假设使用 hi2c1

/* 寄存器地址 */
#define REG_CONFIG          0x00
#define REG_SHUNT_VOLTAGE   0x01
#define REG_BUS_VOLTAGE     0x02
#define REG_POWER           0x03
#define REG_CURRENT         0x04
#define REG_CALIBRATION     0x05

/* 内部静态变量 */
static float currentLSB = 0.0f;
static float powerLSB   = 0.0f;

/* 滑动平均缓冲区（用于功率）*/
#define FILTER_SIZE  8   // 缓冲区大小，可调整
static float powerBuffer[FILTER_SIZE] = {0};
static uint8_t filterIndex = 0;
static uint8_t filterFilled = 0;   // 标记是否已填满

/* 私有函数：读取16位寄存器 */
static uint16_t readRegister(uint8_t reg)
{
    uint8_t data[2];
    if (HAL_I2C_Mem_Read(&hi2c1, INA226_I2C_ADDR << 1, reg,
                         I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY) == HAL_OK) {
        return ((uint16_t)data[0] << 8) | data[1];
    }
    return 0;
}

/* 私有函数：写入16位寄存器 */
static void writeRegister(uint8_t reg, uint16_t value)
{
    uint8_t data[2];
    data[0] = (value >> 8) & 0xFF;
    data[1] = value & 0xFF;
    HAL_I2C_Mem_Write(&hi2c1, INA226_I2C_ADDR << 1, reg,
                      I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
}

/* 初始化 */
void INA226_Init(void)
{
    uint16_t calValue;
    float calFloat;

    /* 1. 写入配置寄存器（根据用户定义）*/
    writeRegister(REG_CONFIG, INA226_CONFIG_VALUE);

    /* 2. 计算 Current_LSB */
    currentLSB = INA226_MAX_CURRENT / 32768.0f;

    /* 3. 计算校准寄存器值 */
    calFloat = 0.00512f / (currentLSB * INA226_RSHUNT);
    if (calFloat > 65535.0f) {
        calValue = 0xFFFF;
    } else if (calFloat < 0.0f) {
        calValue = 0;
    } else {
        calValue = (uint16_t)(calFloat + 0.5f);
    }
    writeRegister(REG_CALIBRATION, calValue);

    /* 4. 计算功率 LSB */
    powerLSB = 25.0f * currentLSB;

    /* 5. 清空滤波器缓冲区 */
    INA226_ClearFilter();
}

/* 清空滑动平均缓冲区 */
void INA226_ClearFilter(void)
{
    for (int i = 0; i < FILTER_SIZE; i++) {
        powerBuffer[i] = 0.0f;
    }
    filterIndex = 0;
    filterFilled = 0;
}

/* 读取总线电压 */
float INA226_ReadBusVoltage(void)
{
    uint16_t raw = readRegister(REG_BUS_VOLTAGE);
    return (float)raw * 0.00125f;
}

/* 读取电流 */
float INA226_ReadCurrent(void)
{
    if (currentLSB == 0.0f) return 0.0f;
    uint16_t raw = readRegister(REG_CURRENT);
    return (float)raw * currentLSB;
}

/* 读取功率（原始值）*/
float INA226_ReadPower(void)
{
    if (powerLSB == 0.0f) return 0.0f;
    uint16_t raw = readRegister(REG_POWER);
    return (float)raw * powerLSB;
}

/* 读取功率（带滑动平均）*/
float INA226_ReadPowerFiltered(void)
{
    float newPower = INA226_ReadPower();

    /* 存入缓冲区 */
    powerBuffer[filterIndex] = newPower;
    filterIndex++;
    if (filterIndex >= FILTER_SIZE) {
        filterIndex = 0;
        filterFilled = 1;
    }

    /* 计算平均值 */
    float sum = 0.0f;
    uint8_t count = filterFilled ? FILTER_SIZE : filterIndex;
    if (count == 0) return newPower;   // 未初始化时返回新值

    for (int i = 0; i < count; i++) {
        sum += powerBuffer[i];
    }
    return sum / count;
}
