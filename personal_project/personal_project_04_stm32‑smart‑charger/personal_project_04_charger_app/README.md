# STM32 RC522 RFID 嵌入式自助充电桩控制器
基于 STM32F103C8T6 + RC522 RFID 实现自助充电桩控制器，支持Mifare卡片刷卡鉴权、余额充值扣费，提供**定时/定费双模式充电**；集成继电器功率控制、ADC电池参数采集、OLED本地显示、蓝牙上位机通信，适配无人值守自助充电场景。

## 个人贡献
- 搭建主控软件整体框架：外设初始化、前后台主循环，定时器与串口中断调度；
- 自研RC522驱动：实现寻卡、防冲突、扇区密码校验、Mifare卡片读写业务；
- 设计计费业务逻辑：实现定时充电、按金额计费算法，完成充值、扣费、卡片余额回写；
- ADC电压电流采集+软件滤波，继电器驱动与充电回路安全保护控制；
- 蓝牙透传模块开发：周期上报设备状态、解析上位机下发控制指令；
- 参与PCB硬件问题排查，完成软硬件联合整机调试。

## 技术栈
- 主控：STM32F103C8T6（HAL库）
- 外设：RC522 RFID、0.96寸I2C OLED、蓝牙透传模块、ADC采集、继电器、蜂鸣器
- 软件架构：前后台主循环 + 定时器/串口中断，业务采用状态机实现

## 项目目录结构
```
personal_project_04_charger_app/
├── README.md                    # 项目主页简介
├── design.md                    # 软硬件详细设计文档
├── doc/
│   ├── hardware/                # 硬件原始工程文件
│   │   ├── 原理图、pcb 文档版.pdf
│   │   ├── hardware_note.md     # 硬件设计笔记
│   │   ├── stm32‑smart‑charger.PcbDoc
│   │   ├── stm32‑smart‑charger.PrjPCB
│   │   └── stm32‑smart‑charger.SchDoc
│   ├── images/                  # 原理图、PCB、流程图、实物截图
│   │   ├── charge_module_pin.png
│   │   ├── hardware_demo.png
│   │   ├── mcu_pin.png
│   │   ├── pcb.png
│   │   ├── rc522_pin.png
│   │   ├── schematic.png
│   │   ├── software_flow1.png
│   │   ├── software_flow2.png
│   │   ├── software_flow3.png
│   │   └── system_arch.png
│   └── STM32 RC522 RFID 嵌入式自助充电桩控制器设计书.docx
└── keil_project/                # STM32CubeMX 生成 Keil 工程
    ├── Core/                    # 业务逻辑代码（RFID、计费、状态机）
    ├── Drivers/                 # 外设底层驱动
    ├── HAL/                     # STM32 HAL 库
    └── MDK-ARM/                 # Keil MDK 工程文件

```

## 系统概述
采用三层分层架构：交互层、控制层、执行感知层
1. **交互层**：OLED显示屏、按键、蓝牙上位机、蜂鸣器声光提示；
2. **控制层**：STM32负责业务调度、计时计费、上位机指令解析；
3. **执行感知层**：RFID卡片读写、电池电参采集、继电器充电回路控制。

业务流程：刷卡鉴权 → 卡片余额校验 → 启动充电（定时/定费）→ 实时采集电池参数 → 到达设定时间/金额阈值自动断电 → 再次刷卡完成扣费并回写卡片余额。

## 核心功能
- ✅ Mifare RFID卡片身份认证、余额读取、充值、扣费与数据写回
- ✅ 双充电模式：定时充电 / 按金额计费充电
- ✅ ADC实时采集电池电压、电流，OLED屏幕可视化展示
- ✅ 继电器硬件切断充电回路，超时自动断电安全保护
- ✅ 蓝牙通信：每2s上报整机状态；上位机可修改单价、切换充电模式
- ✅ 异常声光告警：无效卡片、余额不足等场景蜂鸣提示

## 实物与框图
| 系统架构 | 主控引脚 |
| ---- | ---- |
| <img src="./doc/images/system_arch.png" width="400"/> | <img src="./doc/images/mcu_pin.png" width="400"/> |

| RC522引脚 | 充放电模块引脚 |
| ---- | ---- |
| <img src="./doc/images/rc522_pin.png" width="400"/> | <img src="./doc/images/charge_module_pin.png" width="400"/> |

| 软件流程图1 | 软件流程图2 |
| ---- | ---- |
| <img src="./doc/images/software_flow1.png" width="400"/> | <img src="./doc/images/software_flow2.png" width="400"/> |

| 软件流程图3 | 硬件原理图 |
| ---- | ---- |
| <img src="./doc/images/software_flow3.png" width="400"/> | <img src="./doc/images/schematic.png" width="400"/> |

| PCB版图 | 实物照片 |
| ---- | ---- |
| <img src="./doc/images/pcb.png" width="400"/> | <img src="./doc/images/hardware_demo.png" width="400"/> |



## 核心代码片段说明
`src/`目录存放脱敏后的核心业务片段，**不提供完整Keil工程**，包含RFID读写、计费算法、中断调度框架。
完整业务逻辑描述见文档 `docs/system-design.md`。

## 调试踩坑总结
1. RC522射频天线远离继电器、功率电感，大功率器件会大幅衰减读卡距离；
2. ADC模拟采样增加多次滑动平均滤波，抑制电源引入的噪声干扰；
3. Mifare卡片写操作前必须完成扇区密码认证，写块失败会损坏卡片业务数据；
4. PCB布局时，功率继电器电路与信号电路做电源隔离，降低射频、ADC采样干扰。

## 🔮 后续迭代预研与技术储备方向（尚未落地实现）
- > 🧪 [SWM320充电桩](../personal_project_04_charger_bootloader) 
## 📌返回导航
[⬅ 返回作品集首页](../../../README.md)
