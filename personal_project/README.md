# 🚀 个人嵌入式完整项目合集
> 说明：以系统架构设计、Debug 排查实录、专题技术文档为主；**源码与原理图按各项目实际开放程度提供**，未纳入仓库的会在项目 README 首屏标注，不做含糊承诺。

> 📁 **目录规范（推荐结构，按项目实际提供）**：
> - `README.md`：项目快速简介、核心亮点、依赖环境
> - `design.md`：软硬件完整详细设计文档
> - `docs/`：专题文档、实物截图、调试日志、踩坑复盘资料
> - `hardware/`：原理图、PCB、BOM元器件选型文档
> - `firmware/`：存放项目源码；复合项目firmware内存放多个子源码工程

## 📂项目列表
0. [具身智能边缘计算平台（预研方案）](./00_embodied_ai_platform) 🚧
> ℹ 状态：**仅方案设计** —— 0 行代码、无硬件、无实测数据，所有性能数字均为目标值
> 🏷 `RK3588(Linux+6TOPS NPU)` `双足/四足平台` `大脑—小脑异构` `RL 端侧推理` `Sim-to-Real`
- 🔎 待解问题：Linux 侧 50 Hz 策略推理与 MCU 侧 20 kHz 电流环相差三个数量级，如何构造延迟可界定的通路
- ✅ 已完成：分层架构、四个核心风险拆解、四阶段验收指标、选型权衡与否决理由
- ⬜ 未开始：硬件采购、BSP 适配、RKNN 工具链实操、控制代码

1. [赛博猫狗：四足双平台（MCU 单机 + ROS 2 分层）](./01_quadruped_robotics) 👑
> 🏷 `Cortex‑M` `FreeRTOS` `SPI` `UART` `IMU` `CAN` `Trot步态` `五次多项式轨迹` `姿态互补滤波` `分级故障安全`
> 🏷 `ROS 2 Foxy` `XRCE‑DDS` `micro‑ROS` `UART+DMA` `环形缓冲`
> ℹ 同题两解，实时边界划在不同位置：`hyperdog_rtos_control`（板上闭环）与 `nybble_micro_ros`（上位机规划）
- 🐕 **HyperDog 单机主控** —— [./01_quadruped_robotics/hyperdog_rtos_control](./01_quadruped_robotics/hyperdog_rtos_control)
  - ✅ 对角Trot小跑步态调度，五次多项式足端轨迹，落地冲击小
  - ✅ ICM20602 IMU姿态解算，动态滤波抑制电机振动噪声
  - ✅ CAN分布式驱动关节伺服，1kHz周期下发角度指令
  - ✅ 分级故障保护、平滑收腿兜底，Tick驱动非阻塞状态机
- 🐈 **Nybble ROS 2 分层** —— [./01_quadruped_robotics/nybble_micro_ros](./01_quadruped_robotics/nybble_micro_ros) 🚧 仅方案设计
  - ✅ 规划层搬至 Linux：IK / Trot 轨迹 / 姿态补偿跑在 ROS 2 节点，12 路角度经 XRCE-DDS 下发
  - ✅ DMA 批量接收 + 环形缓冲 + FreeRTOS 队列三段式消抖；TIM1/TIM8 原生 PWM 取代 PCA9685
  - ⬜ 实机、固件与上位机节点均未开始，文档内指标为设计目标值

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