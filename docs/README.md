# 嵌入式技术学习文档
> 技术学习文档覆盖固件可靠性、实时状态机、总线通信、电机FOC控制四大方向。
> 文档为工程实践总结，包含原理说明、伪代码示例、实测踩坑记录，不提供完整Keil工程。

## 📋 技术概览
- ⭐⭐⭐ **量产固件可靠性**：Bootloader、A/B双分区OTA升级、CRC校验、断电回滚、看门狗故障处理、国产MCU迁移适配
- ⭐⭐⭐ **实时系统与状态机**：抛弃阻塞Delay，Tick驱动非阻塞FSM；裸机前后台 / FreeRTOS事件驱动调度
- ⭐⭐ **通信协议与外设驱动**：UART环形缓冲区、RS485 Modbus‑RTU、COBS帧封装CRC校验；I2C/SPI硬件坑点总结
- ⭐⭐⭐⭐ **电机矢量控制**：PMSM电压模式FOC、SVPWM、坐标变换、无电流采样实现方案、调参与故障排查

> 难度标识说明：⭐⭐入门｜⭐⭐⭐中等｜⭐⭐⭐⭐进阶

## 📚 技术文档导航（点击跳转）
1. [《量产固件的OTA方案：双分区+CRC+断电回滚》](./firmware‑mass‑produce.md) ⭐⭐⭐
> Bootloader设计、Flash分区、A/B分区OTA断电保护、看门狗、HardFault故障保存、国产MCU迁移踩坑

2. [《非阻塞FSM设计：从阻塞Delay到Tick驱动》](./realtime‑fsm.md) ⭐⭐⭐
> 消除HAL_Delay阻塞延时；裸机&FreeRTOS有限状态机三种实现；时间戳非阻塞延时；状态死锁、标志位竞争问题排查

3. [《Modbus‑RTU + COBS/CRC协议栈设计与踩坑》](./comm‑protocol‑driver.md) ⭐⭐
> UART环形FIFO缓冲区、RS485硬件要点；Modbus‑RTU主从机；COBS无分隔符帧封装；总线乱码、粘包、超时处理实战

4. [《无电流采样下的FOC电压模式控制实践》](./motor‑foc‑control.md) ⭐⭐⭐⭐
> PMSM矢量控制、Clarke/Park坐标变换、SVPWM；无电流采样电压模式FOC实现；电机抖动、失步、启动故障调参记录

### ✨拓展学习笔记：[docs/tech-notes](./tech-notes/README.md)，包含嵌入式C、STM32驱动、FreeRTOS开发踩坑总结。

## 📌返回导航
[⬅ 返回作品集首页](../README.md)