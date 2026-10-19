# 嵌入式调试工具实践笔记
> 📄 文档路径：docs/debug_tool_practice.md
>
> ✍️ 个人实践总结：MCU嵌入式开发调试工具链实战；J-Link/ST-Link、串口打印、断点调试、RT-Viewer、逻辑分析仪；GDB+OpenOCD远程调试；内存查看、栈检测、HardFault定位；项目调试手段与踩坑记录。
## 概述
嵌入式开发调试难度远高于PC程序，程序运行在独立硬件上，存在时序、寄存器、中断、内存异常等问题。合理组合调试工具，能够快速定位栈溢出、时序冲突、寄存器配置错误、偶发死机等难以复现的bug。
本文整理嵌入式常用调试工具、使用方法与适用场景，覆盖裸机、FreeRTOS项目。
## 覆盖能力
- ST-Link / J-Link下载与在线调试，SWD接口原理
- RT-Viewer实时变量查看，不占用串口带宽
- 串口printf调试、重定向与printf踩坑
- 逻辑分析仪：IO波形、SPI/I2C/UART总线抓取
- OpenOCD + GDB命令行调试，查看寄存器、内存、栈回溯
- 断点、观察点（watchpoint）使用场景
- HardFault定位方法：栈回溯、读取PC/LR寄存器，map文件定位代码行
- 栈水位检测、内存越界检测手段
- 偶现bug的调试策略，避免断点改变时序

## 一、SWD调试器 ST-Link / J-Link
SWD是Cortex-M系列标准调试接口，仅SWDIO+SWCLK两根线，支持下载、断点、寄存器读写。
- ST-Link：低成本，适合STM32；部分AT32也可兼容。
- J-Link：稳定性更强，支持更多MCU，RT-Viewer、实时栈监测功能。
> 注意：Bootloader场景下，需要保证调试接口没有被代码关闭。

### 基础功能
1. 断点breakpoint：代码行断点，适合逻辑bug。
2. 观察点watchpoint：**内存地址读写断点**，定位变量被意外篡改，排查内存越界神器。
3. 实时读取寄存器R0~R15、SP、PC、LR。

## 二、RT-Viewer（J-Link）
RT-Viewer可以实时输出变量/日志，**不阻塞程序，不占用串口**，适合高速实时调试。
原理：MCU开辟一块环形缓存，J-Link后台读取内存。
> 适用场景：电机FOC实时观测电流、转速、PID参数。
> 限制：必须J-Link，需要在代码中初始化RTT控制块。
```c
// RTT极简初始化
#include "SEGGER_RTT.h"
SEGGER_RTT_Init();
SEGGER_RTT_printf(0, "speed:%d\r\n", speed);
```

## 三、串口printf调试
最通用低成本调试方式，把日志重定向到串口。
```c
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
    return ch;
}
```
> 缺点：printf耗时，大量打印会改变时序；中断中禁止调用printf。

## 四、逻辑分析仪
抓取GPIO电平、串行总线（I2C、SPI、UART、PWM），用来排查硬件时序问题：
1. I2C无应答、从机地址错误、ACK异常
2. SPI时钟相位CPOL/CPHA配置错误
3. PWM波形、占空比、开关噪声
4. 外部中断触发时序异常
> 适用：区分是软件逻辑错误，还是硬件总线时序问题。

## 五、OpenOCD + GDB 命令行调试
脱离Keil图形界面，在Linux环境下远程调试Cortex-M芯片。
常用GDB命令：
```
info registers        # 查看所有寄存器
x/10x 0x20000000      # 查看内存
bt                    # 栈回溯
break main            # 设置断点
continue              # 继续运行
```
适合CMake跨平台项目自动化调试。

## 六、HardFault定位流程
1. 进入HardFault中断，保存SP、PC、LR寄存器。
2. 使用`.map`文件，根据PC地址反向查找对应的代码行。
3. 栈回溯查看函数调用链，定位崩溃现场。
> 常见崩溃诱因：野指针、数组越界、非对齐访问、栈溢出。

## 七、工程踩坑记录
1. 断点暂停程序后，外设定时器持续运行，时序被破坏，bug无法复现。
> 解决：偶发时序bug尽量不用断点，改用RTT/逻辑分析仪。
2. 中断内部调用printf，长时间阻塞中断，丢失中断事件。
> 解决：中断只标记标志，printf放到主循环。
3. SWD引脚被代码配置为普通GPIO，下载器无法连接。
> 解决：调试版本保留SWD引脚功能，量产版本关闭。
4. 栈溢出不会直接触发断点，运行一段时间随机死机。
> 解决：开启栈水位检测，观察剩余栈空间。
5. RTT缓冲区过小，日志丢包；缓冲区太大占用过多RAM。
> 解决：根据日志频率合理配置RTT buffer大小。
6. 逻辑分析仪采样率不足，高速SPI波形采样失真，误判问题。
> 解决：采样率至少5倍高于总线时钟。

## 八、适用项目场景
- AT32 USB-IAP Bootloader：使用GDB/ST-Link调试跳转、Flash读写
- STM32 FOC电机项目：RTT实时观测PID、电流、转速波形
- 电源时序器项目：逻辑分析仪抓取PWM与IO时序
- 冷热双循环温控项目：串口日志长期观测温度曲线

## 参考资料
- J-Link官方用户手册
- OpenOCD文档
- SEGGER RTT官方指南
- 《ARM Cortex-M调试实战》
## 📌 返回导航
[⬅ 返回作品集首页](../../README.md)