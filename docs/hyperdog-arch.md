# HyperDog四足机器狗：整机固件架构设计
📄 文档路径：docs/hyperdog-arch.md
> 标识：👑
> 配套项目：HyperDog四足机器狗主控固件
> 定位：分布式足式机器人固件顶层设计文档，面向嵌入式底层开发，**不涉及MPC步态优化算法**；聚焦实时调度、总线时序、故障安全、多节点协同。
> 包含：分层架构、任务划分、数据流、整机状态机、资源预算、时序约束、踩坑复盘。

## 1. 系统总体概述
HyperDog采用**主控板 + 四关节伺服板分布式CAN总线架构**。
- 主控板：STM32F4xx，整机决策、姿态解算、步态调度、遥控指令解析、安全逻辑、日志遥测；
- 关节伺服板（4块）：Cortex‑M0/M4，独立FOC伺服闭环，负责单关节角度/速度/力矩控制、电流采样、故障检测；
- 总线：CAN 2.0B，作为主控与关节唯一控制链路；
- 外设：ICM20602 IMU（躯干姿态）、无线遥控（ESP‑NOW / 串口PPM）、WiFi（上位机调参、波形遥测）；
- 固件框架：FreeRTOS，事件驱动，严格按闭环周期分配任务优先级，全程禁止阻塞延时。

> 设计思路：**关节闭环下沉到伺服板，主控只做轨迹规划、姿态修正、整机安全管理**，减轻主控算力压力，保证关节伺服闭环实时性。

## 2. 分层软件架构（自上而下）
```
【应用层】整机状态机、步态调度器、遥控指令解析、上位机交互、LVGL调试面板
【控制层】姿态融合解算、足端五次多项式轨迹生成、躯干姿态补偿
【协议层】CAN关节协议、遥控协议、遥测协议、参数持久化
【驱动层】IMU(I2C+DMA)、CAN、UART、SPI、PWM、Flash、看门狗
【底层板载库】CMSIS、HAL、FreeRTOS内核、硬件抽象
```
分层原则：
1. 驱动层与业务解耦，更换主控芯片仅修改驱动层；
2. 协议层独立，CAN帧格式修改不侵入步态、安全逻辑；
3. 控制层不直接操作硬件，只输出目标指令；
4. 所有延时统一使用Tick时间戳非阻塞调度，代码中无`HAL_Delay`。

## 3. FreeRTOS任务划分与优先级
> 优先级数字越大，优先级越高
| 任务名称 | 优先级 | 周期 | 核心职责 | 备注 |
| ---- | ---- | ---- | ---- | ---- |
| Joint_CAN_Tx | 6 | 1kHz | 向4个关节下发目标角度/力矩指令 | 硬实时，保障伺服闭环周期稳定 |
| IMU_Sample | 5 | 500Hz | IMU采样、四元数姿态解算、互补滤波 | 姿态为步态补偿的输入源 |
| Joint_CAN_Rx | 4 | 中断回调+队列 | 接收关节回传状态（角度、电流、故障码） | 中断快速入队，任务内解析，缩短中断耗时 |
| Gait_Scheduler | 3 | 200Hz | 步态相位推进、足端轨迹计算、姿态补偿修正 | Trot步态调度核心 |
| Remote_Cmd | 2 | 100Hz | 遥控指令解析、指令限幅、指令平滑滤波 | 抑制遥控突变冲击整机 |
| Telemetry_Log | 1 | 50Hz | 遥测数据打包、VOFA+波形上传、日志存储 | 低优先级，允许丢包，不能抢占控制链路 |
| IdleTask | 0 | — | 空闲钩子、看门狗喂狗、栈溢出检测 | 系统兜底 |

核心约束：
- 1kHz关节指令下发是系统最高时序约束，**CAN总线总负载必须预留充足余量**；
- 姿态解算频率高于步态调度频率，躯干姿态反馈更及时；
- 日志、上位机遥测优先级最低，网络、串口拥堵时丢包不影响控制闭环。

## 4. 整机数据流链路
两条主线：**指令前向链路**、**反馈闭环链路**
### 4.1 前向链路
遥控器/上位机指令 → 指令限幅+死区+平滑滤波 → 步态调度器（根据当前相位生成足端轨迹） → 逆运动学计算关节目标角度 → 封装CAN帧 → 下发至4块关节伺服板 → 关节FOC闭环执行。

### 4.2 反馈闭环链路
关节传感器（编码器、电流） → 伺服板FOC闭环 → 关节状态CAN回传 → 主控接收缓存；
IMU原始陀螺仪/加速度 → 互补滤波解算躯干欧拉角/四元数 → 姿态补偿模块 → 修正步态足端轨迹，抑制机体倾倒。

### 4.3 故障数据流
关节过流/过热/掉线、IMU漂移、姿态倾角超限、遥控失联 → 故障判定模块 → 分级故障响应（告警→限功率→紧急收腿趴下） → 写入Flash故障日志。

## 5. 整机状态机（核心FSM）
状态枚举：`STATE_IDLE`、`STATE_SELF_CHECK`、`STATE_STAND`、`STATE_TROT`、`STATE_FALL_PROTECT`、`STATE_FAULT_LOCK`
状态流转规则：
1. 上电 → `SELF_CHECK`：IMU自检、CAN节点扫描、关节通信检测、零位读取；自检失败直接进入`FAULT_LOCK`锁定；
2. 自检通过 → `IDLE`待机；
3. 收到站立指令 → `STAND`；收到小跑指令且处于站立状态 → `TROT`；
4. 任何状态下检测到致命故障（倾角过大、多关节掉线、遥控失联超时） → `FALL_PROTECT`，平滑收腿落地，再进入`FAULT_LOCK`锁定；
5. 故障锁定后，只有复位或者解锁指令才能回到待机；
6. 步态切换增加过渡缓冲，禁止目标角度阶跃突变，避免冲击震荡。

> 状态机实现：Tick驱动，状态转移函数，禁止在状态分支内写阻塞延时。

伪代码示例：
```c
// HyperDog整机状态机简易框架
typedef enum{
    STATE_IDLE,
    STATE_SELF_CHECK,
    STATE_STAND,
    STATE_TROT,
    STATE_FALL_PROTECT,
    STATE_FAULT_LOCK
}DogState_t;

DogState_t g_dog_state = STATE_IDLE;
void HyperDog_StateMachine(uint32_t tick)
{
    switch(g_dog_state)
    {
        case STATE_SELF_CHECK:
            Dog_SelfCheck();
            if(check_ok) g_dog_state = STATE_IDLE;
            else g_dog_state = STATE_FAULT_LOCK;
            break;
        case STATE_STAND:
            Dog_StandCtrl(tick);
            if(recv_trot_cmd) g_dog_state = STATE_TROT;
            if(IsDangerFault()) g_dog_state = STATE_FALL_PROTECT;
            break;
        case STATE_TROT:
            Dog_Gait_Trot(tick);
            if(recv_stop_cmd) g_dog_state = STATE_STAND;
            if(IsDangerFault()) g_dog_state = STATE_FALL_PROTECT;
            break;
        case STATE_FALL_PROTECT:
            Dog_SafeRetract();
            g_dog_state = STATE_FAULT_LOCK;
            break;
        default:
            break;
    }
}
```

## 6. 总线与时序资源预算
- CAN波特率：1Mbps；
- 主控下发：4帧/周期，每周期1ms，4条腿目标角度指令；
- 关节回传：4帧/2ms，包含角度、电流、温度、故障码；
- 总线带宽预留：控制帧占用带宽＜40%，预留余量应对干扰、重传；
- 时序红线：**关节指令下发抖动必须＜100us**，抖动过大伺服闭环容易震荡；
- RAM预算：队列缓冲区、姿态矩阵、轨迹缓存、CAN接收缓冲区提前评估，规避运行时堆/栈溢出。

## 7. 参数持久化
关节零位、滤波参数、步态周期、姿态补偿系数、故障阈值，存储在Flash分区；上电自动加载，支持遥控/上位机在线修改并保存。
> 注意：Flash写入加互斥锁，禁止高频反复擦写，延长Flash寿命。

## 8. 工程踩坑记录（面试高频）
1. IMU振动污染：电机PWM、减速器振动耦合进陀螺仪，固定参数互补滤波效果差；需要增加振动检测、滑动平均限幅，支撑相/摆动相切换滤波参数；
2. CAN总线抖动：中断优先级配置不当，高优先级中断抢占CAN发送，破坏1kHz下发周期，伺服出现抖动；
3. 步态相位失步：四个关节指令下发时序不一致，对角腿相位错位，机体横摆晃动；
4. 状态机雪崩：故障处理分支缺少互锁，多个故障连续触发，状态来回跳转；
5. 遥控指令突刺：摇杆抖动、丢包恢复瞬间指令跳变，不加平滑限幅直接引发摔机；
6. 内存溢出：步态轨迹缓存、CAN接收队列长度评估不足，长时间运行队列溢出丢帧。

## 9. 文档依赖
本架构依赖以下模块文档：
- [《HyperDog CAN分布式关节通信协议设计》](./hyperdog-can-protocol.md) 🔴
- [《HyperDog IMU姿态融合与电机振动抑制》](./hyperdog-imu-fusion.md) 🔴
- [《HyperDog Trot步态调度：非阻塞状态机与足端轨迹生成》](./hyperdog-gait-scheduler.md) 🔴
- [《HyperDog整机故障安全与分级保护机制》](./hyperdog-safety.md) 🔴
- [《HyperDog整机标定流程与分步调参指南》](./hyperdog-calibration.md) 🟡

## 📌 返回导航
[⬅ 返回嵌入式技术文档总览](./README.md)