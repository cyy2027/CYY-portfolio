# 陈艺苑｜27 届 嵌入式软件工程师 作品集
## 📦 仓库内容说明
本仓库包含：**实习脱敏工程、4 套个人完整项目、嵌入式专题技术文档**，涵盖原理图、PCB、工程源码、项目说明文档、设计文档、实物截图、Office 文档。
💡 阅读建议：推荐使用 **VS Code** 打开本项目文件夹浏览，搭配 Markdown Preview 插件查看文档，代码高亮、目录跳转体验更佳。
> 难度标识说明：
> - ⭐⭐ 入门：基础外设、协议，适合嵌入式入门练习
> - ⭐⭐⭐ 中等：工业固件高频技术点，实习 / 校招面试高频考察
> - ⭐⭐⭐⭐ 进阶：量产难点，涉及实时性、故障安全、复杂算法

### 🧑‍💼 实习脱敏工程（全部脱敏，无企业涉密源码）
> ⭐⭐⭐ ~ ⭐⭐⭐⭐ 工业嵌入式真实业务项目，包含状态机、音频DSP、总线通信、软硬件排错、完整技术文档沉淀
1. 高新兴科技集团｜音频部
- [电源时序器国产化替代 MH2030A](./internship/GAOXINXING/internship_project_01-power-sequencer) ⭐⭐⭐
 国产RISC‑V芯片替代，非阻塞FSM八路电源时序控制、ADC按键、Modbus、看门狗整机交付
- [数字调音台 BP2668Ax RISC‑V](./internship/GAOXINXING/internship_project_02-digital-audio-mixer) ⭐⭐⭐⭐
 RISC‑V音频DSP Bring‑up，I2S音频数据流、COBS‑CRC自定义串口协议、反馈抑制算法预研
- [桌面USB声卡 BP26Ax](./internship/GAOXINXING/internship_project_03-usb-audio-interface) ⭐⭐⭐
 硬件未到货完成上层业务框架开发，音频混音路由、消爆音、全局状态管理、分层软件设计
> 每个实习目录内置：项目方案、流程图、开发总结。

2. 安诺特｜工程部项目开发实习
- [炊具产品建模与 DFM 评审实习](./internship/ANOTECH/README.md)
 家电硬件DFM评审、结构与电控协同，输出实习报告素材
> 实习目录内置：ANOTECH 目录同时存放实习报告 Markdown 网页版与原始 Word 报告、实习作品集截图、实习证明截图。

### 🚀 4 套个人完整项目
> 每套完整项目目录结构统一：`README.md`项目简介、`design.md`详细设计、`docs/`文档与图片素材、`docs/hardware/`原理图 PCB、元器件选型文档，包含实物演示截图、问题排错复盘。

1. [STM32 无刷电机 FOC 控制系统](./personal_project/personal_project_01_foc‑bldc‑moto) ⭐⭐⭐⭐
🏷 `FreeRTOS` `SVPWM` `FOC矢量控制` `CAN总线`
> ✨FreeRTOS实时框架，电压模式FOC矢量控制，AS5600磁编码器，CAN上位机交互，电机实时性调优

2. [STM32 冷热双循环温控系统](./personal_project/personal_project_02_stm32‑tec‑temperature‑ctrl) ⭐⭐⭐
🏷 `STM32/GD32` `复合滤波` `TEC双向精密温控` `PID` `Modbus-RTU` `BLE蓝牙`
> ✨TEC双向冷热闭环恒温系统，多路NTC采集+复合滤波解决ADC采样噪声问题；软件分层解耦，支持向国产GD32 PIN-to-PIN迁移；支持本地、蓝牙、上位机多交互，预留工业Modbus接口，用于元器件温漂自动化测试。

3. [ESP32 智能温室大棚系统](./personal_project/personal_project_03_esp32‑smart‑greenhouse) ⭐⭐
🏷 `ESP32` `MQTT` `阿里云物联网` `多传感器采集` `阈值自动控制`
> ✨物联网三层架构，多传感器环境采集；OLED本地显示，阿里云MQTT双向通信，APP远程管控；EEPROM阈值断电保存，网络断线自动重连

4. [STM32 智能充电桩](./personal_project/personal_project_04_stm32‑smart‑charger) ⭐⭐⭐
🏷 `IAP‑Bootloader` `RFID` `蓝牙透传`
> ✨IAP在应用编程实现免下载器现场升级，RFID身份鉴权，蓝牙参数配置交互

### 📖 嵌入式专题技术文档 ⭐⭐ ~ ⭐⭐⭐⭐
[docs](./docs)
> 四份工程实践专题文档，聚焦量产嵌入式真实工程问题，伪代码 + 实测踩坑记录，**不提供完整工程**
1. [《量产固件的 OTA 方案：双分区 + CRC + 断电回滚》](./docs/firmware‑mass‑produce.md) ⭐⭐⭐
2. [《非阻塞 FSM 设计：从阻塞 Delay 到 Tick 驱动》](./docs/realtime‑fsm.md) ⭐⭐⭐
3. [《Modbus‑RTU + COBS/CRC 协议栈设计与踩坑》](./docs/comm‑protocol‑driver.md) ⭐⭐
4. [《无电流采样下的 FOC 电压模式控制实践》](./docs/motor‑foc‑control.md) ⭐⭐⭐⭐
#### ✨拓展学习笔记：[docs/tech‑notes](./docs/tech-notes/README.md)，包含嵌入式C、STM32驱动、FreeRTOS开发踩坑总结。

## 📂 仓库目录总览
```/
├── README.md                         # 作品集首页（本文件）
├── docs/                             # 嵌入式专题技术文档（4 份 md）
│   ├── firmware‑mass‑produce.md      # OTA双分区升级
│   ├── realtime‑fsm.md               # 非阻塞FSM状态机
│   ├── comm‑protocol‑driver.md       # Modbus、COBS‑CRC协议
│   ├── motor‑foc‑control.md          # 电压模式FOC控制
│   └── tech‑notes/                   # ✨拓展学习笔记（无独立实物项目，知识总结）
│       ├── README.md                 # tech‑notes总目录
│       ├── c_basic_embedded.md       # C语言嵌入式开发易错点
│       ├── stm32_driver_tips.md      # STM3底层驱动、寄存器/库、HAL坑点
│       └── freertos_practice.md      # FreeRTOS实战踩坑、任务、信号量、队列
├── internship/                       # 实习项目
│   ├── GAOXINXING/                   # 高新兴科技｜音频嵌入式实习
│   │   ├── internship_project_01‑power‑sequencer        # 八路电源时序器，国产芯片替代方案开发
│   │   ├── internship_project_02‑digital‑audio‑mixer    # RISC‑V 数字调音台，音频芯片底层 Bring‑up 调试
│   │   └── internship_project_03‑usb‑audio‑interface    # USB 音频声卡，USB Audio 设备驱动与音质问题调优
│   └── ANOTECH/                      # 安诺特炊具实习
└── personal_project/                 # 4 个个人完整实战项目
   ├── personal_project_01_foc‑bldc‑moto               # STM32 无刷电机 FOC 控制系统
   ├── personal_project_02_stm32‑tec‑temperature‑ctrl  # STM32 冷热双循环温控系统
   ├── personal_project_03_esp32‑smart‑greenhouse      # ESP32 智能温室大棚系统
   └── personal_project_04_stm32‑smart‑charger         # STM32 智能充电桩


```

## ⚠️ 声明
```
1. 实习相关代码、资料均已做脱敏处理，**不包含公司涉密业务源码、内部物料参数**，仅保留通用技术方案、技术总结文档；
2. Office 文档（docx/pptx）为原始作品素材，GitHub 网页无法直接预览，请下载至本地打开阅读；
3. 编译产物（hex/axf/o/crf 等）已通过 gitignore 过滤，不会提交到仓库；
4. 个人项目文档与源码仅供学习交流；实习项目知识产权归属对应企业，禁止商用。
```