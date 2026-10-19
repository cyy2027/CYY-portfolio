# Cortex‑M HardFault 深度定位调试
> 📄 文档路径：docs/cortex-m-hardfault-debug.md
>
> ✍️ 个人实践总结：Cortex‑M内核HardFault故障原理、故障寄存器解读；栈溢出、野指针、非对齐访问、数组越界等故障分类；HardFault捕获函数、LR/PC寄存器栈回溯方法；记录嵌入式项目死机崩溃定位实战踩坑记录。
## 概述
HardFault是Cortex‑M系列MCU最严重的异常，一旦触发程序直接卡死，现象为随机死机、偶现崩溃，很难复现。
常见诱因：野指针、数组越界、栈溢出、非对齐内存访问、非法寄存器写入。
本文讲解HardFault捕获原理，通过故障寄存器拿到PC、LR、栈信息，回溯找到崩溃代码位置，并且给出各类故障的排查思路与规避方案。
## 覆盖能力
- Cortex‑M异常模型，HardFault触发分类
- 故障寄存器：CFSR、HFSR、MMFAR、BFAR寄存器含义
- HardFault中断服务函数，保存故障现场
- LR、PC寄存器读取，定位崩溃代码行
- 栈回溯原理，分析函数调用链
- 典型故障案例：野指针、栈溢出、非对齐访问、数组越界
- 偶现死机的调试手段与工程预防方案
## 一、HardFault 故障分类
1. **访问违规（MemManage）**：访问受保护内存、非法读写Flash。
2. **总线故障（BusFault）**：访问不存在的内存地址、读无效指针。
3. **用法故障（UsageFault）**：非对齐内存访问、执行未定义指令、除零错误。
4. **硬故障HardFault**：上面三类异常未使能或者异常升级，最终进入HardFault。
> 工程最常见3类：**栈溢出 > 野指针 > 非对齐访问**。
## 二、关键故障寄存器说明
- HFSR：硬故障状态寄存器，标记HardFault来源
- CFSR：组合故障状态寄存器，包含MemManage、BusFault、UsageFault标志
- BFAR：总线故障地址寄存器，记录出错访问的内存地址
- MMFAR：内存管理故障地址，记录非法访问地址
> 调试核心：进入HardFault中断后，保存PC（程序计数器），PC的值就是崩溃指令所在地址，可在.map文件查找对应代码行。
## 三、HardFault捕获代码（Cortex-M通用）
```c
void HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4\n"
        " ite eq\n"
        " mrseq r0, msp\n"
        " mrsne r0, psp\n"
        " ldr r1, [r0, #24]\n"
        " ldr r2, handler2_address_const\n"
        " bx r2\n"
        " handler2_address_const: .word hardfault_handler_c\n"
    );
}
void hardfault_handler_c(uint32_t *sp)
{
    uint32_t r0 = sp[0];
    uint32_t r1 = sp[1];
    uint32_t r2 = sp[2];
    uint32_t r3 = sp[3];
    uint32_t r12 = sp[4];
    uint32_t lr = sp[5];
    uint32_t pc = sp[6];  // PC：崩溃指令地址
    uint32_t psr = sp[7];
    // 在这里保存故障信息到Flash或者串口打印
    while(1)
    {
        // 死循环停机，等待调试器连接
    }
}
```
原理：自动读取主栈/进程栈，提取寄存器、PC、LR，拿到崩溃现场。
## 四、各类故障现象与定位方法
### 4.1 栈溢出
现象：程序运行一段时间随机死机，崩溃地址不固定，多在函数调用后触发。
根因：局部大数组、递归调用，超出启动文件设置的Stack栈大小。
排查：J-Link查看SP栈指针；FreeRTOS使用`uxTaskGetStackHighWaterMark`查看栈剩余水位。
解决：局部大数组改为`static`；增大启动文件Stack大小；拆分超大函数。
### 4.2 野指针 / 空指针访问
现象：访问`NULL`指针，直接触发总线故障。
根因：指针未初始化、指针释放后继续使用。
解决：所有指针使用前判空；函数返回指针做合法性校验。
### 4.3 非对齐内存访问
Cortex‑M0/M0+不支持非对齐访问；M3/M4虽然支持但部分场景会触发UsageFault。
例：uint32_t变量从非4字节对齐地址读取。
解决：32位变量地址保持4字节对齐；联合体、DMA缓冲区增加对齐属性`__ALIGN(4)`。
### 4.4 数组越界
数组下标超出数组范围，破坏相邻内存，偶现死机。
特点：崩溃位置和越界代码不在同一函数，最难排查。
解决：增加下标边界判断；开启内存检测。
## 五、工程调试步骤
1. 触发HardFault，进入故障处理函数，打印PC、LR、BFAR、CFSR寄存器。
2. 使用Keil生成的`.map`文件，根据PC地址定位到对应的C代码行。
3. 根据故障寄存器判断故障类型：总线错误/内存访问/用法错误。
4. 回溯调用栈，找到上层函数。
5. 复现问题，增加日志，定位偶现bug。
## 六、工程踩坑记录
1. 开启看门狗后，断点暂停依旧计数，直接触发复位，看不到HardFault现场。
> 解决：调试器配置暂停时关闭看门狗；Debug版本默认关闭看门狗。
2. 局部定义超大数组，直接耗尽栈空间，上电直接HardFault。
3. DMA缓冲区放在栈上，DMA改写栈内存，随机崩溃。
4. 指针强制类型转换，造成非对齐访问，偶现死机。
5. 故障信息没有保存，断电后丢失现场，无法复现偶现故障。
> 工程方案：故障发生将故障寄存器写入Flash，上电后读取故障日志。
## 七、适用项目场景
- STM32 FOC电机项目：电机高速运行偶发HardFault、栈溢出排查
- 智能充电桩项目：高压控制逻辑死机、内存越界故障定位
- AT32 IAP Bootloader：升级过程异常崩溃捕获
## 参考资料
- ARM Cortex‑M Reference Manual
- ST AN2068：HardFault故障定位应用笔记
- 《Cortex-M内核死机HardFault深度解析》
- Keil .map文件分析教程
📌返回导航
[⬅ 返回作品集首页](../README.md)