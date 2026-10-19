# 陈艺苑｜2027 届 嵌入式软件工程师 作品集

> 📦 **仓库内容说明**
> 本仓库包含：实习脱敏工程、**8 套个人完整实战系统**（涵盖底层控制、工业通信、物联网终端等，内含14个子工程原型）、嵌入式专题技术文档。内容涵盖原理图、PCB、工程源码、项目说明、设计文档及实物演示截图。

> 💡 **阅读建议**：推荐使用 VS Code 打开浏览，搭配 Markdown 预览插件体验更佳。

**难度与推荐标识说明：**
* 👑 **重点系统项目**：完整系统架构设计，包含复杂控制闭环、多任务通信，为仓库代表性标杆工程
* 🔴 **进阶项目**：贴近量产工程难点，侧重实时性、故障安全、复杂算法实现
* 🟡 **中等项目**：覆盖嵌入式固件高频技术模块，适合校招能力考察
* 🟢 **基础实践项目**：外设、协议基础验证，用于技术能力拓展

---

### 🏢 实习脱敏工程（全部脱敏，无企业涉密源码）
*🟡 ~ 🔴 工业嵌入式真实业务项目，包含状态机、音频DSP、总线通信、软硬件排错、完整技术文档沉淀*

**高新兴科技集团｜音频部**
* 📂 [`./internship/GAOXINXING/internship_project_01-power-sequencer`](./internship/GAOXINXING/internship_project_01-power-sequencer) 🔴
  * 国产 Cortex‑M0 芯片替代，非阻塞 FSM 八路电源时序控制、ADC 矩阵按键、HLW8032 电能计量、IWDG看门狗，2 周独立完成固件适配并交付量产。
* 📂 [`./internship/GAOXINXING/internship_project_02‑digital‑audio‑mixer`](./internship/GAOXINXING/internship_project_02-digital-audio-mixer) 🔴
  * RISC‑V 音频 DSP Bring‑up，I2S 音频数据流、COBS‑CRC32 自定义串口协议、移频‑反馈抑制算法预研，解决爆音、底噪、引脚冲突等软硬件耦合问题。
* 📂 [`./internship/GAOXINXING/internship_project_03‑usb‑audio‑interface`](./internship/GAOXINXING/internship_project_03-usb-audio-interface) 🟡
  * 硬件未到位情况下完成上层业务框架开发，实现音频混音路由、消爆音处理、全局状态管理、分层软件架构设计。

> 补充：另有安诺特结构方向实习经历，侧重硬件DFM评审、工程图纸输出，为文档报告类材料，[点击查看](./internship/ANOTECH/README.md)；无固件源码，**不纳入本代码仓库目录树**，完整经历可查阅个人简历。

*(注：每个实习目录内置项目方案、流程图、开发总结，严格遵守保密协议，无企业涉密源码)*

---

### 🚀 8 套个人完整实战系统
*(注：每套系统目录结构统一：包含 README 简介、design 详细设计、docs 文档/图片、hardware 原理图 PCB、实物演示与排错复盘)*

#### 👑 核心控制与通信系统（仓库标杆）
**1. 无刷电机 FOC 控制系统** 👑
* 📂 [`./personal_project/01_foc‑bldc‑motor`](./personal_project/01_foc-bldc-motor)
* 🛠️ `FreeRTOS · SVPWM · AS5600 · CAN`
* 📝 在无电流采样约束条件下，实现电压模式 FOC 多环控制；基于 TIM1 输出 40kHz 中心对齐 PWM，完成 Clarke/Park 坐标变换与 SVPWM 调制。FreeRTOS 拆分传感器、命令、遥测多任务，CAN 总线支持多节点远程调速。

**2. 基于 STM32 双轴运动控制平台（直流+步进）** 👑
* 📂 [`./personal_project/02_dual‑axis‑motion‑control`](./personal_project/02_dual-axis-motion-control)
* 🛠️ `STM32F103 · H桥 · 霍尔编码器 · 增量式PID · S型加减速`
* 📝 实现 H 桥驱动与霍尔正交解码；直流轴增量式 PID 闭环调速，步进轴梯形/S型加减速抑制高速启停丢步。**实测指标**：0‑3000rpm 宽域调速空载误差 <1%，突加负载200ms恢复稳态；双轴联动响应延迟 ≤10ms。

**3. 工业物联网边缘网关系统（MCU + Linux）** 👑
* 📂 [`./personal_project/03_industrial‑edge‑gateway`](./personal_project/03_industrial-edge-gateway)
  * *（内含 `mcu_gateway` 与 `linux_edge_gateway` 两个子目录）*
* 🛠️ `FreeRTOS · Modbus‑RTU · CAN · LoRa · MQTT · RK3568(Linux) · Socket · SQLite`
* 📝 **MCU层**完成RS485/LoRa/CAN多协议采集与协议转MQTT；**Linux层**基于 RK3568 部署MQTT Broker与SQLite本地缓存，Socket TCP与MCU终端通信，实现断线重连、数据缓存的完整端云协同链路。

**4. 四轴无人机飞控系统** 👑
* 📂 [`./personal_project/04_quadcopter‑flight‑controller`](./personal_project/04_quadcopter-flight-controller)
* 🛠️ `STM32F4 · FreeRTOS · MPU6050 · 互补滤波 · 串级PID`
* 📝 移植 FreeRTOS，拆分姿态解算、PID闭环、电机驱动、遥控接收独立任务；互补滤波融合六轴传感器数据，串级PID输出PWM驱动无刷电机；通过任务优先级调优，解决高速飞行机体剧烈震荡故障。

**5. 储能电池管理与安全充电系统（BMS+充电桩）** 🔴
* 📂 [`./personal_project/05_bms‑and‑charger`](./personal_project/05_bms-and-charger)
  * *（内含 `bms_system` 与 `smart_charger` 两个子目录）*
* 🛠️ `BQ76920 · I2C(CRC‑8) · CAN · IAP Bootloader · RFID`
* 📝 **BMS**：基于AFE芯片完成5串锂电池电压、电流、NTC温度采集，配置过压欠压过流保护，OCV查表+温度补偿实现SOC估算；**充电桩**：独立开发IAP固件升级（CRC校验+断电回滚防变砖），实现RFID扣费、强电继电器安全控制逻辑。

#### 🔴 进阶｜传感器闭环与智能交互
**6. STM32 冷热双循环温控系统** 🔴
* 📂 [`./personal_project/06_stm32‑tec‑temperature‑ctrl`](./personal_project/06_stm32-tec-temperature-ctrl)
* 🛠️ `STM32L031 · TEC双向控温 · 复合滤波 · BLE`
* 📝 针对TEC热惯性大、容易超调问题，采用PID闭环+多重数字滤波，实现 **±0.25℃ 稳态精度**；非阻塞状态机管理制冷/加热/恒温多工况；支持本地按键OLED操控、BLE安卓APP、VOFA+上位机波形观测。

**7. ESP32‑S3 智能视觉触控终端系统（多形态版）** 🟡
* 📂 [`./personal_project/07_esp32‑smart‑terminals`](./personal_project/07_esp32-smart-terminals)
  * *（内含 `touch_voice_terminal`、`vision_voice_terminal`、`video_doorbell` 三个子目录）*
* 🛠️ `ESP‑IDF · FreeRTOS · LVGL · I2S · OV2640/3660 · WebSocket · MQTT · OTA`
* 📝 涵盖触控UI终端、便携视觉语音终端、智能可视门铃三种产品形态；Ringbuf环形缓冲区隔离音视频采集与网络传输，避免网络阻塞丢包；完成多任务优化、BLE配网、NVS参数持久化、OTA在线升级整套业务。

#### 🟢 基础实践｜嵌入式基础与物联网综合实践
**8. 嵌入式基础与物联网综合实践** 🟢
* 📂 [`./personal_project/08_iot‑basic‑projects`](./personal_project/08_iot-basic-projects)
  * *（内含 `offline_voice_speaker`、`smart_greenhouse`、`ecg_monitor` 三个子目录）*
* 🛠️ `杰理SDK · A2DP · 阿里云MQTT · AD8232 · ADC/DMA`
* 📝 包含离线语音蓝牙音箱（A2DP、离线KWS识别、按键红外多输入）；ESP32智能温室大棚（传感器采集、MQTT双向通信、断线重连、EEPROM参数保存）；心电采集监测仪（DMA高速ADC采样，三层软件架构，上位机波形绘制）。

---

### 📚 嵌入式专题技术文档 & 学习笔记
*🟡 ~ 🔴 聚焦量产嵌入式真实工程问题，伪代码 + 实测踩坑记录，持续迭代新增实战文档*
> 📎 [点击进入完整文档总览页面，共13篇专题笔记](./docs/README.md)

* 📂 `./docs`
  * [《量产固件的OTA方案：双分区+CRC+断电回滚》](./docs/firmware‑mass‑produce.md) 🔴：Bootloader、A/B分区OTA断电保护、看门狗、国产MCU迁移踩坑
  * [《非阻塞FSM设计：从阻塞Delay到Tick驱动》](./docs/realtime‑fsm.md) 🔴：裸机&FreeRTOS有限状态机，状态死锁、标志位竞争排查
  * [《Modbus‑RTU + COBS/CRC协议栈设计与踩坑》](./docs/comm‑protocol‑driver.md) 🟡：环形缓冲区、Modbus、COBS帧封装，总线粘包超时实战
  * [《无电流采样下的FOC电压模式控制实践》](./docs/motor‑foc‑control.md) 👑：PMSM矢量变换、SVPWM、电机调参故障排查
* 📂 `./docs/tech‑notes/`
  * 包含嵌入式C、STM32驱动、FreeRTOS实战、调试工具（J‑Link/逻辑分析仪）碎片踩坑笔记。

---

### 🗂️ 仓库目录总览
```text
├── README.md                             # 作品集首页（本文件）
├── docs/                                 # 嵌入式专题技术文档与学习笔记
├── internship/                           # 实习项目（高新兴、安诺特）
│   ├── GAOXINXING/
│   │   ├── internship_project_01‑power‑sequencer      # 八路电源时序器
│   │   ├── internship_project_02‑digital‑audio‑mixer  # RISC‑V 数字调音台
│   │   └── internship_project_03‑usb‑audio‑interface  # USB 音频声卡
│   └── ANOTECH/                         # 安诺特实习（仅文档报告，无固件源码，不在代码主目录展示）
└── personal_project/                     # 8 套个人实战系统
    ├── 01_foc‑bldc‑motor                 # 👑 FOC无刷电机控制
    ├── 02_dual‑axis‑motion‑control       # 👑 双轴运动控制（直流+步进）
    ├── 03_industrial‑edge‑gateway        # 👑 工业物联网边缘网关（含 MCU / Linux 双核）
    ├── 04_quadcopter‑flight‑controller   # 👑 四轴无人机飞控系统
    ├── 05_bms‑and‑charger                # 🔴 储能 BMS 与安全充电（含 BMS / 充电桩）
    ├── 06_stm32‑tec‑temperature‑ctrl     # 🔴 冷热双循环高精度温控系统
    ├── 07_esp32‑smart‑terminals          # 🟡 ESP32 智能终端（含触控 / 视觉 / 门铃）
    └── 08_iot‑basic‑projects             # 🟢 基础与 IoT 实践（含音箱 / 大棚 / 心电）

```
## ⚠️ 声明
1. 实习相关代码、资料均已做脱敏处理，不包含公司涉密业务源码、内部物料参数；
2. 文档为原始作品素材，请下载至本地阅读；
3. 个人项目文档与源码仅供学习交流；实习项目知识产权归属对应企业，禁止商用。