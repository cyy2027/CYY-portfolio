# 陈艺苑 · 嵌入式软件工程师作品集

**2027 届 ｜ 求职方向：嵌入式软件 / 固件开发（MCU 固件 · RTOS · 嵌入式 Linux）**

> 2 段实习（含 1 个量产交付项目）· 9 套完整实战系统 · 1 项在研预研 · 20 篇专题技术文档
> 覆盖：底层固件与 BSP 驱动 · 工业控制与边缘计算 · 机器人 / 飞控 · 音视频终端 · 医疗仪器

📄 [在线版作品集（飞书，排版更精美）](https://p3-flow-sign.byteimg.com/tos-cn-i-ik7evvg4ik/rc/pc/super_tool/3b09bc56cfa24c99ad621f10bb514f7e.html?lk3s=8e244e95&rcl=20261106230041082128A8B8BA412E1300&rrcfp=f512b60b&x-expires=1795273296&x-signature=dPK0icz23Ui18jg5jo2VeMnGbRE%3D&use_xbridge3=true&loader_name=forest&need_sec_link=1&sec_link_scene=im&theme=dark)

> 💡 **阅读方式**：项目统一按 `README.md`（简介与亮点）、`design.md`（软硬件详细设计）、`docs/`（专题文档 / 调试记录 / 复盘）组织。**源码与原理图按各项目实际情况提供**，未纳入仓库的会在对应 README 首屏标注，不做含糊承诺。建议 clone 到本地用 VS Code + Markdown 预览阅读。

---

## ⏱ 30 秒速览

| 项目 | 关键交付 |
|---|---|
| **① 电源时序器国产化替代**（实习 · 已量产） | 陌生国产 Cortex-M0 平台 **2 周**完成 Bring-up 并交付量产；非阻塞 FSM 八路时序控制 + IWDG 强电安全兜底，闭环 8 类软硬件耦合 Bug |
| **② HyperDog 四足机器狗主控固件** | FreeRTOS 分布式主控架构，对角 Trot 步态 + 五次多项式足端轨迹，**1 kHz** 周期通过 CAN 下发 12 路关节指令 |
| **③ 工业物联网边缘网关**（MCU + Linux） | RS485 / LoRa / CAN 多协议采集 → MQTT 端边云闭环；RK3568 上 TCP Socket + SQLite 本地缓存 + 断线重连 |

---

## 🎯 按岗位方向查找（推荐阅读顺序）

| 目标岗位方向 | 建议先看 |
|---|---|
| 芯片原厂 FAE / 固件 / BSP 驱动 | ⭐[电源时序器（量产）](./internship/GAOXINXING/internship_project_01-power-sequencer) → ⭐[冷热双循环温控](./personal_project/07_stm32‑tec‑temperature‑ctrl) → [储能 BMS 与充电桩](./personal_project/06_bms‑and‑charger) |
| 工业控制 / 汽车电子 / 嵌入式 Linux | ⭐[工业边缘网关](./personal_project/05_industrial‑edge‑gateway) → [BMS 与充电桩](./personal_project/06_bms‑and‑charger) → [调音台-反馈抑制器](./internship/GAOXINXING/internship_project_02-digital-audio-mixer) |
| 机器人 / 飞控 / 运动控制 | ⭐[四足双平台（HyperDog / Nybble）](./personal_project/01_quadruped_robotics) → ⭐[北斗 GNSS 飞控](./personal_project/02_gnss-flight-controller) → [FOC 无刷电机](./personal_project/03_foc‑bldc‑motor) → [双轴运动控制](./personal_project/04_dual‑axis‑motion‑control) |
| 音视频终端 / 安防 / AIoT | ⭐[ESP32-S3 智能终端](./personal_project/08_esp32‑smart‑terminals) → [USB 声卡](./internship/GAOXINXING/internship_project_03-usb-audio-interface) → [物联网综合实践](./personal_project/09_iot‑basic‑projects) |
| 医疗器械 / 仪器仪表 / 传感器 | [冷热双循环温控](./personal_project/07_stm32‑tec‑temperature‑ctrl) → [心电采集监测](./personal_project/09_iot‑basic‑projects) |

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

> 📎 每个目录内置项目方案、流程图、开发总结；[实习证明与部门概览](./internship/GAOXINXING/README.md)。
> 📎 **[安诺特实习](./internship/ANOTECH)** 为早期**产品结构设计**实习总结（非固件代码），建立了三维空间直觉、硬件制造视野与跨部门协同经验。
> ⚠️ 严格遵守保密协议，实习代码与资料均已脱敏，不含企业涉密业务源码与内部物料参数。

---

## 💎 个人完整实战系统（9 套）

### 🐕🐈 01 · [赛博猫狗：四足双平台（MCU 单机 + ROS 2 分层）](./personal_project/01_quadruped_robotics)　👑 代表作

同一个母题做两遍 —— 用来说明**实时边界到底该划在哪一层**。

**🐕 [HyperDog · MCU 单机主控](./personal_project/01_quadruped_robotics/hyperdog_rtos_control)**　`Cortex-M` `FreeRTOS` `IMU(ICM20602)` `CAN` `Trot 步态` `五次多项式`
- 对角 Trot 步态调度 + 五次多项式足端轨迹，起止点速度、加速度均为 0，落地冲击小。
- CAN 一主多从：主控 **1 kHz** 下发 12 自由度关节目标角，200 Hz 步态主循环，关节侧各自下沉 FOC。
- 自适应互补滤波（冲击检测动态调权）+ 四级故障保护与平滑收腿，配套 6 篇专题文档。

**🐈 [Nybble · ROS 2 + micro-ROS 分层](./personal_project/01_quadruped_robotics/nybble_micro_ros)**　`ROS 2 Foxy` `XRCE-DDS` `UART+DMA` `FreeRTOS` 🚧
- 规划层搬到 Linux（IK / 步态 / 姿态补偿），MCU 只做实时执行，12 路角度以 Topic 周期下发。
- DMA + 环形缓冲 + 队列三段式消抖；TIM1/TIM8 原生 PWM 取代 PCA9685 扩展板。
- 当前**仅架构设计**，无实机与代码 —— 与左边那套同题不同解，对照看取舍更清楚。

### ✈️ 02 · [北斗 / GNSS 定位飞控系统](./personal_project/02_gnss-flight-controller)
`STM32H723/F405` `FreeRTOS` `IMU/GNSS 融合` `串级 PID` `Dshot-600` `ELRS(CRSF)` `MSP OSD` `ESP8285` `Qt5`
- ICM-42688-P + 气压计 + M10 北斗/GNSS 多源融合，多级串级 PID 实现姿态稳定、定高、位置定点。
- Dshot-600 高速数字电调驱动；ELRS CRSF 远距离遥控遥测；移植 IRC Tramp 图传控制与 MSP OSD 画面叠加。
- 双芯片架构：ESP8285 负责 Wi-Fi 透传调参；Qt5 上位机（桌面/安卓）传感器可视化 + 参数一键下发。

### ⚙️ 03 · [STM32 无刷电机 FOC 控制系统](./personal_project/03_foc‑bldc‑motor)
`FreeRTOS` `SVPWM` `电压模式 FOC` `AS5600 磁编码器` `CAN`
- 无电流采样条件下的电压模式 FOC 矢量控制实现，SVPWM 调制。
- AS5600 磁编码器位置采集；CAN 与上位机交互完成电机实时性能调优。

### 🎯 04 · [STM32 双轴运动控制平台（直流 + 步进）](./personal_project/04_dual‑axis‑motion‑control)
`STM32F103` `IR2104S H 桥` `霍尔编码器` `增量式 PID` `S 型加减速` `CAN` `Modbus-RTU`
- H 桥驱动直流电机、脉冲-方向模式驱动步进电机，硬件过流保护与死区控制。
- 直流轴增量式 PID 闭环调速；步进梯形/S 型加减速抑制启停丢步。
- CAN、Modbus-RTU 下发指令，支持双轴独立调速、正反转与多轴联动。

### 🛰 05 · [工业物联网边缘网关系统（MCU + Linux）](./personal_project/05_industrial‑edge‑gateway)
`FreeRTOS` `Modbus-RTU` `CAN` `LLCC68-LoRa` `W5500` `MQTT` `cJSON` `RK3568(Linux)` `Socket` `SQLite`
- MCU 端：RS485 / LoRa / CAN 多协议采集，协议转换后转发 MQTT。
- Linux 端：RK3568 部署 MQTT Broker 与 SQLite 本地缓存。
- TCP Socket 打通 MCU-Linux 通信，断线重连 + 本地缓存，形成完整端边云链路。

### 🔋 06 · [储能电池管理与安全充电系统（BMS + 充电桩）](./personal_project/06_bms‑and‑charger)
`BQ76920` `I2C(CRC-8)` `CAN` `IAP Bootloader` `RC522 RFID` `ECB02 BLE` `OLED`
- BMS：BQ76920 采集 5 串锂电电压/电流/NTC 温度，过压欠压过流保护，OCV 查表 + 温度补偿估算 SOC。
- 充电桩：自研 IAP 固件升级（CRC 校验 + 断电回滚防变砖）；RFID 刷卡扣费；强电继电器安全控制逻辑。

### 🌡 07 · [STM32 冷热双循环高精度温控系统](./personal_project/07_stm32‑tec‑temperature‑ctrl)
`STM32L031` `TEC 半导体制冷` `NTC` `复合数字滤波` `BLE` `OLED` `编码器`
- TEC 半导体制冷片冷热双向驱动，多路 NTC 测温。
- 软件复合滤波 + 硬件 LDO/π 型滤波，实现 **±0.25 ℃** 稳态控温精度。
- OLED + 编码器本地交互、BLE 安卓 APP、VOFA+ 上位机多端观测；非阻塞 FSM 管理制冷/加热/恒温工况。

### 📱 08 · [ESP32-S3 智能视觉触控终端（多形态版）](./personal_project/08_esp32‑smart‑terminals)
`ESP-IDF` `FreeRTOS` `LVGL` `FT6336G` `ILI9341V` `I2S` `ES8311` `OV2640/OV3660` `SDIO` `WebSocket` `MQTT` `BLE` `NVS` `OTA`
- 含可视门铃、触控语音、视觉语音三种形态子工程。
- FT6336G 电容触摸 + ILI9341V SPI 彩屏，LVGL 图形交互；I2S + ES8311 音频采播。
- 环形缓冲区隔离音视频避免网络阻塞丢包；SDIO TF 卡存储、BLE 配网、NVS 参数持久化、OTA 升级。

### 🧩 09 · [物联网与消费电子综合实践](./personal_project/09_iot‑basic‑projects)
`杰理 Byle-U8 SDK` `A2DP` `离线 KWS` `AD 按键` `红外` `WS2812` `阿里云 MQTT` `AD8232` `ADC/DMA` `EEPROM`
- 离线语音蓝牙音箱：A2DP 音频播放 + 离线 KWS 唤醒，语音/按键/红外三路输入统一事件分发控制彩灯。
- ESP32 智能温室：多传感器采集、MQTT 上云、APP 远程管控、断线重连、EEPROM 参数保存。
- STM32 心电监测仪：ADC-DMA 采集 AD8232 信号，驱动层-业务层-调试层三层架构，串口上报波形。

---

## 🚧 在研方向（预研阶段 · 无交付物）

> 下面这些是**已拆解清楚、但尚未开工**的技术路线。列出来是因为问题本身有工程价值，不是为了充数：
> 所有指标都是目标值，凡是没验证的都写明了「待验证」。

### 🤖 00 · [具身智能边缘计算平台](./personal_project/00_embodied_ai_platform)　🚧 方案设计阶段
`RK3588 (Linux + 6 TOPS NPU)` `双足 / 四足平台` `大脑—小脑异构` `RL 端侧推理` `Sim-to-Real`

- **要回答的问题**：Linux 侧 50 Hz 策略推理与 MCU 侧 20 kHz 电流环相差三个数量级，如何构造一条延迟可界定的通路，让上层调度抖动不传导到关节。
- **三条硬约束**：安全边界必须在实时域内闭合（Linux 崩了机器人要能自己收腿）· 决策与电流环之间必须有插值限幅层 · 每一段延迟单独打点可测量。
- **已完成**：分层架构、四个核心风险的拆解、四阶段验收指标、选型权衡与否决理由（[方案文档](./personal_project/00_embodied_ai_platform)）。
- **未开始**：硬件采购、BSP 适配、任何一行控制代码 —— 这份文档的价值在于想清楚，而不是假装做完了。
- **为什么值得立项**：小脑侧复用 01 的 CAN 主控与故障安全，执行侧复用 03 的 FOC，大脑侧复用 05 的 RK3568 与 Socket 经验，每一层都有已交付项目垫底，缺的是把它们接起来并对齐时间尺度这一环。

---

## 📚 嵌入式专题技术文档（20 篇 + 4 篇工具笔记）

*工程实践总结：原理说明 + 伪代码 + 实测踩坑记录。完整目录见 [docs/README.md](./docs/README.md)。*

**A. 量产固件与可靠性**
- 🔴 [《量产固件的 OTA 方案：双分区 + CRC + 断电回滚》](./docs/firmware‑mass‑produce.md)
- 🔴 [《非阻塞 FSM 设计：从阻塞 Delay 到 Tick 驱动》](./docs/realtime‑fsm.md)
- 🔴 [《Cortex-M HardFault 故障定位与调试手段》](./docs/cortex-m-hardfault-debug.md)
- 🔴 [《FreeRTOS 工程踩坑集：栈溢出、优先级反转、临界区》](./docs/freertos-trap-notes.md)

**B. 电机与控制**
- 🔴 [《无电流采样下的 FOC 电压模式控制实践》](./docs/motor‑foc‑control.md)
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

**E. HyperDog 足式机器人专题（6 篇）**
- 👑 [整机固件架构设计](./docs/hyperdog-arch.md) ｜ 🔴 [CAN 分布式关节协议](./docs/hyperdog-can-protocol.md) ｜ 🔴 [IMU 姿态融合与振动抑制](./docs/hyperdog-imu-fusion.md)
- 🔴 [Trot 步态调度与足端轨迹](./docs/hyperdog-gait-scheduler.md) ｜ 🔴 [整机故障安全与分级保护](./docs/hyperdog-safety.md) ｜ 🟡 [整机标定与分步调参](./docs/hyperdog-calibration.md)

**P. 拓展笔记**：[docs/tech-notes](./docs/tech-notes/README.md) —— 嵌入式 C 编码陷阱、STM32 外设踩坑、FreeRTOS 零散案例、J-Link / 逻辑分析仪调试技巧。

---

## 🗂 仓库目录

```text
CYY-portfolio/
├── README.md                                         # 作品集首页（本文件）
├── docs/                                             # 20 篇专题技术文档
│   └── tech-notes/                                   # 嵌入式 C / 驱动 / RTOS 碎片笔记
├── internship/                                       # 实习项目（已脱敏）
│   ├── GAOXINXING/
│   │   ├── internship_project_01-power-sequencer     # 八路电源时序器（量产）
│   │   ├── internship_project_02-digital-audio-mixer # RISC-V 数字调音台
│   │   └── internship_project_03-usb-audio-interface # 桌面 USB 声卡
│   └── ANOTECH/                                      # 产品结构设计实习（仅文档报告）
└── personal_project/                                 # 9 套实战系统 + 1 项在研预研
    ├── 00_embodied_ai_platform                       # 🚧 具身智能边缘计算平台（预研方案）
    ├── 01_quadruped_robotics                         # 🐕🐈 四足双平台（MCU 主控 + ROS 2）
    │   ├── hyperdog_rtos_control                     # 单机 MCU + CAN（设计文档）
    │   └── nybble_micro_ros                          # ROS 2 + micro-ROS（方案设计）
    ├── 02_gnss-flight-controller                     # ✈️ 北斗/GNSS 定位飞控
    ├── 03_foc-bldc-motor                             # ⚙️ 无刷电机 FOC 控制
    ├── 04_dual-axis-motion-control                   # 🎯 双轴运动控制（直流+步进）
    ├── 05_industrial-edge-gateway                    # 🛰 工业物联网边缘网关（MCU+Linux）
    ├── 06_bms-and-charger                            # 🔋 储能 BMS 与智能充电桩
    ├── 07_stm32-tec-temperature-ctrl                 # 🌡 冷热双循环高精度温控
    ├── 08_esp32-smart-terminals                      # 📱 ESP32-S3 智能视觉终端
    └── 09_iot-basic-projects                         # 🧩 物联网与消费电子综合实践
```

---

## 🏷 标识说明

| 标识 | 含义 |
|---|---|
| 👑 | 代表作：完成度最高、软硬件全栈闭环、有实物展示，优先面试展示 |
| 🚧 | 在研 / 预研：方案与验证路线已完成，尚无交付物与实测数据，指标为目标值 |
| ⭐ | 该岗位方向建议优先阅读的项目 |
| 🔴 | 进阶 / 量产级难点：实时性、故障安全、复杂算法落地 |
| 🟡 | 中等模块：嵌入式高频技术点，校招常考 |
| 🟢 | 基础实践：外设与协议基础验证，体现技术广度 |

---

## ⚠️ 声明

1. 实习相关代码与资料均已脱敏，不含公司涉密业务源码、内部物料参数；
2. 文档为原始创作素材，建议下载至本地阅读；
3. 个人项目文档与源码仅供学习交流；实习项目知识产权归属对应企业，禁止商用。
