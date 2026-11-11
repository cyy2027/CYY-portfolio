# 陈艺苑 · 嵌入式软件工程师作品集

**2027 届 ｜ 求职方向：嵌入式软件 / 固件开发（MCU 固件 · RTOS · 嵌入式 Linux）**

> 🌐 [English version](./README_EN.md)

> 2 段实习（含 1 个量产交付项目）· 9 套完整实战系统 · 1 项在研预研 · 26 篇专题技术文档
> 覆盖：底层固件与 BSP 驱动 · 工业控制与边缘计算 · 机器人 / 飞控 · 音视频终端 · 医疗仪器

[在线版作品集（飞书，排版更精美）](https://my.feishu.cn/docx/D5kzdRqFVoco87x0ESDcsfz8njd?from=from_copylink)

> 💡 **阅读方式**：项目统一按 `README.md`（简介与亮点）、`design.md`（软硬件详细设计）、`docs/`（专题文档 / 调试记录 / 复盘）组织。建议 clone 到本地用 VS Code + Markdown 预览阅读。

---

## 🎯 五套作品集（按岗位方向选一个看）

> 面试时间有限时，按目标岗位挑一套看。每套是一页**策展页**：只做筛选、链接与证据标注，项目内容仍在各目录原文，不重复维护。

| 作品集 | 覆盖岗位方向 | 核心证据 | 入口 |
|---|---|---|---|
| **A · 动态控制** | 飞控、具身/机器人、AGV、运动控制、FOC、变频家电 | 1 kHz CAN 关节调度、四级串级 PID、SVPWM（源码在库） | [→ A](./showcase/A_motion-control.md) |
| **B · 音频** | 音频算法（反馈抑制）、音视频终端、安防 IPC | FBX 陷波 + 移频防啸叫、I2S 链路、UAC2.0 声卡 | [→ B](./showcase/B_audio.md) |
| **C · 结构** | 结构设计 | 温控腔体自研（3D 设计 + 打印验证）、机器猫结构优化、ANOTECH 实习 | [→ C](./showcase/C_mechanical.md) |
| **D · 硬件** | 电机驱动·功率、PCB Layout、消费电子、AE/FAE | TEC 整板原理图 + PCB 源文件、量产强电板 | [→ D](./showcase/D_hardware.md) |
| **E · 数据报告** | BMS、充电桩、计量、可靠性 | ±0.25 ℃ 实测曲线、OCV+温度补偿 SOC、8 类 Bug 复盘 | [→ E](./showcase/E_data-report.md) |

---

## 🧭 按岗位方向查找（60+ 方向完整索引）

> **[《作品集方向索引（完整版）》](./docs/career-path-index.md)** —— 按 **60+ 个岗位方向**组织的主线系统索引：
> 嵌入式固件（通用 / Linux / IoT / 电机控制 / 工业 / 医疗 / 具身智能 / 家电 / 储能 / 车载…）·
> 硬件研发 · 测试 / 可靠性 / 制造 · 结构 / 文档 / FAE 支持类，另含外企**英文简历**方向的选型。
> 每个方向列出建议展示的主线系统与工程，简历投递时可按表引导面试官查阅。

---

## 🧰 能力矩阵（技能 → 可核验的证据）

| 能力项 | 对应项目与文档 |
|---|---|
| **陌生平台 Bring-up / 国产芯片迁移** | 实习① 国产 Cortex-M0 替代（2 周量产）· 实习② BP2668Ax RISC-V 音频 DSP Bring-up · [《量产固件 OTA 与国产 MCU 迁移》](./docs/firmware‑mass‑produce.md) |
| **量产可靠性设计** | 06 自研 IAP + CRC 校验 + 断电回滚 · 实习① IWDG 看门狗与强电安全逻辑 · 01 分级故障安全 · [《Cortex-M HardFault 定位》](./docs/cortex-m-hardfault-debug.md) |
| **裸机 / RTOS 固件架构** | 01 / 03 / 04 / 07 非阻塞 FSM + FreeRTOS 多任务划分 · [《非阻塞 FSM 设计》](./docs/realtime‑fsm.md) · [《FreeRTOS 踩坑集》](./docs/freertos-trap-notes.md) |
| **控制算法落地** | 02 多级串级 PID · 03 SVPWM 电压模式 FOC · 01 Trot 步态与五次多项式轨迹 · 07 ±0.25 ℃ 稳态温控 · [《PID 工程实战》](./docs/pid-engineering-tuning.md) |
| **总线与协议栈** | CAN（01/04/06）· Modbus-RTU（04/05）· COBS+CRC 自定义帧（实习②）· LoRa / MQTT（05）· [《CAN 总线实践》](./docs/can-bus-practice.md) |
| **嵌入式 Linux / 边缘计算** | 05 RK3568 部署 MQTT Broker、TCP Socket 跨芯片通信、SQLite 本地缓存 · [《边缘网关 Socket 与本地缓存》](./docs/linux-edge-socket-cache.md) |
| **音视频与 GUI** | 实习②③ I2S 音频数据流、混音路由、消爆音 · 08 LVGL + OV2640 + WebSocket · [《音频 pop 噪声定位》](./docs/audio-pop-noise-debug.md) |
| **上位机与调试工具链** | 02 Qt5 桌面/安卓上位机 · 09 Python 波形上位机 · J-Link / 逻辑分析仪 / 串口排错 |

---

## 🏢 实习项目（已脱敏，无企业涉密源码）

**高新兴科技集团｜音频部**　*嵌入式软件助理工程师*

1. 🔴 **[电源时序器国产化替代](./internship/GAOXINXING/internship_project_01-power-sequencer)** —— 国产 Cortex-M0 芯片迁移替代项目。非阻塞 FSM 实现八路电源时序控制，ADC 矩阵按键、HLW8032 电能计量、IWDG 看门狗；**2 周独立完成固件适配并交付量产**，定位并闭环 8 类软硬件耦合 Bug。
2. 🔴 **[数字调音台-反馈抑制器](./internship/GAOXINXING/internship_project_02-digital-audio-mixer)** —— 基于 BP2668Ax 评估板完成 RISC-V 音频 DSP Bring-up：I2S 音频数据流、COBS+CRC 自定义串口协议、移频反馈抑制算法预研；解决爆音、底噪、IO 引脚冲突等软硬件耦合问题。
3. 🟡 **[桌面 USB 声卡](./internship/GAOXINXING/internship_project_03-usb-audio-interface)** —— 方案设计阶段介入，硬件未到位前提下先完成上层业务框架：音频混音路由、消爆音处理、全局状态管理、分层软件架构设计。

> 每个目录内置项目方案、流程图、开发总结；[实习证明与部门概览](./internship/GAOXINXING/README.md)。
> **[安诺特实习](./internship/ANOTECH)** 为早期**产品结构设计**实习总结（非固件代码），建立了三维空间直觉、硬件制造视野与跨部门协同经验。
> ⚠️ 严格遵守保密协议，实习代码与资料均已脱敏，不含企业涉密业务源码与内部物料参数。

---

## 个人完整实战系统（9 套）

### 01 · [赛博猫狗：四足双平台（MCU 单机 + ROS 2 分层）](./personal_project/01_quadruped_robotics)　👑 代表作

同一个母题做两遍 —— 用来说明**实时边界到底该划在哪一层**。

**[HyperDog · MCU 单机主控](./personal_project/01_quadruped_robotics/hyperdog_rtos_control)**　`Cortex-M` `FreeRTOS` `IMU(ICM20602)` `CAN` `Trot 步态` `五次多项式`
- 对角 Trot 步态调度 + 五次多项式足端轨迹，起止点速度、加速度均为 0，落地冲击小。
- CAN 一主多从：主控 **1 kHz** 下发 12 自由度关节目标角，200 Hz 步态主循环，关节侧各自下沉 FOC。
- 自适应互补滤波（冲击检测动态调权）+ 四级故障保护与平滑收腿；架构 / 协议 / 融合 / 步态 / 安全 / 标定六个部分见本目录 README。

**[Nybble · ROS 2 + micro-ROS 分层](./personal_project/01_quadruped_robotics/nybble_micro_ros)**　`ROS 2 Foxy` `XRCE-DDS` `UART+DMA` `FreeRTOS`　🚧 在研
- 规划层搬到 Linux（IK / 步态 / 姿态补偿），MCU 只做实时执行，12 路角度以 Topic 周期下发。
- DMA + 环形缓冲 + 队列三段式消抖；TIM1/TIM8 原生 PWM 取代 PCA9685 扩展板。
- 当前**仅架构设计**，无实机与代码 —— 与左边那套同题不同解，对照看取舍更清楚。

### 02 · [北斗 / GNSS 定位飞控系统](./personal_project/02_gnss-flight-controller)　🔴
`STM32H723/F405` `FreeRTOS` `IMU/GNSS 融合` `串级 PID` `Dshot-600` `ELRS(CRSF)` `MSP OSD` `ESP8285` `Qt5`
- ICM-42688-P + 气压计 + M10 北斗/GNSS 多源融合，多级串级 PID 实现姿态稳定、定高、位置定点。
- Dshot-600 高速数字电调驱动；ELRS CRSF 远距离遥控遥测；移植 IRC Tramp 图传控制与 MSP OSD 画面叠加。
- 双芯片架构：ESP8285 负责 Wi-Fi 透传调参；Qt5 上位机（桌面/安卓）传感器可视化 + 参数一键下发。

### 03 · [STM32 无刷电机 FOC 控制系统](./personal_project/03_foc‑bldc‑motor)　🔴
`FreeRTOS` `SVPWM` `电压模式 FOC` `AS5600 磁编码器` `CAN`
- 无电流采样条件下的电压模式 FOC 矢量控制实现，SVPWM 调制。
- AS5600 磁编码器位置采集；CAN 与上位机交互完成电机实时性能调优。

### 04 · [STM32 双轴运动控制平台（直流 + 步进）](./personal_project/04_dual‑axis‑motion‑control)　🟡
`STM32F103` `IR2104S H 桥` `霍尔编码器` `增量式 PID` `S 型加减速` `CAN` `Modbus-RTU`
- H 桥驱动直流电机、脉冲-方向模式驱动步进电机，硬件过流保护与死区控制。
- 直流轴增量式 PID 闭环调速；步进梯形/S 型加减速抑制启停丢步。
- CAN、Modbus-RTU 下发指令，支持双轴独立调速、正反转与多轴联动。

### 05 · [工业物联网边缘网关系统（MCU + Linux）](./personal_project/05_industrial‑edge‑gateway)　🔴
`FreeRTOS` `Modbus-RTU` `CAN` `LLCC68-LoRa` `W5500` `MQTT` `cJSON` `RK3568(Linux)` `Socket` `SQLite`
- MCU 端：RS485 / LoRa / CAN 多协议采集，协议转换后转发 MQTT。
- Linux 端：RK3568 部署 MQTT Broker 与 SQLite 本地缓存。
- TCP Socket 打通 MCU-Linux 通信，断线重连 + 本地缓存，形成完整端边云链路。

### 06 · [储能电池管理与安全充电系统（BMS + 充电桩）](./personal_project/06_bms‑and‑charger)　🔴
`BQ76920` `I2C(CRC-8)` `CAN` `IAP Bootloader` `RC522 RFID` `ECB02 BLE` `OLED`
- BMS：BQ76920 采集 5 串锂电电压/电流/NTC 温度，过压欠压过流保护，OCV 查表 + 温度补偿估算 SOC。
- 充电桩：自研 IAP 固件升级（CRC 校验 + 断电回滚防变砖）；RFID 刷卡扣费；强电继电器安全控制逻辑。

### 07 · [STM32 冷热双循环高精度温控系统](./personal_project/07_stm32‑tec‑temperature‑ctrl)　🔴
`STM32L031` `TEC 半导体制冷` `NTC` `复合数字滤波` `BLE` `OLED` `编码器`
- TEC 半导体制冷片冷热双向驱动，多路 NTC 测温。
- 软件复合滤波 + 硬件 LDO/π 型滤波，实现 **±0.25 ℃** 稳态控温精度。
- OLED + 编码器本地交互、BLE 安卓 APP、VOFA+ 上位机多端观测；非阻塞 FSM 管理制冷/加热/恒温工况。

### 08 · [ESP32-S3 智能视觉触控终端（多形态版）](./personal_project/08_esp32‑smart‑terminals)　🟡
`ESP-IDF` `FreeRTOS` `LVGL` `FT6336G` `ILI9341V` `I2S` `ES8311` `OV2640/OV3660` `SDIO` `WebSocket` `MQTT` `BLE` `NVS` `OTA`
- 含可视门铃、触控语音、视觉语音三种形态子工程。
- FT6336G 电容触摸 + ILI9341V SPI 彩屏，LVGL 图形交互；I2S + ES8311 音频采播。
- 环形缓冲区隔离音视频避免网络阻塞丢包；SDIO TF 卡存储、BLE 配网、NVS 参数持久化、OTA 升级。

### 09 · [物联网与消费电子综合实践](./personal_project/09_iot‑basic‑projects)　🟢
`杰理 Byle-U8 SDK` `A2DP` `离线 KWS` `AD 按键` `红外` `WS2812` `阿里云 MQTT` `AD8232` `ADC/DMA` `EEPROM`
- 离线语音蓝牙音箱：A2DP 音频播放 + 离线 KWS 唤醒，语音/按键/红外三路输入统一事件分发控制彩灯。
- ESP32 智能温室：多传感器采集、MQTT 上云、APP 远程管控、断线重连、EEPROM 参数保存。
- STM32 心电监测仪：ADC-DMA 采集 AD8232 信号，驱动层-业务层-调试层三层架构，串口上报波形。

---

## 🚧 在研方向（预研阶段 · 无交付物）

> 下面这些是**已拆解清楚、但尚未开工**的技术路线。列出来是因为问题本身有工程价值，不是为了充数：
> 所有指标都是目标值，凡是没验证的都写明了「待验证」。

### 00 · [具身智能边缘计算平台](./personal_project/00_embodied_ai_platform)　🚧 在研 / 预研
`RK3588 (Linux + 6 TOPS NPU)` `双足 / 四足平台` `大脑—小脑异构` `RL 端侧推理` `Sim-to-Real`

- **要回答的问题**：Linux 侧 50 Hz 策略推理与 MCU 侧 20 kHz 电流环相差三个数量级，如何构造一条延迟可界定的通路，让上层调度抖动不传导到关节。
- **三条硬约束**：安全边界必须在实时域内闭合（Linux 崩了机器人要能自己收腿）· 决策与电流环之间必须有插值限幅层 · 每一段延迟单独打点可测量。
- **已完成**：分层架构、四个核心风险的拆解、四阶段验收指标、选型权衡与否决理由（[方案文档](./personal_project/00_embodied_ai_platform)）。
- **未开始**：硬件采购、BSP 适配、任何一行控制代码 —— 这份文档的价值在于想清楚，而不是假装做完了。
- **为什么值得立项**：小脑侧复用 01 的 CAN 主控与故障安全，执行侧复用 03 的 FOC，大脑侧复用 05 的 RK3568 与 Socket 经验，每一层都有已交付项目垫底，缺的是把它们接起来并对齐时间尺度这一环。

---

## 📚 嵌入式专题技术文档（26 篇 + 4 篇工具笔记）

*工程实践总结：原理说明 + 伪代码 + 实测踩坑记录。完整目录见 [docs/README.md](./docs/README.md)。*

**A. 量产固件与可靠性**
- 🔴 [《量产固件的 OTA 方案：双分区 + CRC + 断电回滚》](./docs/firmware‑mass‑produce.md)
- 🔴 [《非阻塞 FSM 设计：从阻塞 Delay 到 Tick 驱动》](./docs/realtime‑fsm.md)
- 🔴 [《Cortex-M HardFault 故障定位与调试手段》](./docs/cortex-m-hardfault-debug.md)
- 🔴 [《FreeRTOS 工程踩坑集：栈溢出、优先级反转、临界区》](./docs/freertos-trap-notes.md)

**B. 电机与控制**
- 🔴 [《无电流采样下的 FOC 电压模式控制实践》](./docs/motor‑foc‑control.md)
- 🔴 [《无感 FOC 转子位置观测器：滑模观测、PLL 提取与低速高频注入》](./docs/sensorless-foc-observer.md)
- 🔴 [《PID 控制工程实战：温控/电机调参、超调抑制、前馈》](./docs/pid-engineering-tuning.md)

**C. 通信协议与外设**
- 🟡 [《Modbus-RTU + COBS/CRC 协议栈设计与踩坑》](./docs/comm‑protocol‑driver.md)
- 🟡 [《CAN 总线开发：过滤器、错误状态、总线离线恢复》](./docs/can-bus-practice.md)
- 🟡 [《MQTT 客户端实战：断连重连、遗嘱消息、QoS 选型》](./docs/mqtt-practice-note.md)
- 🔴 [《RK3568 边缘网关：Socket 通信、断线重连、本地缓存》](./docs/linux-edge-socket-cache.md)
- 🟡 [《ADC 采集工程实践：偏移校准、滤波、强电噪声抑制》](./docs/adc-engineering-practice.md)

**D. 音视频与 GUI**
- 🔴 [《I2S/PCM 音频实战：爆音、底噪、pop 噪声定位与排查》](./docs/audio-pop-noise-debug.md)
- 🟡 [《ESP-IDF Ringbuf 环形缓冲区实战》](./docs/esp32-ringbuf-practice.md)
- 🔴 [《LVGL 嵌入式 GUI 实战：内存管理、帧率优化、泄漏排查》](./docs/lvgl-gui-practice.md)

**E. 具身智能专题（11 篇）** —— 00 平台（Rock 5B + Qmini，规划中）＋ 01 双平台（🐕 HyperDog 单机 / 🐈 Nybble 分层）

- **E.1 异构平台与策略部署**（Rock 5B + Qmini · 00 在研）：🔴 [大脑—小脑异构架构](./docs/embodied-brain-body-arch.md) ｜ 🔴 [RK3588 NPU 部署链路](./docs/rk3588-npu-deploy.md) ｜ 🔴 [Sim-to-Real 训练与迁移](./docs/sim-to-real-legged.md) ｜ 🟡 [Qmini 上板路线](./docs/qmini-rock5b-bringup.md)
- **E.2 四足整机实时控制**（HyperDog · 单机 MCU + CAN）：🔴 [整机固件架构](./docs/hyperdog-arch.md) ｜ 🔴 [CAN 关节协议](./docs/hyperdog-can-protocol.md) ｜ 🔴 [IMU 姿态融合与振动抑制](./docs/hyperdog-imu-fusion.md) ｜ 🔴 [Trot 步态调度](./docs/hyperdog-gait-scheduler.md) ｜ 🔴 [故障安全分级保护](./docs/hyperdog-safety.md) ｜ 🟡 [整机标定与分步调参](./docs/hyperdog-calibration.md)
- **E.3 ROS 2 + micro-ROS 分层**（Nybble · 链路分层）：🟡 [把实时边界挪到链路上](./docs/ros2-micro-ros-layered.md)

**P. 拓展笔记**：[docs/tech-notes](./docs/tech-notes/README.md) —— 嵌入式 C 编码陷阱、STM32 外设踩坑、FreeRTOS 零散案例、J-Link / 逻辑分析仪调试技巧。

---

## 🗂 仓库目录

```text
CYY-portfolio/
├── README.md                                         # 作品集首页（本文件）
├── README_EN.md                                      # 英文版首页
├── showcase/                                         # 五套作品集策展页（A 动态 / B 音频 / C 结构 / D 硬件 / E 数据）
├── assets/                                           # 演示媒体（波形 / 音频 / 板卡照 / 数据，多数待补）
├── docs/                                             # 26 篇专题技术文档
│   └── tech-notes/                                   # 嵌入式 C / 驱动 / RTOS 碎片笔记
├── internship/                                       # 实习项目（已脱敏）
│   ├── GAOXINXING/
│   │   ├── internship_project_01-power-sequencer     # 八路电源时序器（量产）
│   │   ├── internship_project_02-digital-audio-mixer # RISC-V 数字调音台
│   │   └── internship_project_03-usb-audio-interface # 桌面 USB 声卡
│   └── ANOTECH/                                      # 产品结构设计实习（仅文档报告）
└── personal_project/                                 # 9 套实战系统 + 1 项在研预研
    ├── 00_embodied_ai_platform                       # 🚧 具身智能边缘计算平台（预研方案）
    ├── 01_quadruped_robotics                         # 四足双平台（MCU 主控 + ROS 2）
    │   ├── hyperdog_rtos_control                     # 单机 MCU + CAN（设计文档）
    │   └── nybble_micro_ros                          # ROS 2 + micro-ROS（方案设计）
    ├── 02_gnss-flight-controller                     # 北斗/GNSS 定位飞控
    ├── 03_foc-bldc-motor                             # 无刷电机 FOC 控制
    ├── 04_dual-axis-motion-control                   # 双轴运动控制（直流+步进）
    ├── 05_industrial-edge-gateway                    # 工业物联网边缘网关（MCU+Linux）
    ├── 06_bms-and-charger                            # 储能 BMS 与智能充电桩
    ├── 07_stm32-tec-temperature-ctrl                 # 冷热双循环高精度温控
    ├── 08_esp32-smart-terminals                      # ESP32-S3 智能视觉终端
    └── 09_iot-basic-projects                         # 物联网与消费电子综合实践
```

---

## 🏷 标识说明

> 每个项目页末尾都附有同一套说明（只解释该页实际用到的标识），从任一入口进来都不会看不懂标记。

| 标识 | 含义 |
|---|---|
| 👑 | 代表作：完成度最高、软硬件全栈闭环，优先面试展示 |
| ⭐ | 该岗位方向建议优先阅读的项目 |
| 🔴 | 进阶 / 量产级难点：实时性、故障安全、复杂算法落地 |
| 🟡 | 中等模块：嵌入式高频技术点，校招常考 |
| 🟢 | 基础实践：外设与协议基础验证，体现技术广度 |
| 🚧 | 在研 / 预研：方案与验证路线已完成，尚无交付物与实测数据，指标为目标值 |

> 下方「嵌入式专题技术文档」里的 🔴 🟡 只表示该篇的**阅读难度**，与项目等级无关。

---

## ⚠️ 声明

1. 实习相关代码与资料均已脱敏，不含公司涉密业务源码、内部物料参数；
2. 文档为原始创作素材，建议下载至本地阅读；
3. 个人项目文档与源码仅供学习交流；实习项目知识产权归属对应企业，禁止商用。
