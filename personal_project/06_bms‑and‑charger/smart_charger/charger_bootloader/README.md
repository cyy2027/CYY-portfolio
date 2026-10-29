# 学习参考：SWM320智能充电桩课程(armsoc)
> 说明：本部分为课程要点笔记，对比个人STM32‑RFID充电桩项目，吸收软件架构、驱动设计思想，不直接照搬硬件实现。

## 📎课程原始链接
- **主课程**http://www.armsoc.cn/zxx/doc/charge/
1. http://www.armsoc.cn/zxx/doc/charge/1/index.html
2. http://www.armsoc.cn/zxx/doc/charge/2/index.html
3. http://www.armsoc.cn/zxx/doc/charge/3/index.html
4. http://www.armsoc.cn/zxx/doc/charge/4/index.html
5. http://www.armsoc.cn/zxx/doc/charge/5/index.html

- **扩展课程**
EX1 http://www.armsoc.cn/zxx/doc/charge/lib/install_apache.html
EX2 http://www.armsoc.cn/zxx/doc/charge/lib/rt_studio.html

- **工具 & 操作文档**
1. http://www.armsoc.cn/zxx/doc/charge/lib/recovery.html
2. http://www.armsoc.cn/zxx/doc/charge/pyocd.html

## 🧩芯片基础：SWM320RET7
- 内核：Cortex‑M4，主频 **110.592MHz**
- 存储：Flash 512KB，SRAM 128KB
- 引脚特性：支持 **FUN0 / FUN1** 两套外设功能复用，引脚灵活分配外设功能
- 注意点：外部晶振选型直接影响串口波特率计算精度，晶振不匹配会产生串口误码。

## 📚课程核心知识点
### 课程1｜裸机开发
1. **GPIO操作**：提供寄存器直接操作、SDK库函数两套编码方式；使用任意外设，**必须先开启外设时钟**，否则外设不工作。
2. **SysTick系统滴答定时器**：掌握**裸机非阻塞状态机编程**，摒弃`while(延时)`这种阻塞CPU的写法，提高CPU利用率。
3. **UART串口整套方案**
    - 硬件：FIFO接收 + 接收超时中断，降低中断触发频率；
    - 软件：`loopbuf`环形缓冲区实现数据缓存；
    - 业务：串口帧协议解析；简易Shell交互式命令行实现。

> 💡对比个人STM32‑RFID充电桩项目：
> 我的项目同样采用「主循环 + 中断」裸机架构，这套课程里**环形缓冲区、串口帧解析**思路，可以直接复用到我的充电桩串口/蓝牙业务。

### 课程2｜FreeRTOS分层驱动框架（⭐重点学习）
> 设计目标：硬件层与上层业务解耦，提高代码可移植、易维护，方便多人协同开发。

两层驱动划分：
- `drvp` 底层硬件端口层：直接操作寄存器、处理硬件中断，只负责硬件访问，不做业务逻辑；
- `drv` 上层驱动抽象层：实现软件缓冲区、回调函数接口，**不直接操作寄存器**，对接上层业务。

#### 关键技术收获
1. FreeRTOS基础配置：系统节拍、堆内存大小配置；使用**互斥量**解决多任务同时调用printf造成的打印乱序。
2. 可复用模板：GPIO / UART / RTC / WDT看门狗，均采用`drvp+drv`两层封装范式。
3. **FAL Flash分区管理**
    - 分区规划：`boot`引导区、`app`应用区、`conf`配置保存区、`fdb`数据库分区；
    - 业务能力：上电识别首次开机，实现配置参数掉电保存。
4. RTC时间处理：UTC日历时间 ↔ Unix时间戳相互转换。
5. Air780E 4G模组驱动：**状态机实现AT指令驱动框架**；增加超时检测、错误计数器、模块自动复位重连，提升网络稳定性。

## 🛠参考工具链接
1. shturl.cc/yiSB58jEQFZEbkvmDqoFGebL6yRpa47cp0oOs8yYoj
2. https://www.runoob.com/cprogramming/c-tutorial.html
    - 函数指针与回调：https://www.runoob.com/cprogramming/c-fun-pointer-callback.html
    - 命令行参数：https://www.runoob.com/cprogramming/c-command-line-arguments.html
