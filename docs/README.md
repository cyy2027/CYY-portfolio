# 嵌入式技术学习文档
> 技术学习文档覆盖固件可靠性、实时状态机、总线通信、电机FOC控制四大方向。
> 文档为工程实践总结，包含原理说明、伪代码示例、实测踩坑记录，不提供完整Keil工程。

## 📋 技术概览
- 👑 **电机矢量控制**：PMSM电压模式FOC、SVPWM、坐标变换、无电流采样实现方案、调参与故障排查
- 🔴 **量产固件可靠性**：Bootloader、A/B双分区OTA升级、CRC校验、断电回滚、看门狗故障处理、国产MCU迁移适配
- 🔴 **实时系统与状态机**：抛弃阻塞Delay，Tick驱动非阻塞FSM；裸机前后台 / FreeRTOS事件驱动调度
- 🟡 **通信协议与外设驱动**：UART环形缓冲区、RS485 Modbus‑RTU、COBS帧封装CRC校验；I2C/SPI硬件坑点总结

> 难度标识说明（与仓库首页保持一致）：
> *   👑 **重点系统项目**：完整系统架构设计，包含复杂控制闭环、多任务通信，为仓库代表性标杆工程
> *   🔴 **进阶项目**：贴近量产工程难点，侧重实时性、故障安全、复杂算法实现
> *   🟡 **中等项目**：覆盖嵌入式固件高频技术模块，适合校招能力考察
> *   🟢 **基础实践项目**：外设、协议基础验证，用于技术能力拓展

## 📚 技术文档导航（点击跳转）
1. [《量产固件的OTA方案：双分区+CRC+断电回滚》](./firmware‑mass‑produce.md) 🔴
> Bootloader设计、Flash分区、A/B分区OTA断电保护、看门狗、HardFault故障保存、国产MCU迁移踩坑

2. [《非阻塞FSM设计：从阻塞Delay到Tick驱动》](./realtime‑fsm.md) 🔴
> 消除HAL_Delay阻塞延时；裸机&FreeRTOS有限状态机三种实现；时间戳非阻塞延时；状态死锁、标志位竞争问题排查

3. [《Modbus‑RTU + COBS/CRC协议栈设计与踩坑》](./comm‑protocol‑driver.md) 🟡
> UART环形FIFO缓冲区、RS485硬件要点；Modbus‑RTU主从机；COBS无分隔符帧封装；总线乱码、粘包、超时处理实战

4. [《无电流采样下的FOC电压模式控制实践》](./motor‑foc‑control.md) 👑
> PMSM矢量控制、Clarke/Park坐标变换、SVPWM；无电流采样电压模式FOC实现；电机抖动、失步、启动故障调参记录

5. [《I2S/PCM音频开发实战：爆音、底噪、pop噪声定位与排查》](./audio-pop-noise-debug.md) 🔴
> I2S时序、DMA音频缓冲区；增益渐变消除pop爆音；中断抢占冲突；RISC‑V音频调试踩坑，移频反馈抑制工程要点

6. [《ESP‑IDF Ringbuf环形缓冲区实战》](./esp32-ringbuf-practice.md) 🟡
> 音视频生产者‑消费者模型，网络阻塞丢包处理，读写指针竞争问题，伪代码与踩坑记录

7. [《FreeRTOS工程踩坑集：栈溢出、优先级反转、临界区》](./freertos-trap-notes.md) 🔴
> 任务栈评估、栈溢出检测；互斥锁与信号量区分；优先级反转；临界区边界；RTOS API使用误区

8. [《Cortex‑M HardFault故障定位与调试手段》](./cortex-m-hardfault-debug.md) 🔴
> HardFault常见诱因；J‑Link寄存器回溯、map文件分析；栈转储；生产环境故障保存方案

9. [《RK3568边缘网关：Socket通信、断线重连、本地缓存设计》](./linux-edge-socket-cache.md) 🔴
> Linux‑MCU跨芯片TCP通信；粘包半包；断线重连策略；SQLite数据缓存，端云协同架构权衡

10. [《MQTT客户端实战：断连重连、遗嘱消息、QoS等级选型》](./mqtt-practice-note.md) 🟡
> MQTT QoS工程取舍、遗嘱消息；网络抖动场景；cJSON解析云端报文踩坑

11. [《ADC采集工程实践：偏移校准、滤波、强电噪声抑制》](./adc-engineering-practice.md) 🟡
> ADC增益偏移校准；数字滤波；强电环境噪声；DMA‑ADC多路采集策略

12. [《PID控制工程实战：温控/电机PID调参、超调抑制、前馈》](./pid-engineering-tuning.md) 🔴
> 增量式PID、串级PID；积分饱和抑制；大惯性系统调参；电机带载扰动处理，调参流程总结

13. [《CAN总线嵌入式开发：过滤器、错误状态、总线离线恢复》](./can-bus-practice.md) 🟡
> CAN过滤器配置；发送缓冲区满；错误计数；总线干扰、短路自动恢复，多节点组网踩坑

### ✨拓展学习笔记：[docs/tech‑notes](./tech‑notes/README.md)
包含嵌入式C、STM32驱动、FreeRTOS开发踩坑总结，碎片笔记示例：
- 嵌入式C编码陷阱：指针、volatile、位操作、内存对齐
- STM32外设踩坑：DMA、EXTI、时钟树配置常见bug
- FreeRTOS零散踩坑：队列读写、任务通知、任务休眠小案例
- 调试工具使用：J‑Link、逻辑分析仪、串口调试技巧

## 📌返回导航
[⬅ 返回作品集首页](../README.md)
