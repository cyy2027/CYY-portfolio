# FOC-BLDC 无刷电机矢量控制器 ⭐⭐⭐⭐
🏷 `STM32` `FreeRTOS` `FOC矢量控制` `SVPWM` `AS5600` `CMake` `ESP32`
> ✨亮点：基于正点原子STM32F103xE实现电压型FOC，三层模块化软件架构，算法与硬件解耦；支持FreeRTOS多任务+事件标志组；CMake+Keil MDK双工程；外接嘉立创ESP32+ESP8266无线协处理器，实现电机状态无线上传与远程调参。

## 🧩硬件平台
- 主控：正点原子 STM32F103xE
- 功率驱动：SimpleFOC Mini 驱动板
- 电机：2804外转子PMSM无刷电机
- 位置采集：AS5600磁编码器
- 无线协处理器：嘉立创 ESP32 + ESP8266模组
- 外设：TIM1输出40kHz中心对齐三相PWM；TIM3 500Hz定时中断作为FOC控制周期
- 通信：USART1本地串口调试；USART3预留语音桥接；串口对接ESP无线模组

## 📁目录结构
```
personal_project_01_foc-bldc-moto
├── algorithm/              # 算法层：一阶低通滤波、FOC矢量算法，硬件无关可移植
├── cmake/                  # CMake交叉编译配置（GCC/Clang双编译器支持）
├── Core/                   # 应用核心层：FreeRTOS任务、事件管理、中断、业务逻辑
├── doc/                    # 项目文档、硬件说明、实物与波形截图
├── Drivers/                # 底层外设驱动
├── MDK-ARM/                # Keil MDK备选工程
├── Middlewares/            # 可选中间件
├── .clang-format           # 代码格式化规范
├── .gitignore              # Git忽略配置
├── CMakeLists.txt          # CMake顶层构建脚本
├── design.md               # 完整系统设计文档
└── README.md               # 项目简介
```

## ✅核心功能
1. 电压模式FOC矢量控制 + SVPWM调制，速度环PID闭环
2. AS5600磁编码器角度采集，一阶低通滤波降噪，改善低速抖动
3. 电机状态机：停机→上电自检→开环启动→闭环FOC运行
4. FreeRTOS多任务框架，事件标志组实现任务解耦
5. 本地串口调试，支持启停、转速修改；ESP32协处理器实现远程调参、数据上传
6. 双构建系统：CMake跨平台编译 + Keil MDK本地调试

## 📄文档入口
- [完整系统设计文档](./design.md)
- [硬件清单与电路说明](./doc/hardware.md)

## 📌返回导航
[⬅ 返回作品集首页](../../README.md)