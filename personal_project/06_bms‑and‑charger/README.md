# 储能BMS与智能充电桩
## 项目简介
本项目分为两大子原型：**BMS电池管理系统**与**智能充电桩系统**。
BMS基于BQ76920模拟前端AFE芯片实现5串锂电池组的电压、电流、NTC温度采集，实现过压、欠压、过流硬件保护；采用OCV查表+温度补偿完成SOC估算。
智能充电桩实现IAP‑Bootloader固件升级，具备CRC校验、断电回滚防变砖机制；集成RFID刷卡扣费、继电器强电控制、蓝牙参数配置、OLED状态显示。
整套原型面向储能锂电池充放电安全管控，验证电池保护、固件安全升级、计费交互整套业务逻辑。

## 系统架构
1. **BMS电池管理系统**
- STM32F103主控，I2C带CRC‑8校验与BQ76920 AFE芯片通信；采集单体电压、总电流、多路NTC温度。
- FreeRTOS多任务：数据采集任务、SOC估算任务、CAN通信任务、OLED显示任务。
- 硬件+软件双重保护：过压、欠压、过流、过温故障检测，触发切断充放电回路。
- CAN总线上报电池状态，支持远程充放电控制。

2. **智能充电桩 smart_charger**
- charger_bootloader：IAP引导程序，Flash分区管理；固件CRC完整性校验，升级异常断电自动回滚旧版本，防止变砖。
- charger_app：充电业务主程序；RC522‑RFID刷卡扣费；ECB02蓝牙透传；OLED显示电压电流、扣费状态；继电器控制强电输出。

## 技术栈
STM32F103、FreeRTOS、BQ76920、I2C(CRC‑8)、CAN总线、IAP Bootloader、CRC校验、NXP‑RC522 RFID、ECB02 BLE、OLED

## 核心功能
### BMS子系统
1. BQ76920 AFE芯片采集5串锂电单体电压、充放电电流、多路NTC温度。
2. OCV开路电压查表SOC估算，增加温度补偿，提升不同温度下SOC准确度。
3. 过压、欠压、过流、过温故障检测，软硬件联动保护，切断充放电回路。
4. CAN总线上报电池电压、电流、SOC、温度、故障状态，接收远程控制指令。
5. OLED本地可视化电池各项参数。

### 充电桩子系统
1. IAP固件升级：Flash分区，APP固件CRC校验；升级过程断电自动回滚旧固件，避免设备变砖。
2. RC522 RFID寻卡、防冲突、刷卡扣费业务逻辑。
3. ECB02蓝牙：手机蓝牙修改单价、查询充电记录。
4. 继电器强电安全控制，充电状态OLED屏幕显示。

## 关键难点与解决方案
1. **锂电池SOC估算受温度影响误差大**
单纯OCV查表只适合常温；增加NTC温度对OCV曲线做补偿修正，降低高低温下SOC估算偏差。

2. **I2C通信干扰，BQ76920读取数据出错**
I2C增加CRC‑8校验，校验失败丢弃本次采样，增加重试机制，避免错误数据参与SOC计算。

3. **IAP升级断电，设备变砖风险**
Flash划分Bootloader区、备份固件区、APP运行区；新固件写入备份区，CRC校验完全通过之后才搬运到APP运行区；升级中途断电，下次上电检测校验失败，直接运行旧可用固件实现回滚。

4. **强电继电器控制安全风险**
增加故障状态判断；异常状态下强制关闭继电器输出；上电默认断开强电输出，防止上电误输出。

5. **多任务访问电池参数结构体产生数据竞争**
采集任务更新电池数据，显示、CAN任务读取数据；共享结构体访问增加互斥锁保护。

## 项目成果
- BMS实现5串锂电池完整采集、保护、SOC估算，CAN远程状态上报，具备故障安全保护能力。
- 充电桩实现IAP断电回滚升级机制，RFID扣费、蓝牙参数配置、强电继电器控制完整业务原型。

## 🎬 演示与实物

| 内容 | 文件 |
|---|---|
| 实物照片 | [hardware_demo.png](./smart_charger/charger_app/doc/images/hardware_demo.png) |
| 系统架构框图 | [system_arch.png](./smart_charger/charger_app/doc/images/system_arch.png) |
| 原理图 / PCB | [schematic.png](./smart_charger/charger_app/doc/images/schematic.png) · [pcb.png](./smart_charger/charger_app/doc/images/pcb.png) |
| 软件流程图 | [流程 1](./smart_charger/charger_app/doc/images/software_flow1.png) · [流程 2](./smart_charger/charger_app/doc/images/software_flow2.png) · [流程 3](./smart_charger/charger_app/doc/images/software_flow3.png) |
| 主控 / 模块引脚 | [mcu_pin.png](./smart_charger/charger_app/doc/images/mcu_pin.png) · [rc522_pin.png](./smart_charger/charger_app/doc/images/rc522_pin.png) · [charge_module_pin.png](./smart_charger/charger_app/doc/images/charge_module_pin.png) |
| 硬件工程（Altium） | [stm32‑smart‑charger.PrjPCB](./smart_charger/charger_app/doc/hardware/) |
| 设计说明书 | [STM32 RC522 RFID 自助充电桩控制器设计书](./smart_charger/charger_app/doc/) |
| 器件手册 | [SWM320 数据手册](./smart_charger/charger_bootloader/doc/SWM320_ds.pdf) · [Air780 AT 手册](./smart_charger/charger_bootloader/doc/Air780%20AT.pdf) |

## 🧱 源码与第三方依赖

本仓库**只保留自研代码、原理图与文档**，不重复上传厂商 SDK 与第三方库。编译前请按下表补齐依赖：

| 依赖 | 用途 | 获取方式 |
|---|---|---|
| Synwit SWM320 SDK（StdPeriph 驱动 + FreeRTOS 移植） | `charger_bootloader` 编译；仓库内保留 `SWM320_StdPeriph_Driver` 与两个在用工程 | 华芯微特官网 |
| STM32CubeF1 HAL / CMSIS | `charger_app` 编译 | 用 STM32CubeMX 打开 `smart_charger/charger_app/keil_project/Project_C8.ioc` 重新生成 `Drivers/` |
| FlashDB | 参数掉电存储 | <https://github.com/armink/FlashDB> |

> 已移除的编译产物（`out/`、`Objects/`、`Listings/`、`*.uvguix.*`、`JLinkLog.txt`）由本地编译重新生成，`jlink` 与 Keil 版本相关文件不应入库。

## 📌返回导航

[⬅ 返回作品集首页](../../README.md)