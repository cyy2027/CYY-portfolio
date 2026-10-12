# 高新兴科技集团｜音频部实习项目
> 实习岗位：嵌入式软件助理工程师
> ⚠️全部资料已脱敏，**无企业涉密源码**，仅保留项目方案、流程图、开发总结文档。

## 📂项目列表
### 1. 电源时序器国产化替代 MH2030A ⭐⭐⭐
[internship_project_01‑power‑sequencer](./GAOXINXING/internship_project_01-power-sequencer)
🏷 `RISC‑V` `非阻塞FSM` `Modbus‑RTU` `Flash掉电保存`
- ✅已完成：MH2030A国产时序芯片驱动开发，实现对进口电源时序器件国产化替代
- ✅已完成：八路电源通道时序上电/逆序下电、电压监控、故障检测逻辑
- ✅已完成：ADC按键、段码屏、功率计量、看门狗、Flash参数保存；整机联调测试，交付可用固件

### 2. 数字调音台‑反馈抑制器 BP2668Ax RISC‑V ⭐⭐⭐
[internship_project_02‑digital‑audio‑mixer](./GAOXINXING/internship_project_02-digital-audio-mixer)
🏷 `RISC‑V DSP` `I2S音频` `COBS‑CRC` `串口协议`
- ✅已完成：音频DSP底层Bring‑up，SDK Demo调试，熟悉I2S音频数据流与音频路由API
- ✅已完成：研读对接M20‑DSPAPI自定义COBS‑CRC串口协议，梳理指令下发、ACK应答、周期上报完整流程
- ✅已完成：定位UART0被I2S抢占无法打印问题，搭建JTAG/GPIO替代调试方案，整理烧录编译踩坑笔记与开发Checklist
- ⏳待硬件底板到位：FBX陷波、移频反馈抑制声学闭环实物验证

### 3. 桌面USB声卡 BP2668Ax ⭐⭐⭐
[internship_project_03‑usb‑audio‑interface](./GAOXINXING/internship_project_03-usb-audio-interface)
🏷 `音频混音` `状态管理` `增益渐变消爆音` `空桩分层开发`
- ✅已完成：输出完整落地方案文档；硬件未到货前提下完成全部上层业务软件框架
- ✅已完成：长短按按键状态机、音频混音路由、增益平滑渐变消除pop爆音；`SystemState_t`全局单一数据源状态管理
- ✅已完成：屏幕、继电器模块业务逻辑，预留算法接入接口，解决IDE编译工程问题，工程可正常编译
- ⏳待外设硬件到货：Codec、USB音频芯片、SPI彩屏底层驱动填充与整机联调

---

# 安诺特｜工程部项目开发实习
> 实习方向：家电结构建模、硬件DFM可制造性评审，输出完整实习报告材料

- [炊具产品建模与 DFM 评审实习](./ANOTECH/README.md)
> 📁目录包含：Markdown版实习报告、原始Word报告、作品集截图、实习证明截图。

## 📌返回导航
[⬅ 返回作品集首页](../README.md)
