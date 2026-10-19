
# FreeRTOS 陷阱笔记
> 📄 文档路径：docs/freertos-trap-notes.md
> 
> ✍️ 个人实践总结：FreeRTOS嵌入式开发高频陷阱；任务栈、优先级翻转、死锁、队列/信号量中断API使用限制；内核API误用案例；多任务工程避坑，FOC电机、充电桩项目实战记录。

## 概述
FreeRTOS是嵌入式MCU最常用的实时操作系统，简化多任务调度。但很多API存在隐性限制，新手极易踩坑：中断误用普通API、栈大小估算不足、互斥锁优先级翻转、死锁、阻塞调用放在中断等，会造成随机死机、任务卡死、HardFault。
本文整理工程开发中高频问题，梳理原理、现象、规避方案，适合电机控制器、充电桩等多任务固件开发。

## 覆盖能力
- 任务栈原理、栈水位监测、栈溢出风险识别
- 任务优先级，优先级翻转问题与互斥锁使用规范
- 队列、信号量、事件组：中断上下文只能调用`FromISR`系列接口
- 死锁产生场景与工程规避方法
- 软件定时器任务（Timer Task）执行限制
- 阻塞延时`vTaskDelay()`与绝对延时`vTaskDelayUntil()`区别
- 中断内调用FreeRTOS API的硬性规则
- 内存碎片、动态内存分配风险

## 一、任务栈相关陷阱
栈溢出是FreeRTOS项目最高发故障。每个任务拥有独立栈空间，栈不足会直接覆盖内存，触发随机HardFault。
1. 栈大小不能凭感觉设置：函数局部大数组、函数嵌套深度、浮点数运算都会占用栈。
2. 栈水位监测：`uxTaskGetStackHighWaterMark()`，查看任务运行后剩余最小栈空间，预留30%余量。
```c
// 获取任务剩余栈
uint16_t stack_remain = uxTaskGetStackHighWaterMark(NULL);
```
> 坑：浮点数任务需要额外开启FPU上下文保存，栈消耗会额外增加。

## 二、互斥锁与优先级翻转
优先级翻转：低优先级任务持有互斥锁，高优先级任务阻塞等待；此时中优先级任务抢占CPU，高优先级任务被长时间阻塞，破坏实时性。
- 互斥锁`xSemaphoreCreateMutex()`自带**优先级继承**，专门缓解优先级翻转。
- ❌ 信号量`xSemaphoreCreateBinary()`**没有优先级继承**，不能用来做资源互斥。

> 工程规则：资源互斥必须使用互斥锁，不要用二值信号量替代。

## 三、中断上下文API硬性规则
**中断里面，只能调用后缀带 FromISR 的API！**
- ✅ `xQueueSendFromISR()` / `xSemaphoreGiveFromISR()`
- ❌ 禁止：`xQueueSend()`、`xSemaphoreGive()`、`vTaskDelay()`、`xTaskNotify()`普通版本

错误后果：内核调度器状态错乱，随机死机。
中断中发送消息后，必须检查pxHigherPriorityTaskWoken，最后调用`portYIELD_FROM_ISR()`。
```c
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
xSemaphoreGiveFromISR(sem_handle, &xHigherPriorityTaskWoken);
portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
```

## 四、死锁产生场景
典型死锁条件：任务A持有锁1，等待锁2；任务B持有锁2，等待锁1。
工程规避策略：
1. 多个互斥锁，**所有任务必须保持相同获取顺序**。
2. 获取锁增加超时时间`xSemaphoreTake(..., xTicksToWait)`，永久等待容易死锁。
3. 尽量减少嵌套持有多个互斥锁。

## 五、软件定时器任务陷阱
FreeRTOS软件定时器运行在单独的Timer服务任务，有严格限制：
1. 定时器回调函数**禁止阻塞**：不能调用`vTaskDelay()`、不能无限等待信号量。
2. 回调执行时间不能过长，会阻塞其他定时器。
> 长业务逻辑放到普通任务，定时器回调只做简短标记、发事件通知。

## 六、延时函数区别
1. `vTaskDelay(n)`：相对延时，从调用时刻等待n个tick；任务被抢占会造成延时不准。
2. `vTaskDelayUntil()`：绝对周期延时，适合固定周期控制（电机控制、ADC采样）。
> FOC电流环、速度环固定周期任务优先使用`vTaskDelayUntil()`。

## 七、动态内存与内存碎片
`pvPortMalloc()`频繁申请释放小块内存，会产生内存碎片，长时间运行出现malloc失败。
工程方案：
1. 固件运行期间尽量少动态申请内存，优先全局静态缓冲区。
2. 启用堆4（heap_4），支持内存合并；设置最大堆监控。

## 八、工程踩坑记录
1. 中断中直接调用`xSemaphoreGive()`，偶现系统崩溃。
> 解决：中断统一使用`FromISR`版本API。
2. 任务栈设置过小，高负载运行随机HardFault。
> 解决：开启栈水位监测，预留足够栈余量。
3. 使用二值信号量做资源互斥，出现严重优先级翻转。
> 解决：资源保护使用`xSemaphoreCreateMutex()`互斥锁。
4. 软件定时器回调里面写阻塞等待，定时器全部停止。
> 解决：定时器回调只做标记，业务交给普通任务。
5. 高优先级任务死循环不加阻塞，抢占全部CPU，低优先级任务永远不执行。
> 解决：高优先级任务必须有阻塞点（信号量、队列、延时）。
6. 任务句柄、队列句柄在任务删除后继续使用，内存访问异常。

## 九、适用项目场景
- STM32 FOC电机项目：多任务框架（电流采集、速度闭环、上位机通信、故障检测），任务优先级划分与栈评估
- 智能充电桩项目：充电控制、CAN通信、按键、状态机多任务调度，互斥锁保护共享资源

参考资料
FreeRTOS官方参考手册
Mastering the FreeRTOS Real Time Kernel
FreeRTOS优先级翻转与互斥锁说明
FreeRTOS中断API使用规范

📌返回导航
[⬅ 返回作品集首页](../README.md)