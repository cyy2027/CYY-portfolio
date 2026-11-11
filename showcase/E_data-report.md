# E · 数据与可靠性作品集

> 这一包回答一个问题：**有没有拿数据说话的习惯——估算、测量、复盘。**

## 覆盖岗位方向

BMS·储能 ｜ 充电桩·计费 ｜ 智能电表·计量 ｜ 计量校准 ｜ 可靠性 ｜ DQE ｜ ATE

## 证据清单

| 证据 | 位置 | 证明了什么 | 状态 |
|---|---|---|---|
| BMS 电池管理（06） | [06_bms‑and‑charger](../personal_project/06_bms‑and‑charger) | BQ76920 AFE 5 串采集、OCV 查表 + 温度补偿 SOC、过压/欠压/过流/过温软硬件双重保护 | ✅ 源码 + 文档 |
| 充电桩计费与安全升级（06） | [smart_charger](../personal_project/06_bms‑and‑charger/smart_charger) | RFID 刷卡扣费、IAP Bootloader + CRC 校验 + 断电回滚防变砖 | ✅ 源码 + 文档 |
| ±0.25 ℃ 温控实测曲线（07） | [docs/images](../personal_project/07_stm32‑tec‑temperature‑ctrl/docs/images) | VOFA+ 上位机 6 张实测曲线（0/11/30/41/42/50 ℃ 目标点） | ✅ 实测曲线 |
| 电源时序器功率采集与 Bug 复盘（实习） | [internship_project_01](../internship/GAOXINXING/internship_project_01-power-sequencer) | HL-388W2 交流电压/电流/功率采集、8 类软硬件耦合 Bug 闭环排查 | ✅ 脱敏文档 |
| SOC 充放电实测曲线 | `assets/data/`（规划） | SOC 估算与真实荷电状态的偏差 | ⏳ 待补 |
| 计量误差标定表 | `assets/data/`（规划） | 电压/电流/功率计量精度 | ⏳ 待补 |
| 可靠性测试报告（高低温/老化） | `assets/data/`（规划） | 系统级可靠性数据 | ⏳ 待补 |

## 关键数据

控温精度优于 ±0.25 ℃（0–52 ℃ 双向控温） ｜ OCV 查表 + 温度补偿 SOC ｜ 8 类量产软硬件耦合 Bug 复盘 ｜ IAP 断电回滚防变砖

## 局限与未验证

- **SOC 估算没有实际充放电曲线验证**，OCV 查表精度未标定。
- 计量维度只有功率采集的功能实现，没有误差标定表。
- 可靠性维度目前是设计手段（IWDG / IAP 回滚 / TVS），无系统测试报告。

## 30 秒阅读路径

`06 README` → `07 README`（看 VOFA 曲线）→ `实习① README`（Bug 复盘）

---

## 📌 返回导航

[⬅ 作品集首页](../README.md) ｜ [⬅ 五套作品集导航](../README.md#-五套作品集按岗位方向选一个看) ｜ [⬅ 60+ 岗位方向索引](../docs/career-path-index.md)

> 本页为**策展页**：只做筛选、链接与证据标注，项目内容一律在各目录原文，不在此复制。
