# D · 硬件作品集

> 这一包回答一个问题：**懂不懂把原理图、PCB、功率电路和整机做出来。**

## 覆盖岗位方向

电机驱动·功率硬件 ｜ PCB Layout ｜ 消费电子 ｜ AE/FAE 现场支持

## 证据清单

| 证据 | 位置 | 证明了什么 | 状态 |
|---|---|---|---|
| TEC 温控整板（07） | [07_stm32‑tec‑temperature‑ctrl](../personal_project/07_stm32‑tec‑temperature‑ctrl) | 原理图 + **PCB 源文件（嘉立创 EDA .eprj）**、DCDC+双 LDO+π 型滤波电源树、TVS 防护 | ✅ 源文件 + 截图 |
| TEC 整板实物与电路截图 | [docs/images](../personal_project/07_stm32‑tec‑temperature‑ctrl/docs/images) | hardware_overview / pcb / schematic 等 12 张 | ✅ 实物照片 |
| 智能充电桩（06） | [smart_charger](../personal_project/06_bms‑and‑charger/smart_charger) | 原理图 / PCB / 系统架构等 11 张图、继电器强电控制、RC522 读卡 | ✅ 图 + 源码 |
| 双轴 H 桥驱动（04） | [04_dual‑axis‑motion‑control](../personal_project/04_dual‑axis‑motion‑control) | IR2104S + MOS 半桥、硬件过流检测、PWM 死区控制 | ✅ 文档 |
| 八路电源时序器（实习·量产） | [internship_project_01](../internship/GAOXINXING/internship_project_01-power-sequencer) | 八路继电器强电、IWDG 兜底、HL-388W 功率采集，已随产品量产 | ✅ 脱敏文档 |
| 温室硬件实物照（09） | [docs/images](../personal_project/09_iot‑basic‑projects/smart_greenhouse/docs/images) | hardware_demo 实物照片 | ✅ 照片 |
| FOC 驱动板（03） | [03_foc‑bldc‑motor](../personal_project/03_foc‑bldc‑motor) | SimpleFOC Mini 成品模块选型与功率链路说明 | ✅ 文档（成品模块，非自研板） |
| H 桥 / 相电压示波器波形 | `assets/wave/`（规划） | 功率电路的实际开关质量 | ⏳ 待补 |

## 关键数据

12 V 大电流 TEC 供电 ｜ DCDC 功率变换 + 双 LDO 低噪声 + π 型滤波 ｜ 八路继电器强电时序 ｜ IWDG 独立看门狗

## 局限与未验证

- 除 06/07/09 的图片外，**多数功率电路没有示波器波形入库存证**。
- 03 的驱动板为成品模块（SimpleFOC Mini），自研部分是算法与系统，不是功率板。
- 实习项目硬件图纸受保密限制不在仓库。

## 30 秒阅读路径

`07 README` → `07/hardware/hardware.md` → `06/smart_charger` → `实习① README`

---

## 📌 返回导航

[⬅ 作品集首页](../README.md) ｜ [⬅ 五套作品集导航](../README.md#-五套作品集按岗位方向选一个看) ｜ [⬅ 60+ 岗位方向索引](../docs/career-path-index.md)

> 本页为**策展页**：只做筛选、链接与证据标注，项目内容一律在各目录原文，不在此复制。
