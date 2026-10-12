# 🚀 个人嵌入式完整项目合集
> 说明：全部为独立完整练手项目，**源码可直接编译复现**。
> 📁 **统一目录规范**：
> 每个子项目固定包含：
> - `README.md`：项目快速简介、核心亮点、依赖环境
> - `design.md`：软硬件完整详细设计文档
> - `docs/`：实物截图、调试日志、踩坑复盘资料
> - `docs/hardware/`：原理图、PCB、BOM元器件选型文档

## 📂项目列表
1. [STM32 无刷电机 FOC 控制系统](./personal_project_01_foc‑bldc‑moto) ⭐⭐⭐⭐
> 🏷 `FreeRTOS` `SVPWM` `电压模式FOC` `AS5600磁编码器` `CAN总线`
- ✅ FreeRTOS实时任务框架
- ✅ 电压模式FOC矢量控制、SVPWM调制
- ✅ AS5600磁编码器位置采集
- ✅ CAN总线与上位机交互，电机实时性能调优

2. [STM32 冷热双循环温控系统](./personal_project_02_stm32‑tec‑temperature‑ctrl) ⭐⭐⭐
> 🏷 `STM32/GD32预研迁移` `TEC双向控温` `复合数字滤波` `BLE蓝牙` `Modbus‑RTU(预留)`
- ✅ TEC半导体制冷片冷热双向驱动，用于元器件温漂测试
- ✅ 多路NTC测温，复合数字滤波抑制电源与调试引入的采样噪声
- ✅ 本地人机 + BLE蓝牙APP + VOFA+上位机多端温控与波形观测
- ✅ 分层软件架构，可PIN‑to‑PIN迁移国产GD32；预研扩展分段PID、Modbus‑RTU、看门狗心跳，面向工业无人值守场景

1. [ESP32 智能温室大棚系统](./personal_project_03_esp32‑smart‑greenhouse) ⭐⭐
> 🏷 `ESP32` `阿里云MQTT` `多传感器采集` `自动阈值控制`
- ✅ 物联网三层架构，温湿度/土壤湿度/光照多维度环境采集
- ✅ OLED本地实时数据可视化
- ✅ EEPROM阈值参数断电保存
- ✅ 自动/手动双控制模式，WiFi‑MQTT断线自动重连，手机APP远程管控

1. [STM32 智能充电桩](./personal_project_04_stm32‑smart‑charger/) ⭐⭐⭐
> 🏷 `IAP‑Bootloader` `RFID` `蓝牙透传`
- ✅ IAP在应用编程，免下载器现场固件升级
- ✅ RFID刷卡身份鉴权
- ✅ 蓝牙串口透传，实现参数配置与状态交互
- 🧪 [SWM320充电桩](./personal_project_04_stm32‑smart‑charger/personal_project_04_charger_bootloader) **后续预研方案（尚未实现，技术储备方向）**

## 📌返回导航
[⬅ 返回作品集首页](../README.md)
