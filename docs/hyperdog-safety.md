# HyperDog四足机器狗：整机故障安全与分级保护机制
📄 文档路径：docs/hyperdog-safety.md
> 
> 标识：🔴
> 配套项目：HyperDog 四足机器狗主控固件
> 定位：整机安全子系统文档，定义分级故障判定、响应逻辑、故障日志；是整机状态机的安全兜底模块。
> 包含：故障分级、故障源、响应策略、遥控失联兜底、收腿保护、Flash 日志存储、踩坑复盘。

## 1. 模块概述
安全模块独立于步态调度、CAN通信模块，作为**系统最高优先级的决策逻辑**。
- 输入：关节状态字、IMU姿态、遥控心跳、供电电压、CAN总线状态；
- 输出：告警标记、步态锁止、急停广播、安全收腿序列、故障码；
- 设计原则：**分级响应，不一刀切；致命故障平滑保护，禁止瞬间硬锁关节**，避免腿直接锁死砸坏减速器；
- 调用时机：随步态调度任务200Hz同步检查故障标志，紧急故障帧可异步触发保护。

故障分级定义：
1. **LEVEL_INFO 提示告警**：不影响运行，仅记录日志，不干预步态；例：温度偏高、单次丢帧；
2. **LEVEL_WARN 警告**：限制性能，保留行走能力；例：单关节温度接近阈值、总线偶尔丢包；减小最大步长、降低速度上限；
3. **LEVEL_CRIT 严重故障**：停止步态，切换至站立保持；例：单关节掉线、单关节过流；不再允许小跑，仅维持站立；
4. **LEVEL_FATAL 致命故障**：触发倾倒保护，平滑收腿落地，进入故障锁死；例：姿态倾角超限、两个及以上关节掉线、遥控失联超时。

## 2. 故障源判定规则
| 故障来源 | 判定条件 | 故障等级 |
| ---- | ---- | ---- |
| 关节通信 | 单关节2ms无状态回传（心跳超时） | CRIT |
| 关节通信 | ≥2个关节同时超时 | FATAL |
| 关节内部 | 过流、堵转、编码器丢失 | CRIT |
| 关节内部 | 电机温度＞85℃ | WARN；＞100℃ | FATAL |
| IMU姿态 | Roll/Pitch ＞±25°告警；＞±30°致命倾倒阈值 | FATAL |
| 遥控链路 | 遥控心跳超时＞500ms | FATAL |
| CAN总线 | 总线关闭、持续大量错误计数 | FATAL |
| 供电 | 母线低压阈值触发 | CRIT |

> 防抖逻辑：故障信号连续N帧稳定触发才判定有效，避免单次干扰误触发保护。

## 3. 保护动作时序（重点）
### 3.1 警告级 WARN
- 置位告警标志；
- 日志写入Flash；
- 动态限速、限步长，步态继续运行；
- 向上位机/遥控推送告警信息。

### 3.2 严重级 CRIT
- 冻结步态相位，步长强制清零；
- 平滑过渡到站立姿态；
- 不允许进入Trot小跑；
- 保持关节使能，维持支撑；
- 持续上报故障码。

### 3.3 致命级 FATAL（倾倒保护流程）
1. 立刻停止步态相位推进，不再产生新的摆腿轨迹；
2. 启动安全收腿序列：五次多项式平滑收回四条腿，缓慢下放机身，**禁止关节瞬间锁死**；
3. 收腿完成后，下发广播脱机指令，关节进入零力矩模式；
4. 整机进入 `STATE_FAULT_LOCK`，锁定所有遥控指令；
5. 写入故障日志（故障类型、发生时刻、姿态、关节状态快照）；
6. 必须复位或者解锁指令，才能退出故障锁定。

> 核心工程要点：致命故障也必须平滑，直接锁死关节，落地冲击力会打坏减速器、编码器。

## 4. 故障日志存储方案
日志存储在Flash预留分区，环形循环存储：
- 一条日志包含：时间戳tick、故障等级、故障码、当时Roll/Pitch、4个关节状态；
- 写Flash加互斥锁，避免和参数保存冲突；
- 不频繁擦写：同一个故障100ms内只记录一次；
- 支持上位机读取日志，用于线下调试。

## 5. 安全模块伪代码
```c
// 故障分级判定入口，200Hz周期调用
void HyperDog_SafetyCheck(Attitude_t *att, JointStatus_t joint[], RemoteStatus_t *remote)
{
    uint8_t fault_level = LEVEL_INFO;

    // 姿态倾倒检测
    if(fabsf(att->roll) > DEG2RAD(30.0f) || fabsf(att->pitch) > DEG2RAD(30.0f))
    {
        fault_level = LEVEL_FATAL;
    }
    // 关节掉线统计
    uint8_t lose_cnt = 0;
    for(uint8_t i=0; i<4; i++)
    {
        if(joint[i].online == 0) lose_cnt++;
    }
    if(lose_cnt >= 2)
    {
        fault_level = LEVEL_FATAL;
    }
    else if(lose_cnt == 1)
    {
        fault_level = LEVEL_CRIT;
    }
    // 遥控失联
    if(remote->is_alive == 0)
    {
        fault_level = LEVEL_FATAL;
    }

    // 根据故障等级执行保护
    switch(fault_level)
    {
        case LEVEL_WARN:
            Gait_LimitSpeed();
            Fault_LogSave(fault_level);
            break;
        case LEVEL_CRIT:
            Gait_Freeze();
            HyperDog_SwitchToStand();
            Fault_LogSave(fault_level);
            break;
        case LEVEL_FATAL:
            HyperDog_SafeRetractSequence();
            g_dog_state = STATE_FAULT_LOCK;
            Fault_LogSave(fault_level);
            break;
        default:
            break;
    }
}
```

## 6. 互锁与隔离规则
1. 安全模块拥有最高权限，步态调度无法绕过安全标志；只要`fault_level >= CRIT`，步态调度直接不生成跨步轨迹；
2. 故障锁期间，遥控指令只识别复位解锁指令，其余摇杆指令全部丢弃；
3. CAN急停广播帧独立于周期发送，故障触发时优先推送；
4. 看门狗独立兜底：如果安全任务卡死，硬件看门狗复位整机。

## 7. 实测踩坑记录（面试高频）
1. 无防抖，干扰误触发倾倒保护：单次IMU尖峰、CAN单帧丢包直接判定致命故障；
   解决：连续多帧确认故障，增加故障防抖计数器；
2. 保护动作太粗暴：故障直接发送脱机指令，腿瞬间垮塌；
   解决：增加平滑收腿轨迹，和摆腿轨迹同源的五次多项式；
3. 故障状态自锁逻辑漏洞：故障消失后自动恢复小跑，容易二次摔机；
   解决：致命故障必须人工复位解锁，不自动恢复；
4. 日志频繁擦写Flash：故障高频上报，短时间耗尽Flash擦写寿命；
   解决：同故障限流，加时间窗口；
5. 多故障同时触发冲突：姿态超限+关节掉线同时到来，保护序列混乱；
   解决：故障优先级排序，倾倒保护优先级最高；
6. 遥控心跳判定不准：WiFi/ESP-NOW短暂卡顿直接判定失联；
   解决：心跳超时阈值根据链路特性调试，区分短暂抖动和断连。

## 8. 文档依赖
- 上层：hyperdog_arch.md（整机状态机流转）
- 数据源：hyperdog-imu-fusion.md（姿态）、hyperdog-can-protocol.md（关节状态）
- 联动：hyperdog-gait-scheduler.md（步态冻结、限速）

## 📌 返回导航
[⬅ 返回嵌入式技术文档总览](./README.md)
下一篇：`hyperdog-calibration.md`（整机标定流程与分步调参指南），继续吗？