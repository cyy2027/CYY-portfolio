# 🚀 个人嵌入式完整项目合集
> 说明：全部为独立完整练手项目，**以系统架构设计、Debug排查实录、专题技术文档为主，附核心代码与原理图**。

> 📁 **统一目录规范**：
> 每个子项目固定包含：
> - `README.md`：项目快速简介、核心亮点、依赖环境
> - `design.md`：软硬件完整详细设计文档
> - `docs/`：实物截图、调试日志、踩坑复盘资料
> - `hardware/`：原理图、PCB、BOM元器件选型文档
> - `firmware/`：存放项目源码；复合项目firmware内存放多个子源码工程

## 📂项目列表
1. [HyperDog 四足机器人主控固件](./01_hyperdog-quadruped) 👑
> 🏷 `Cortex‑M` `FreeRTOS` `SPI` `UART` `IMU` `CAN` `Trot步态` `五次多项式轨迹` `姿态互补滤波` `分级故障安全`
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ 对角Trot小跑步态调度，五次多项式足端轨迹，落地冲击小
- ✅ ICM20602 IMU姿态解算，动态滤波抑制电机振动噪声
- ✅ CAN分布式驱动关节伺服，1kHz周期下发角度指令
- ✅ 分级故障保护、平滑收腿兜底，Tick驱动非阻塞状态机

2. [北斗/GNSS 定位飞控系统 | 实时控制与空间定位](./02_gnss-flight-controller) 🔥
> 🏷 `STM32H723/F405` `FreeRTOS` `C/C++` `Qt5` `IMU/GNSS多传感器融合` `Dshot-600` `ELRS(CRSF)` `MSP OSD` `ESP8285 Wi-Fi`
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ 基于ICM-42688-P、气压计、M10北斗/GNSS多源传感器融合，搭建多级串级PID，实现姿态稳定、定高、位置定点控制
- ✅ 实现Dshot-600高速数字电调驱动；ELRS CRSF远距离遥控遥测链路；移植IRC Tramp图传控制、MSP OSD画面数据叠加
- ✅ 双芯片架构，ESP8285负责Wi‑Fi无线透传调参；Qt5开发桌面/安卓上位机，传感器可视化+参数一键下发
- ✅ 打通传感器融合→串级PID→Dshot电调→无线遥测全链路，完成硬件驱动到上位机交互整套系统开发

3. [STM32 无刷电机 FOC 控制系统](./03_foc‑bldc‑motor) 🔥
> 🏷 `FreeRTOS` `SVPWM` `电压模式FOC` `AS5600磁编码器` `CAN总线`
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ FreeRTOS实时任务框架
- ✅ 电压模式FOC矢量控制、SVPWM调制
- ✅ AS5600磁编码器位置采集
- ✅ CAN总线与上位机交互，电机实时性能调优

4. [STM32双轴运动控制平台（直流+步进）](./04_dual‑axis‑motion‑control) 🔥
> 🏷 `STM32F103` `IR2104S` `H桥驱动` `霍尔编码器` `增量式PID` `S型加减速` `CAN` `Modbus‑RTU`
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ H桥驱动直流电机、脉冲‑方向模式驱动步进电机，硬件过流保护、死区控制
- ✅ 直流轴增量式PID闭环调速；步进电机梯形/S型加减速抑制启停丢步
- ✅ FreeRTOS划分运动控制、指令解析、状态上报多任务
- ✅ CAN、Modbus‑RTU接收上位机指令，支持双轴独立调速、正反转、多轴联动

5. [工业物联网边缘网关系统（MCU + Linux）](./05_industrial‑edge‑gateway/) 🔥
> 🏷 `FreeRTOS` `Modbus‑RTU` `CAN` `LLCC68‑LoRa` `W5500` `MQTT` `cJSON` `RK3568(Linux)` `Socket` `SQLite`
> ℹ 内含 `mcu_gateway`、`linux_edge_gateway` 两个子工程
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ MCU端：RS485/LoRa/CAN多协议采集，完成协议转换转发MQTT
- ✅ Linux端：RK3568部署MQTT Broker、SQLite本地缓存
- ✅ TCP Socket完成MCU‑Linux通信，支持断线重连、本地数据缓存，实现完整端云链路

6. [储能电池管理与安全充电系统（BMS+充电桩）](./06_bms‑and‑charger/) 🔴
> 🏷 `BQ76920` `I2C(CRC‑8)` `CAN` `IAP Bootloader` `NXP‑RC522‑RFID` `ECB02‑BLE` `OLED`
> ℹ 内含 `bms_system`、`smart_charger`两大子产品；smart_charger包含`charger_bootloader(IAP引导)`、`charger_app(充电主程序)`两套固件
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ BMS：BQ76920采集5串锂电电压、电流、NTC温度；过压/欠压/过流保护，OCV查表+温度补偿SOC估算
- ✅ 充电桩：自研IAP固件升级（CRC校验+断电回滚防变砖）；RFID刷卡扣费；强电继电器安全控制逻辑

7. [STM32冷热双循环温控系统](./07_stm32‑tec‑temperature‑ctrl) 🔴
> 🏷 `STM32L031` `TEC半导体制冷` `NTC` `复合数字滤波` `BLE` `OLED` `编码器`
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ TEC半导体制冷片冷热双向驱动
- ✅ 多路NTC测温，复合数字滤波抑制采样噪声，实现±0.25℃稳态控温精度
- ✅ 本地OLED+编码器人机、BLE安卓APP、VOFA+上位机多端观测控制
- ✅ 非阻塞FSM管理制冷/加热/恒温工况，面向工业长期无人值守场景

8. [ESP32‑S3智能视觉触控终端系统（多形态版）](./08_esp32‑smart‑terminals) 🟡
> 🏷 `ESP‑IDF` `FreeRTOS` `LVGL` `FT6336G触摸` `ILI9341V` `I2S` `ES8311` `OV2640/OV3660` `SDIO` `WebSocket` `MQTT` `BLE` `NVS` `OTA`
> ℹ 内含 `touch_voice_terminal`、`vision_voice_terminal`、`video_doorbell`三个子工程
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ FT6336G电容触摸+ILI9341V SPI彩屏，LVGL图形触控交互
- ✅ I2S+ES8311完成音频采集播放；环形缓冲区隔离音视频，避免网络阻塞丢包
- ✅ SDIO TF卡存储、ADC电池电量检测；BLE配网、NVS参数持久化、OTA在线升级完整业务

9. [物联网与消费电子综合实践](./09_iot‑basic‑projects) 🟢
> 🏷 `杰理Byle‑U8 SDK` `A2DP` `离线KWS` `AD按键` `红外` `WS2812` `阿里云MQTT` `Android` `AD8232` `ADC/DMA` `EEPROM`
> ℹ 内含 `offline_voice_speaker`、`smart_greenhouse`、`ecg_monitor`三个子工程
> 文档类型：系统架构设计 + 专题技术文档 + Debug实录，附核心代码、原理图
- ✅ 离线语音蓝牙音箱：A2DP蓝牙音频、离线KWS识别，按键/红外多输入控制彩灯
- ✅ ESP32智能温室大棚：多传感器采集，MQTT上云，Android APP远程管控，断线重连，EEPROM参数保存
- ✅ 心电采集监测仪：ADC‑DMA采样AD8232心电信号；三层软件架构；串口上报波形至上位机

## 📌返回导航
[⬅ 返回作品集首页](../README.md)