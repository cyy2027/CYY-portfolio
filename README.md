# 陈艺苑｜2027 届 嵌入式软件工程师 作品集
> 📦 **仓库内容说明**
> 本仓库包含：实习脱敏工程、**9套个人完整实战系统**（涵盖底层控制、工业通信、物联网终端，内含16个子工程原型）、嵌入式专题技术文档。包含原理图、PCB、工程源码、项目设计文档、实物演示与调试复盘截图。

> 💡 **阅读建议**：推荐使用 VS Code 打开，搭配 Markdown 预览插件体验更佳。

**难度与推荐标识说明：**
* 👑 **仓库头号标杆项目**：整套作品集代表性工程，复杂闭环控制、分布式架构、完整安全体系
* 🔥 **重磅硬核项目**：完整系统工程，贴近量产难点，实时性、闭环控制、多任务通信能力突出
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

*(注：[安诺特实习](./internship/ANOTECH) 为早期产品结构设计与 DFM 实习总结文档，非固件代码，用于建立硬件量产全局观)*

---

### 🚀 9套个人完整实战系统

*(注：每套系统目录结构统一：README简介、design详细设计、docs文档与图片、hardware原理图PCB、实物演示与排错复盘)*

#### 👑 仓库头号标杆项目

**1. HyperDog 四足机器人主控固件 👑**
* 📂 [HyperDog 四足机器人主控固件](./personal_project/01_hyperdog-quadruped)
* 🛠️ `Cortex‑M` `FreeRTOS` `SPI` `UART` `IMU` `CAN` `Trot步态` `五次多项式轨迹` `姿态互补滤波` `分级故障安全`
* 📝 对角Trot小跑步态调度，五次多项式足端轨迹，落地冲击小
* 📝 ICM20602 IMU姿态解算，动态滤波抑制电机振动噪声
* 📝 CAN分布式驱动关节伺服，1kHz周期下发角度指令
* 📝 分级故障保护、平滑收腿兜底，Tick驱动非阻塞状态机

#### 🔥 重磅硬核项目

**2. 北斗/GNSS 定位飞控系统 | 实时控制与空间定位** 🔥
* 📂 [北斗/GNSS 定位飞控系统 | 实时控制与空间定位](./personal_project/02_gnss-flight-controller)
* 🛠️ `STM32H723/F405` `FreeRTOS` `C/C++` `Qt5` `IMU/GNSS多传感器融合` `Dshot-600` `ELRS(CRSF)` `MSP OSD` `ESP8285 Wi-Fi`
* 📝 基于ICM-42688-P、气压计、M10北斗/GNSS多源传感器融合，搭建多级串级PID，实现姿态稳定、定高、位置定点控制
* 📝 实现Dshot-600高速数字电调驱动；ELRS CRSF远距离遥控遥测链路；移植IRC Tramp图传控制、MSP OSD画面数据叠加
* 📝 双芯片架构，ESP8285负责Wi‑Fi无线透传调参；Qt5开发桌面/安卓上位机，传感器可视化+参数一键下发
* 📝 打通传感器融合→串级PID→Dshot电调→无线遥测全链路，完成硬件驱动到上位机交互整套系统开发

**3. STM32 无刷电机 FOC 控制系统** 🔥
* 📂 [STM32 无刷电机 FOC 控制系统](./personal_project/03_foc‑bldc‑motor/)
* 🛠️ `FreeRTOS` `SVPWM` `电压模式FOC` `AS5600磁编码器` `CAN总线`
* 📝 FreeRTOS实时任务框架
* 📝 电压模式FOC矢量控制、SVPWM调制
* 📝 AS5600磁编码器位置采集
* 📝 CAN总线与上位机交互，电机实时性能调优

**4. STM32双轴运动控制平台（直流+步进）** 🔥
* 📂 [STM32双轴运动控制平台（直流+步进）](./personal_project/04_dual‑axis‑motion‑control/)
*   * *内含 `dc_reduce_motor`、`stepper_motory` 两个子工程*
* 🛠️ `STM32F103` `IR2104S` `H桥驱动` `霍尔编码器` `增量式PID` `S型加减速` `CAN` `Modbus‑RTU`
* 📝 H桥驱动直流电机、脉冲‑方向模式驱动步进电机，硬件过流保护、死区控制
* 📝 直流轴增量式PID闭环调速；步进电机梯形/S型加减速抑制启停丢步
* 📝 FreeRTOS划分运动控制、指令解析、状态上报多任务
* 📝 CAN、Modbus‑RTU接收上位机指令，支持双轴独立调速、正反转、多轴联动

**5. 工业物联网边缘网关系统（MCU + Linux）** 🔥
* 📂 [工业物联网边缘网关系统（MCU + Linux）](./personal_project/05_industrial‑edge‑gateway/)
  * *内含 `mcu_gateway`、`linux_edge_gateway` 两个子工程*
* 🛠️ `FreeRTOS` `Modbus‑RTU` `CAN` `LLCC68‑LoRa` `W5500` `MQTT` `cJSON` `RK3568(Linux)` `Socket` `SQLite`
* 📝 MCU端：RS485/LoRa/CAN多协议采集，完成协议转换转发MQTT
* 📝 Linux端：RK3568部署MQTT Broker、SQLite本地缓存
* 📝 TCP Socket完成MCU‑Linux通信，支持断线重连、本地数据缓存，实现完整端云链路

#### 🔴 进阶｜传感器闭环与智能交互

**6. 储能电池管理与安全充电系统（BMS+充电桩）** 🔴
* 📂 [储能电池管理与安全充电系统（BMS+充电桩）](./personal_project/06_bms‑and‑charger/)
  * *内含 `bms_system`、`smart_charger`两大子产品；smart_charger包含`charger_bootloader(IAP引导)`、`charger_app(充电主程序)`两套固件*
* 🛠️ `BQ76920` `I2C(CRC‑8)` `CAN` `IAP Bootloader` `NXP‑RC522‑RFID` `ECB02‑BLE` `OLED`
* 📝 BMS：BQ76920采集5串锂电电压、电流、NTC温度；过压/欠压/过流保护，OCV查表+温度补偿SOC估算
* 📝 充电桩：自研IAP固件升级（CRC校验+断电回滚防变砖）；RFID刷卡扣费；强电继电器安全控制逻辑

**7. STM32冷热双循环温控系统** 🔴
* 📂 [STM32冷热双循环温控系统](./personal_project/07_stm32‑tec‑temperature‑ctrl)
* 🛠️ `STM32L031` `TEC半导体制冷` `NTC` `复合数字滤波` `BLE` `OLED` `编码器`
* 📝 TEC半导体制冷片冷热双向驱动
* 📝 多路NTC测温，复合数字滤波抑制采样噪声，实现±0.25℃稳态控温精度
* 📝 本地OLED+编码器人机、BLE安卓APP、VOFA+上位机多端观测控制
* 📝 非阻塞FSM管理制冷/加热/恒温工况，面向工业长期无人值守场景

#### 🟡 中等｜人机交互与物联网终端

**8. ESP32‑S3智能视觉触控终端系统（多形态版）** 🟡
* 📂 [ESP32‑S3智能视觉触控终端系统（多形态版）](./personal_project/08_esp32‑smart‑terminals)
  * *内含 `touch_voice_terminal`、`vision_voice_terminal`、`video_doorbell`三个子工程*
* 🛠️ `ESP‑IDF` `FreeRTOS` `LVGL` `FT6336G触摸` `ILI9341V` `I2S` `ES8311` `OV2640/OV3660` `SDIO` `WebSocket` `MQTT` `BLE` `NVS` `OTA`
* 📝 FT6336G电容触摸+ILI9341V SPI彩屏，LVGL图形触控交互
* 📝 I2S+ES8311完成音频采集播放；环形缓冲区隔离音视频，避免网络阻塞丢包
* 📝 SDIO TF卡存储、ADC电池电量检测；BLE配网、NVS参数持久化、OTA在线升级完整业务

#### 🟢 基础实践｜物联网与消费电子综合实践

**9. 物联网与消费电子综合实践** 🟢
* 📂 [物联网与消费电子综合实践](./personal_project/09_iot‑basic‑projects)
  * *内含 `offline_voice_speaker`、`smart_greenhouse`、`ecg_monitor`三个子工程*
* 🛠️ `杰理Byle‑U8 SDK` `A2DP` `离线KWS` `AD按键` `红外` `WS2812` `阿里云MQTT` `Android` `AD8232` `ADC/DMA` `EEPROM` `生物信号` `智能穿戴`
* 📝 离线语音蓝牙音箱：A2DP蓝牙音频、离线KWS识别，按键/红外多输入控制彩灯
* 📝 ESP32智能温室大棚：多传感器采集，MQTT上云，Android APP远程管控，断线重连，EEPROM参数保存
* 📝 心电采集监测仪：ADC‑DMA采样AD8232心电信号；三层软件架构；串口上报波形至上位机

---

### 📚 嵌入式专题技术文档 & 学习笔记

*🟡 ~ 🔴 聚焦量产嵌入式工程问题，伪代码+实测踩坑记录，持续迭代更新*

> 📎 [文档总览（共20篇专题笔记）](./docs/README.md)

* 📂 `./docs`
  * [《量产固件OTA方案：双分区+CRC+断电回滚》](./docs/firmware‑mass‑produce.md) 🔴：Bootloader、A/B分区OTA断电保护、看门狗、国产MCU迁移踩坑
  * [《非阻塞FSM设计：从阻塞Delay到Tick驱动》](./docs/realtime‑fsm.md) 🔴：裸机&FreeRTOS有限状态机，状态死锁、标志位竞争排查
  * [《Modbus‑RTU + COBS/CRC协议栈设计与踩坑》](./docs/comm‑protocol‑driver.md) 🟡：环形缓冲区、Modbus、COBS帧封装，总线粘包超时实战
  * [《无电流采样FOC电压模式控制实践》](./docs/motor‑foc‑control.md) 🔥：PMSM矢量变换、SVPWM、电机调参故障排查
* 📂 [`./docs/tech-notes/`](./docs/tech-notes/)
  * 嵌入式C、STM32驱动、FreeRTOS实战、J‑Link/逻辑分析仪调试工具碎片踩坑笔记。

---

### 🗂️ 仓库目录总览

```text
├── README.md                          # 作品集首页（本文件）
├── docs/                              # 嵌入式专题技术文档与学习笔记
├── internship/                        # 实习项目（高新兴、安诺特）
│   ├── GAOXINXING/
│   │   ├── internship_project_01-power-sequencer      # 八路电源时序器
│   │   ├── internship_project_02-digital-audio-mixer  # RISC‑V 数字调音台
│   │   └── internship_project_03-usb-audio-interface  # USB 音频声卡
│   └── ANOTECH/                       # 安诺特实习（仅文档报告，无固件源码）
└── personal_project/                  # 9 套个人实战系统
    ├── 01_hyperdog-quadruped          # 👑 HyperDog 四足机器狗主控固件
    ├── 02_gnss-flight-controller      # 🔥 北斗GNSS定位飞控系统
    ├── 03_foc-bldc-motor              # 🔥 FOC无刷电机控制
    ├── 04_dual-axis-motion-control    # 🔥 双轴运动控制（直流+步进）
    ├── 05_industrial-edge-gateway     # 🔥 工业物联网边缘网关（MCU+Linux）
    ├── 06_bms-and-charger             # 🔴 储能 BMS 与智能充电桩
    ├── 07_stm32-tec-temperature-ctrl  # 🔴 冷热双循环高精度温控系统
    ├── 08_esp32-smart-terminals       # 🟡 ESP32 智能视觉终端
    └── 09_iot-basic-projects          # 🟢 物联网与消费电子综合实践
```
## ⚠️ 声明
1. 实习相关代码、资料均已做脱敏处理，不包含公司涉密业务源码、内部物料参数；
2. 文档为原始作品素材，请下载至本地阅读；
3. 个人项目文档与源码仅供学习交流；实习项目知识产权归属对应企业，禁止商用。