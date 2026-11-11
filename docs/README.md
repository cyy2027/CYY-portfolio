# 嵌入式技术学习文档
>  * 技术学习文档覆盖固件可靠性、实时状态机、总线通信、电机FOC控制、具身智能五大方向。
>  * 文档为工程实践总结，包含原理说明、伪代码示例、实测踩坑记录，不提供完整Keil工程。

## 📋 技术概览
- 🔴 **电机矢量控制**：PMSM电压模式FOC、SVPWM、坐标变换、无电流采样实现方案、无感观测器（SMO / PLL / 高频注入）、调参与故障排查
- 🔴 **量产固件可靠性**：Bootloader、A/B双分区OTA升级、CRC校验、断电回滚、看门狗故障处理、国产MCU迁移适配
- 🔴 **实时系统与状态机**：抛弃阻塞Delay，Tick驱动非阻塞FSM；裸机前后台 / FreeRTOS事件驱动调度
- 🟡 **通信协议与外设驱动**：UART环形缓冲区、RS485 Modbus‑RTU、COBS帧封装CRC校验；I2C/SPI硬件坑点总结
- 🔴 **具身智能系统**：大脑—小脑异构架构与延迟预算、RK3588 NPU 策略部署、Sim-to-Real 迁移、四足整机实时控制（Trot 步态 / CAN 关节协议 / IMU 融合 / 故障安全 / 标定）、ROS 2 + micro-ROS 分层链路

> **本目录的 🔴 / 🟡 / 🟢 只表示文档阅读难度**，与 [仓库首页](../README.md) 项目条目的项目等级不是同一套东西：
>
> | 标识 | 本目录含义 |
> |---|---|
> | 🔴 | 偏系统级：涉及量产可靠性、故障安全、实时性权衡，读之前最好有对应项目经验 |
> | 🟡 | 偏模块级：单一外设或协议，可独立阅读 |
> | 🟢 | 入门备忘：查漏补缺用 |

## 📚 技术文档导航（点击跳转）
1. [《量产固件的OTA方案：双分区+CRC+断电回滚》](./firmware‑mass‑produce.md) 🔴
> Bootloader设计、Flash分区、A/B分区OTA断电保护、看门狗、HardFault故障保存、国产MCU迁移踩坑

2. [《非阻塞FSM设计：从阻塞Delay到Tick驱动》](./realtime‑fsm.md) 🔴
> 消除HAL_Delay阻塞延时；裸机&FreeRTOS有限状态机三种实现；时间戳非阻塞延时；状态死锁、标志位竞争问题排查

3. [《Modbus‑RTU + COBS/CRC协议栈设计与踩坑》](./comm‑protocol‑driver.md) 🟡
> UART环形FIFO缓冲区、RS485硬件要点；Modbus‑RTU主从机；COBS无分隔符帧封装；总线乱码、粘包、超时处理实战

4. [《无电流采样下的FOC电压模式控制实践》](./motor‑foc‑control.md) 🔴
> PMSM矢量控制、Clarke/Park坐标变换、SVPWM；无电流采样电压模式FOC实现；电机抖动、失步、启动故障调参记录

5. [《无感FOC转子位置观测器：滑模观测、PLL提取与低速高频注入》](./sensorless-foc-observer.md) 🔴
> αβ电压方程与反电动势可观测性；滑模观测器设计与抖振抑制、低通相位补偿；PLL角度提取；SMO/龙伯格/磁链积分/EEMF/HFI 五类方案取舍；低速盲区量级估算；I-F强拖与切换判据；离散化稳定性与死区补偿

6. [《I2S/PCM音频开发实战：爆音、底噪、pop噪声定位与排查》](./audio-pop-noise-debug.md) 🔴
> I2S时序、DMA音频缓冲区；增益渐变消除pop爆音；中断抢占冲突；RISC‑V音频调试踩坑，移频反馈抑制工程要点

7. [《ESP‑IDF Ringbuf环形缓冲区实战》](./esp32-ringbuf-practice.md) 🟡
> 音视频生产者‑消费者模型，网络阻塞丢包处理，读写指针竞争问题，伪代码与踩坑记录

8. [《FreeRTOS工程踩坑集：栈溢出、优先级反转、临界区》](./freertos-trap-notes.md) 🔴
> 任务栈评估、栈溢出检测；互斥锁与信号量区分；优先级反转；临界区边界；RTOS API使用误区

9. [《Cortex‑M HardFault故障定位与调试手段》](./cortex-m-hardfault-debug.md) 🔴
> HardFault常见诱因；J‑Link寄存器回溯、map文件分析；栈转储；生产环境故障保存方案

10. [《RK3568边缘网关：Socket通信、断线重连、本地缓存设计》](./linux-edge-socket-cache.md) 🔴
> Linux‑MCU跨芯片TCP通信；粘包半包；断线重连策略；SQLite数据缓存，端云协同架构权衡

11. [《MQTT客户端实战：断连重连、遗嘱消息、QoS等级选型》](./mqtt-practice-note.md) 🟡
> MQTT QoS工程取舍、遗嘱消息；网络抖动场景；cJSON解析云端报文踩坑

12. [《ADC采集工程实践：偏移校准、滤波、强电噪声抑制》](./adc-engineering-practice.md) 🟡
> ADC增益偏移校准；数字滤波；强电环境噪声；DMA‑ADC多路采集策略

13. [《PID控制工程实战：温控/电机PID调参、超调抑制、前馈》](./pid-engineering-tuning.md) 🔴
> 增量式PID、串级PID；积分饱和抑制；大惯性系统调参；电机带载扰动处理，调参流程总结

14. [《CAN总线嵌入式开发：过滤器、错误状态、总线离线恢复》](./can-bus-practice.md) 🟡
> CAN过滤器配置；发送缓冲区满；错误计数；总线干扰、短路自动恢复，多节点组网踩坑

15. [《LVGL嵌入式GUI实战：内存管理、页面切换、帧率优化、内存泄漏排查》](./lvgl-gui-practice.md) 🔴
> LVGL内存池、动态控件销毁防泄漏；页面切换动画、屏幕刷新撕裂；RTOS任务调度冲突；低资源MCU裁剪优化、触摸屏输入事件踩坑

**具身智能 · 异构平台与策略部署（Rock 5B + Qmini，00 平台，规划中）**

16. [《大脑—小脑异构架构：50Hz决策与20kHz电流环之间》](./embodied-brain-body-arch.md) 🔴
> 四层时间尺度错配；延迟预算分段（采集/估计/推理/传输/插值）与p99口径；安全边界为何必须在实时域闭合；五次多项式与临界阻尼插值；延迟打点测量法；Rock 5B + Qmini 映射

17. [《RK3588 NPU部署链路：从ONNX到策略上板》](./rk3588-npu-deploy.md) 🔴
> ONNX导出检查项；INT8量化与校准集设计、逐层误差分析；算子回落判定；板上推理循环与线程绑核、调频锁定、零拷贝；时延分布测量

18. [《Sim-to-Real：从Isaac Gym训练到Qmini真机落地》](./sim-to-real-legged.md) 🔴
> 观测/动作空间定义；奖励设计与关节加速度惩罚（防真机高频抖）；域随机化九项含控制延迟；PD参数桥接；迁移成功的三个量化指标

19. [《Qmini + Rock 5B上板路线：从开箱到策略闭环》](./qmini-rock5b-bringup.md) 🟡
> 采购清单与成本结构；三项采购前必确认（电机协议/SDK维护/供电）；P0-P4分阶段验收口与失败退路；风险表；与已有项目的模块衔接

**具身智能 · 四足整机实时控制（HyperDog 单机 MCU + CAN，01 项目 🐕 路线）**

20. [《HyperDog四足机器狗：整机固件架构设计》](./hyperdog-arch.md) 🔴
> FreeRTOS任务划分与时序节拍、中断优先级分配、整机状态机、资源预算；1kHz关节指令下发 + 200Hz步态主循环的分层解耦

21. [《HyperDog CAN分布式关节通信协议设计》](./hyperdog-can-protocol.md) 🔴
> CAN 2.0B报文ID分配、数据帧格式、CRC校验、丢包重传容错、多节点同步策略、总线保护

22. [《HyperDog IMU姿态融合与电机振动抑制》](./hyperdog-imu-fusion.md) 🔴
> IMU原始数据预处理、振动自适应互补滤波、冲击时段权重动态调整，解决行走振动导致的姿态发散

23. [《HyperDog Trot步态调度：非阻塞状态机与足端轨迹生成》](./hyperdog-gait-scheduler.md) 🔴
> 对角步态相位时序、五次多项式足端轨迹（起止速度/加速度为0）、机身横滚/俯仰姿态补偿、步态启停平滑过渡

24. [《HyperDog整机故障安全与分级保护机制》](./hyperdog-safety.md) 🔴
> INFO/WARN/CRIT/FATAL四级故障判定；致命故障不直接锁死电机，执行平滑收腿停机序列；故障日志存储与上报

25. [《HyperDog整机标定流程与分步调参指南》](./hyperdog-calibration.md) 🟡
> IMU零偏校准、关节机械零位标定、连杆几何参数整定、参数Flash存储+CRC完整性校验；从小到大的分步调参顺序

**具身智能 · ROS 2 + micro-ROS 分层（Nybble 链路分层，01 项目 🐈 路线）**

26. [《ROS 2 + micro-ROS分层控制：把实时边界挪到链路上》](./ros2-micro-ros-layered.md) 🟡
> XRCE-DDS Client-Agent模型与代价；实时边界退化的三层后果；DMA+环形缓冲+队列三段式接收；时间戳与匀速插值、欠载策略；控制流QoS选型；何时退回单机方案

### ✨拓展学习笔记：[docs/tech‑notes](./tech-notes/README.md)
包含嵌入式C、STM32驱动、FreeRTOS开发踩坑总结，碎片笔记示例：
- 嵌入式C编码陷阱：指针、volatile、位操作、内存对齐
- STM32外设踩坑：DMA、EXTI、时钟树配置常见bug
- FreeRTOS零散踩坑：队列读写、任务通知、任务休眠小案例
- 调试工具使用：J‑Link、逻辑分析仪、串口调试技巧

## 📌返回导航
[⬅ 返回作品集首页](../README.md)
