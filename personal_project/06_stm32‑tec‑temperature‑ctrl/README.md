# STM32冷热双循环控制系统
🏷 `STM32` `TEC半导体制冷` `双向温控` `PID` `BLE蓝牙` `VOFA+`
> 基于STM32实现TEC半导体制冷片双向冷热恒温控制，可实现腔体制冷、加热，支持本地编码器操作、安卓APP控制、VOFA+PC上位机波形观测，面向元器件温漂测试场景，具备多路温度采集与数字滤波降噪。
## 🧩硬件平台
- 主控：STM32L031G6UX MCU
- 执行器件：TEC半导体制冷片，支持制冷/加热双向切换
- 温度采集：多路NTC热敏电阻，采集**腔体温度、散热片温度、环境温度**
- 人机交互：0.96寸OLED显示屏、EC11旋转编码器、状态指示灯（制冷/加热/恒温稳定指示）
- 无线通信：BLE蓝牙模块，对接安卓APP
- 功率电源：12V大电流直流供电；板载DCDC功率变换、双LDO低噪声电源、π型滤波电路抑制开关电源噪声
- 机械结构：CNC加工隔热恒温腔体，配套散热风扇组件
## ✨核心功能
1. **双向恒温控制**：TEC半导体制冷片，支持制冷、加热，控温区间 0‑52℃；控制精度优于 ±0.25℃
2. 多路温度监测：同时采集腔体内部、散热片、环境温度，监控热耦合状态
3. 双重数字滤波：滑动平均滤波+复合滤波算法，解决拔掉ST‑LINK后温度采样噪声飙升问题
4. 两种控制操作方式
  - 本地：旋转编码器设定目标温度，OLED实时显示电压、功率、各路温度；LED指示制冷/加热/恒温完成状态
  - 蓝牙：安卓APP下发设定温度、启停控制，实时查看温度波形；支持VOFA+上位机绘制温控曲线
5. 保护逻辑：硬件TVS过压防护；限制最高温度防止TEC器件烧毁；恒温稳定状态上报提示。

## 📁目录结构
```

personal_project_02_gd32-tec-temperature-ctrl
├── README.md                # 项目首页简介、亮点、标签
├── design.md                # 完整系统设计文档（软件架构、算法、测试复盘、后续优化）
├── hardware/                # 硬件相关文件夹
│   ├── hardware.md          # 硬件详细说明、器件清单、PCB / 电源 / 机械设计要点
│   ├── pcb/                 # PCB 源文件
│   ├── mechanical/          # 3D 机械结构 STEP 模型（CNC 腔体、磁吸钣金）
│   └── datasheet/           # 器件规格书、BOM表、功率核算表、采购报价、蓝牙调试文档
├── keil_project/            # Keil MDK‑ARM 原版 STM32 工程
│   ├── OLED.ioc             # STM32CubeMX 配置文件
│   ├── Core/
│   ├── Drivers/
│   └── MDK‑ARM/
└── docs/                    # 测试文档与素材
├── images/              # 实物照片、VOFA 波形截图、APP 界面
├── user_guide.md        # 用户硬件使用说明、安全警告、参数
├── upper_manual.md     # 上位机使用手册
└── test_report.md       # 测试数据、阶跃曲线记录

```
## 🧾源码工程说明
源码目录：`keil_project/`
- `OLED.ioc`：STM32CubeMX工程配置文件，可直接打开查看引脚分配、时钟树、外设配置；配套`.mxproject`为CubeMX工程缓存文件。
- `Core/`：**逻辑三层分层代码**，在原生Core目录内按职责划分模块，不新增独立子文件夹，实现算法与硬件驱动解耦，便于移植国产MCU：
  - 驱动模块：`adc.c/gpio.c/i2c.c/tim.c/usart.c`，CubeMX自动生成外设底层驱动，负责硬件读写；
  - 算法模块：`pid.c / filter.c`，纯数学运算，无硬件依赖，实现PID闭环、复合数字滤波；
  - 应用业务模块：`temp_app.c`，温控状态机、蓝牙指令解析、恒温判断、数据上报业务逻辑；
  - `main.c`：程序入口，负责系统初始化与主循环调度。
- `Drivers/`：HAL库底层驱动文件，不修改库源码。
- `MDK‑ARM/`：Keil MDK工程，可直接打开编译。
> ⚠️ 仓库已通过gitignore过滤Debug/Release编译输出文件（axf、hex、map、目标文件等），下载工程后需要重新编译生成固件。
> 📌 预研规划：当前工程为STM32版本，后续计划PIN‑to‑PIN迁移至国产GD32F103RCT6，硬件无需改动，仅适配底层库与时钟配置；增加模糊PID、环形日志缓存、参数掉电存储功能。
## 📦机械与硬件源文件
- 3D结构模型存放于 `hardware/mechanical/`，包含CNC腔体与磁吸钣金盖STEP模型，可使用SolidWorks/FreeCAD打开查看腔体隔热结构。
- PCB与原理图PDF存放在`hardware/schematic/`、`hardware/pcb/`目录。
> 原始大型工程压缩包（恒温器.rar、蓝牙串口PCB.rar）因体积较大未存入Git仓库，本地留存，面试可现场打开演示。
## 📄文档入口
- [用户使用说明](./docs/user_guide.md)
- [上位机操作手册](./docs/upper_manual.md)
- [整机测试报告](./docs/test_report.md)
- [详细软硬件设计文档](./design.md)

## 📌返回导航
[⬅ 返回作品集首页](../../README.md)