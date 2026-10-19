#ifndef __INA226_H
#define __INA226_H

#include "stm32l0xx_hal.h"

/* 用户配置参数（根据实际硬件修改）********************************/
#define INA226_I2C_ADDR         0x40    // I2C 设备地址（7位）
#define INA226_RSHUNT            0.02f   // 分流电阻值，单位：欧姆（20mΩ）
#define INA226_MAX_CURRENT       10.0f   // 最大预期电流，单位：安培

/* 配置寄存器参数（用于调整测量速度和噪声）************************/
// 平均次数选择 (AVG bits) 
#define INA226_AVG_1             0x00    // 1次平均（默认）
#define INA226_AVG_4             0x01
#define INA226_AVG_16            0x02
#define INA226_AVG_64            0x03
#define INA226_AVG_128           0x04
#define INA226_AVG_256           0x05
#define INA226_AVG_512           0x06
#define INA226_AVG_1024          0x07

// 转换时间选择 (VBUSCT 和 VSHCT)
#define INA226_CT_140us          0x00
#define INA226_CT_204us          0x01
#define INA226_CT_332us          0x02
#define INA226_CT_588us          0x03
#define INA226_CT_1p1ms          0x04   // 1.1ms
#define INA226_CT_2p116ms        0x05   // 2.116ms
#define INA226_CT_4p156ms        0x06   // 4.156ms
#define INA226_CT_8p244ms        0x07   // 8.244ms

// 工作模式
#define INA226_MODE_POWER_DOWN   0x00
#define INA226_MODE_SHUNT_TRIG   0x01
#define INA226_MODE_BUS_TRIG     0x02
#define INA226_MODE_SHUNT_BUS_TRIG 0x03
#define INA226_MODE_POWER_DOWN2  0x04
#define INA226_MODE_SHUNT_CONT   0x05
#define INA226_MODE_BUS_CONT     0x06
#define INA226_MODE_SHUNT_BUS_CONT 0x07   // 连续测量 shunt 和 bus

/* 配置组合（可根据需要修改）**************************************/
// 慢速配置：平均16次，转换时间4.156ms → 更新周期约 (4.156+4.156)*16 = 133ms ≈ 7.5Hz
// 再配合软件平均，可降至5Hz左右
#define INA226_AVG_SEL            INA226_AVG_16
#define INA226_VBUSCT_SEL         INA226_CT_4p156ms
#define INA226_VSHCT_SEL          INA226_CT_4p156ms
#define INA226_MODE_SEL           INA226_MODE_SHUNT_BUS_CONT

// 构建配置寄存器值 (参考数据手册)
#define INA226_CONFIG_VALUE ( \
    (0 << 15) | (0 << 14) | (0 << 13) | (0 << 12) | \
    (INA226_AVG_SEL << 9) | \
    (INA226_VBUSCT_SEL << 6) | \
    (INA226_VSHCT_SEL << 3) | \
    INA226_MODE_SEL \
)
/****************************************************************/

/* 初始化 INA226（必须在读取前调用一次）*/
void INA226_Init(void);

/* 基本读取函数（每次返回最新值）*/
float INA226_ReadBusVoltage(void);
float INA226_ReadCurrent(void);
float INA226_ReadPower(void);

/* 带滑动平均的功率读取（推荐用于观察）*/
float INA226_ReadPowerFiltered(void);

/* 可选：清空滑动平均缓冲区 */
void INA226_ClearFilter(void);

#endif
