# A · 动态控制作品集

> 这一包回答一个问题：**能不能在毫秒级实时域里，把姿态、轨迹和多路执行器同时控住。**

## 覆盖岗位方向

无人机·飞控 ｜ 具身智能·机器人 ｜ AGV·低速无人车 ｜ 清洁机器人 ｜ 运动控制（多轴·伺服）｜ 电机控制（FOC）｜ 变频家电·压缩机 ｜ 玩具·教育机器人

> 其中 AGV / 清洁机器人 / 变频家电 / 教育机器人四个方向没有专门项目，证据以可迁移技术为主（FOC、串级 PID、CAN/Modbus 总线、步态调度），面试时建议主动说明迁移逻辑。

## 证据清单

| 证据 | 位置 | 证明了什么 | 状态 |
|---|---|---|---|
| HyperDog 四足主控固件设计 | [hyperdog_rtos_control](../personal_project/01_quadruped_robotics/hyperdog_rtos_control) | 1 kHz CAN 下发 12 路关节指令、200 Hz 步态主循环、Trot 五次多项式足端轨迹、四级故障保护 | ✅ 设计文档 + 6 篇专题 |
| Nybble ROS 2 + micro-ROS 分层方案 | [nybble_micro_ros](../personal_project/01_quadruped_robotics/nybble_micro_ros) | 实时边界划在 MCU 侧的架构论证（XRCE-DDS 取舍） | ✅ 方案（无实机） |
| 北斗/GNSS 定位飞控 | [02_gnss-flight-controller](../personal_project/02_gnss-flight-controller) | 角度-角速度-高度-位置四级串级 PID、IMU+GNSS+气压计融合、Dshot-600、ELRS/CRSF、Qt5 上位机 | ✅ 文档 |
| FOC 无刷电机矢量控制 | [03_foc‑bldc‑motor](../personal_project/03_foc‑bldc‑motor) | SVPWM、Clarke/Park、AS5600 磁编码器、FreeRTOS 三层架构；**完整源码在仓库** | ✅ 源码 + 文档 |
| 双轴运动控制平台 | [04_dual‑axis‑motion‑control](../personal_project/04_dual‑axis‑motion‑control) | 0–3000 rpm 增量 PID 闭环、梯形/S 型加减速、CAN + Modbus-RTU 双总线 | ✅ 文档 |
| 具身智能平台预研（Rock 5B + Qmini） | [00_embodied_ai_platform](../personal_project/00_embodied_ai_platform) | 大脑—小脑异构架构、NPU 部署链路（规划阶段） | ✅ 方案（在研） |
| 悬停 / 抗风飞行视频 | `assets/video/`（规划） | 真机飞得起来 | ⏳ 待补 |
| 四足 Trot 行走视频 | `assets/video/`（规划） | 步态真的跑通 | ⏳ 待补 |
| FOC 转速阶跃响应波形 | `assets/wave/`（规划） | 响应时间与超调 | ⏳ 待补 |

## 关键数据

1 kHz 关节指令下发 ｜ 200 Hz 步态主循环 ｜ 0–3000 rpm 直流轴宽域调速 ｜ Dshot-600 高刷新数字电调协议 ｜ GNSS 定点悬停

## 局限与未验证

- **01 HyperDog**：设计与调参文档在库，固件源码与实机行走视频未纳入；Nybble 仅有架构设计，无实机、无代码。
- **02 飞控 / 04 双轴**：目录内目前只有文档，源码未入库；无实飞视频与定位精度测试记录，室外长航时与风场扰动未做系统标定。
- **03 FOC**：电压模式、无电流环（驱动板无相电流采样电路），低速带载性能受限；阶跃响应波形未入库存证。

## 30 秒阅读路径

`01 README` → `02 README` → `03 README` → [《PID 工程实战》](../docs/pid-engineering-tuning.md) → [《无感 FOC 观测器》](../docs/sensorless-foc-observer.md)

---

## 📌 返回导航

[⬅ 作品集首页](../README.md) ｜ [⬅ 五套作品集导航](../README.md#-五套作品集按岗位方向选一个看) ｜ [⬅ 60+ 岗位方向索引](../docs/career-path-index.md)

> 本页为**策展页**：只做筛选、链接与证据标注，项目内容一律在各目录原文，不在此复制。
