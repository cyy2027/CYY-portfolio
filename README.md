# 陈艺苑｜2027 届 嵌入式软件工程师 作品集

> 📦 **仓库内容说明**
> 本仓库包含：实习脱敏工程、**8套个人完整实战系统**（涵盖底层控制、工业通信、物联网终端，内含14个子工程原型）、嵌入式专题技术文档。包含原理图、PCB、工程源码、项目设计文档、实物演示与调试复盘截图。
> 💡 **阅读建议**：推荐使用 VS Code 打开，搭配 Markdown 预览插件体验更佳。

**难度与推荐标识说明：**
* 👑 **重点标杆项目**：完整系统架构设计，复杂闭环控制、多任务通信，为本仓库代表性工程
* 🔴 **进阶项目**：贴近量产工程难点，侧重实时性、故障安全、复杂算法落地
* 🟡 **中等项目**：嵌入式固件高频技术模块，适合校招能力考察
* 🟢 **基础实践项目**：外设、通信协议基础验证，拓展技术广度

---

### 🏢 实习脱敏工程（全部脱敏，无企业涉密源码）

*🟡 ~ 🔴 工业嵌入式真实业务项目，包含状态机、音频DSP、总线通信、软硬件排错，配套完整技术文档沉淀*

**高新兴科技集团｜音频部**
* 📂 [电源时序器国产化替代](./internship/GAOXINXING/internship_project_01-power-sequencer) 🔴
  * 国产Cortex‑M0芯片迁移替代，非阻塞FSM八路电源时序控制、ADC矩阵按键、HLW8032电能计量、IWDG看门狗，2周独立完成固件适配并交付量产。
* 📂 [数字调音台‑反馈抑制器](./internship/GAOXINXING/internship_project_02-digital-audio-mixer) 🔴
  * 使用BP2668Ax评估开发板完成RISC‑V音频DSP Bring‑up，I2S音频数据流、COBS‑CRC自定义串口协议、移频反馈抑制算法预研，解决爆音、底噪、IO引脚冲突等软硬件耦合问题。
* 📂 [桌面USB声卡](./internship/GAOXINXING/internship_project_03-usb-audio-interface) 🟡
  * 基于BP2668Ax评估开发板开展方案设计；硬件未到位前提下完成上层业务框架开发，实现音频混音路由、消爆音处理、全局状态管理、分层软件架构设计。
*(注：每个实习目录内置项目方案、流程图、开发总结，严格遵守保密协议，无企业涉密源码)*
*(注：`./internship/ANOTECH` 为早期产品结构设计与 DFM 实习总结文档，非固件代码，用于建立硬件量产全局观)*

---

### 🚀 8套个人完整实战系统

*(注：每套系统目录结构统一：README简介、design详细设计、docs文档与图片、hardware原理图PCB、实物演示与排错复盘)*

#### 👑 核心控制与通信系统（仓库标杆）

**1. 无刷电机FOC控制系统** 👑
* 📂 [无刷电机FOC控制系统](./personal_project/01_foc-bldc-motor)
* 🛠️ `FreeRTOS · SVPWM · AS5600 · CAN`
* 📝 在无电流采样约束条件下，实现电压模式FOC多环控制；TIM1输出40kHz中心对齐PWM，完成Clarke/Park坐标变换与SVPWM调制。FreeRTOS拆分传感器、命令、遥测多任务，CAN总线支持多节点远程调速。

**2. STM32双轴运动控制平台（直流+步进）** 👑
* 📂 [双轴运动控制平台](./personal_project/02_dual-axis-motion-control)
* 🛠️ `STM32F103 · H桥驱动 · IR2104S · 霍尔编码器 · 增量式PID · S型加减速 · CAN · Modbus‑RTU`
* 📝 基于IR2104S+MOS实现H桥驱动与霍尔正交解码；直流轴增量式PID闭环调速，步进轴梯形/S型加减速抑制高速启停丢步。**实测指标**：0‑3000rpm宽域调速空载误差＜1%，突加负载200ms恢复稳态；双轴联动响应延迟 ≤10ms。

**3. 工业物联网边缘网关系统（MCU + Linux）** 👑
* 📂 [工业物联网边缘网关](./personal_project/03_industrial-edge-gateway)
  * *内含 `mcu_gateway` 与 `linux_edge_gateway` 两个子工程*
* 🛠️ `FreeRTOS · Modbus‑RTU · CAN · LLCC68‑LoRa · W5500 · MQTT · cJSON · RK3568(Linux) · Socket · SQLite`
* 📝 **MCU层**：RS485/LoRa/CAN多协议采集，协议转换MQTT；**Linux层**：RK3568部署MQTT Broker+SQLite本地缓存，TCP Socket与MCU终端通信，实现断线重连、数据缓存完整端云链路。

**4. 四轴无人机飞控系统** 👑
* 📂 [四轴无人机飞控系统](./personal_project/04_quadcopter-flight-controller)
* 🛠️ `STM32F4 · FreeRTOS · MPU6050 · 互补滤波 · 串级PID · PWM`
* 📝 移植FreeRTOS，拆分姿态解算、PID闭环、电机驱动、遥控接收独立任务；互补滤波融合六轴传感器，串级PID输出PWM驱动无刷电机；任务优先级调优，解决高速飞行机体剧烈震荡故障。

**5. 储能电池管理与安全充电系统（BMS+充电桩）** 🔴
* 📂 [储能BMS与智能充电桩](./personal_project/05_bms-and-charger)
  * *内含 `bms_system` 与 `smart_charger` 两大产品子工程；其中smart_charger包含 charger_bootloader(IAP引导)、charger_app(充电主程序)两套固件*
* 🛠️ `BQ76920 · I2C(CRC‑8) · CAN · IAP Bootloader · NXP‑RC522‑RFID · ECB02‑BLE · OLED`
* 📝 **BMS**：AFE芯片采集5串锂电电压、电流、NTC温度，配置过压/欠压/过流保护，OCV查表+温度补偿SOC估算；**充电桩**：自研IAP固件升级（CRC校验+断电回滚防变砖），RFID扣费、强电继电器安全控制逻辑。

#### 🔴 进阶｜传感器闭环与智能交互

**6. STM32冷热双循环温控系统** 🔴
* 📂 [冷热双循环高精度温控系统](./personal_project/06_stm32-tec-temperature-ctrl)
* 🛠️ `STM32L031 · TEC半导体制冷 · NTC · 复合数字滤波 · BLE · OLED · 编码器`
* 📝 针对TEC大热惯性、超调问题，PID闭环+多重数字滤波，**±0.25℃稳态精度**；非阻塞状态机管理制冷/加热/恒温多工况；本地OLED+编码器操控、BLE安卓APP、VOFA+上位机波形观测。

**7. ESP32‑S3智能视觉触控终端系统（多形态版）** 🟡
* 📂 [ESP32智能视觉终端](./personal_project/07_esp32-smart-terminals)
  * *内含 `touch_voice_terminal`、`vision_voice_terminal`、`video_doorbell` 三个子工程*
* 🛠️ `ESP‑IDF · FreeRTOS · LVGL · FT6336G触摸 · ILI9341V · I2S · ES8311 · OV2640/OV3660 · SDIO · WebSocket · MQTT · BLE · NVS · OTA`
* 📝 触控UI终端、便携视觉语音终端、可视门铃三类产品形态；基于FT6336G电容触摸+ILI9341V SPI彩屏实现LVGL触控交互；I2S+ES8311完成音频采集播放；环形缓冲区隔离音视频采集与网络传输，防止网络阻塞丢包；SDIO TF卡存储、ADC电池电量监测；多任务优化、BLE配网、NVS参数持久化、OTA在线升级完整业务。

#### 🟢 基础实践｜嵌入式基础与物联网综合实践

**8. 嵌入式基础与物联网综合实践** 🟢
* 📂 [物联网综合基础项目集](./personal_project/08_iot-basic-projects)
  * *内含 `offline_voice_speaker`、`smart_greenhouse`、`ecg_monitor` 三个子工程*
* 🛠️ `杰理Byle‑U8 SDK · A2DP · 离线KWS · AD按键 · 红外 · WS2812 · 阿里云MQTT · Android · AD8232 · ADC/DMA · EEPROM`
* 📝 离线语音蓝牙音箱（A2DP、离线KWS识别、红外按键多输入）；ESP32智能温室大棚（传感器采集、MQTT双向通信、Android APP远程管控、断线重连、EEPROM参数持久化）；心电采集监测仪（DMA高速ADC采样，三层软件架构，上位机波形绘制）。

---

### 📚 嵌入式专题技术文档 & 学习笔记

*🟡 ~ 🔴 聚焦量产嵌入式工程问题，伪代码+实测踩坑记录，持续迭代更新*

> 📎 [文档总览（共13篇专题笔记）](./docs/README.md)

* 📂 `./docs`
  * [《量产固件OTA方案：双分区+CRC+断电回滚》](./docs/firmware-mass-produce.md) 🔴：Bootloader、A/B分区OTA断电保护、看门狗、国产MCU迁移踩坑
  * [《非阻塞FSM设计：从阻塞Delay到Tick驱动》](./docs/realtime-fsm.md) 🔴：裸机&FreeRTOS有限状态机，状态死锁、标志位竞争排查
  * [《Modbus‑RTU + COBS/CRC协议栈设计与踩坑》](./docs/comm-protocol-driver.md) 🟡：环形缓冲区、Modbus、COBS帧封装，总线粘包超时实战
  * [《无电流采样FOC电压模式控制实践》](./docs/motor-foc-control.md) 👑：PMSM矢量变换、SVPWM、电机调参故障排查
* 📂 `./docs/tech-notes/`
  * 嵌入式C、STM32驱动、FreeRTOS实战、J‑Link/逻辑分析仪调试工具碎片踩坑笔记。

---

### 🗂️ 仓库目录总览

```text
├── README.md                          # 作品集首页（本文件）
├── docs/                              # 嵌入式专题技术文档与学习笔记
├── internship/                        # 实习项目（高新兴、安诺特）
│   ├── GAOXINXING/
│   │   ├── internship_project_01-power-sequencer      # 八路电源时序器
│   │   ├── internship_project_02-digital-audio-mixer  # RISC‑V 数字调音台
│   │   └── internship_project_03-usb-audio-interface  # USB 音频声卡
│   └── ANOTECH/                       # 安诺特实习（仅文档报告，无固件源码）
└── personal_project/                  # 8 套个人实战系统
    ├── 01_foc-bldc-motor              # 👑 FOC无刷电机控制
    ├── 02_dual-axis-motion-control    # 👑 双轴运动控制（直流+步进）
    ├── 03_industrial-edge-gateway     # 👑 工业物联网边缘网关（MCU+Linux）
    ├── 04_quadcopter-flight-controller# 👑 四轴无人机飞控系统
    ├── 05_bms-and-charger             # 🔴 储能 BMS 与智能充电桩
    ├── 06_stm32-tec-temperature-ctrl  # 🔴 冷热双循环高精度温控系统
    ├── 07_esp32-smart-terminals       # 🟡 ESP32 智能视觉终端
    └── 08_iot-basic-projects          # 🟢 IoT基础综合实践项目集

```
## ⚠️ 声明
1. 实习相关代码、资料均已做脱敏处理，不包含公司涉密业务源码、内部物料参数；
2. 文档为原始作品素材，请下载至本地阅读；
3. 个人项目文档与源码仅供学习交流；实习项目知识产权归属对应企业，禁止商用。