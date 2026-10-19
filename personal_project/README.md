# 🚀 个人嵌入式完整项目合集
> 说明：全部为独立完整练手项目，**源码可直接编译复现**。

> 📁 **统一目录规范**：
> 每个子项目固定包含：
> - `README.md`：项目快速简介、核心亮点、依赖环境
> - `design.md`：软硬件完整详细设计文档
> - `docs/`：实物截图、调试日志、踩坑复盘资料
> - `hardware/`：原理图、PCB、BOM元器件选型文档
> - `firmware/`：存放项目源码；复合项目firmware内存放多个子源码工程

## 📂项目列表
1. [STM32 无刷电机 FOC 控制系统](./personal_project/01_foc‑bldc‑motor) 👑
> 🏷 `FreeRTOS` `SVPWM` `电压模式FOC` `AS5600磁编码器` `CAN总线`
- ✅ FreeRTOS实时任务框架
- ✅ 电压模式FOC矢量控制、SVPWM调制
- ✅ AS5600磁编码器位置采集
- ✅ CAN总线与上位机交互，电机实时性能调优

2. [STM32双轴运动控制平台（直流+步进）](./personal_project/02_dual-axis-motion-control) 👑
> 🏷 `STM32F103` `IR2104S` `H桥驱动` `霍尔编码器` `增量式PID` `S型加减速` `CAN` `Modbus‑RTU`
- ✅ H桥驱动直流电机、脉冲‑方向模式驱动步进电机，硬件过流保护、死区控制
- ✅ 直流轴增量式PID闭环调速；步进电机梯形/S型加减速抑制启停丢步
- ✅ FreeRTOS划分运动控制、指令解析、状态上报多任务
- ✅ CAN、Modbus‑RTU接收上位机指令，支持双轴独立调速、正反转、多轴联动

3. [工业物联网边缘网关系统（MCU + Linux）](./personal_project/03_industrial-edge-gateway) 👑
> 🏷 `FreeRTOS` `Modbus‑RTU` `CAN` `LLCC68‑LoRa` `W5500` `MQTT` `cJSON` `RK3568(Linux)` `Socket` `SQLite`
> ℹ 内含 `mcu_gateway`、`linux_edge_gateway` 两个子工程
- ✅ MCU端：RS485/LoRa/CAN多协议采集，完成协议转换转发MQTT
- ✅ Linux端：RK3568部署MQTT Broker、SQLite本地缓存
- ✅ TCP Socket完成MCU‑Linux通信，支持断线重连、本地数据缓存，实现完整端云链路

4. [四轴无人机飞控系统](./personal_project/04_quadcopter-flight-controller) 👑
> 🏷 `STM32F4` `FreeRTOS` `MPU6050` `互补滤波` `串级PID` `PWM`
- ✅ FreeRTOS拆分姿态解算、PID闭环、电机驱动、遥控接收独立任务
- ✅ 互补滤波融合六轴传感器，输出稳定姿态数据
- ✅ 串级PID输出PWM驱动无刷电机，实现飞行器稳定悬停
- ✅ 任务优先级调优，解决高速飞行机体剧烈震荡故障

5. [储能电池管理与安全充电系统（BMS+充电桩）](./personal_project/05_bms-and-charger) 🔴
> 🏷 `BQ76920` `I2C(CRC‑8)` `CAN` `IAP Bootloader` `NXP‑RC522‑RFID` `ECB02‑BLE` `OLED`
> ℹ 内含 `bms_system`、`smart_charger`两大子产品；smart_charger包含`charger_bootloader(IAP引导)`、`charger_app(充电主程序)`两套固件
- ✅ BMS：BQ76920采集5串锂电电压、电流、NTC温度；过压/欠压/过流保护，OCV查表+温度补偿SOC估算
- ✅ 充电桩：自研IAP固件升级（CRC校验+断电回滚防变砖）；RFID刷卡扣费；强电继电器安全控制逻辑

6. [STM32冷热双循环温控系统](./personal_project/06_stm32‑tec‑temperature‑ctrl) 🔴
> 🏷 `STM32L031` `TEC半导体制冷` `NTC` `复合数字滤波` `BLE` `OLED` `编码器`
- ✅ TEC半导体制冷片冷热双向驱动
- ✅ 多路NTC测温，复合数字滤波抑制采样噪声，实现±0.25℃稳态控温精度
- ✅ 本地OLED+编码器人机、BLE安卓APP、VOFA+上位机多端观测控制
- ✅ 非阻塞FSM管理制冷/加热/恒温工况，面向工业长期无人值守场景

7. [ESP32‑S3智能视觉触控终端系统（多形态版）](./personal_project/07_esp32‑smart‑terminals) 🟡
> 🏷 `ESP‑IDF` `FreeRTOS` `LVGL` `FT6336G触摸` `ILI9341V` `I2S` `ES8311` `OV2640/OV3660` `SDIO` `WebSocket` `MQTT` `BLE` `NVS` `OTA`
> ℹ 内含 `touch_voice_terminal`、`vision_voice_terminal`、`video_doorbell`三个子工程
- ✅ FT6336G电容触摸+ILI9341V SPI彩屏，LVGL图形触控交互
- ✅ I2S+ES8311完成音频采集播放；环形缓冲区隔离音视频，避免网络阻塞丢包
- ✅ SDIO TF卡存储、ADC电池电量检测；BLE配网、NVS参数持久化、OTA在线升级完整业务

8. [嵌入式基础与物联网综合实践](./personal_project/08_iot‑basic‑projects) 🟢
> 🏷 `杰理Byle‑U8 SDK` `A2DP` `离线KWS` `AD按键` `红外` `WS2812` `阿里云MQTT` `Android` `AD8232` `ADC/DMA` `EEPROM`
> ℹ 内含 `offline_voice_speaker`、`smart_greenhouse`、`ecg_monitor`三个子工程
- ✅ 离线语音蓝牙音箱：A2DP蓝牙音频、离线KWS识别，按键/红外多输入控制彩灯
- ✅ ESP32智能温室大棚：多传感器采集，MQTT上云，Android APP远程管控，断线重连，EEPROM参数保存
- ✅ 心电采集监测仪：ADC‑DMA采样AD8232心电信号；三层软件架构；串口上报波形至上位机

## 📌返回导航
[⬅ 返回作品集首页](../README.md)
