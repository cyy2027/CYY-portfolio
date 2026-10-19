# FreeRTOS实战开发笔记：任务、队列、信号量、常见Bug
> 文档位置：`docs/tech-notes/freertos_practice.md`
> 
> 本文为项目开发整理的 FreeRTOS 实战经验与踩坑记录，**不是完整可编译工程源码**，案例来源于 STM32 无刷电机 FOC 控制系统项目。

## 1. 任务基础
### 1.1 任务创建与栈大小
- 任务创建API：`xTaskCreate()`，任务函数必须是`void vTaskFunction(void *pvParameters)`，**不能return**，内部一般放死循环。
- 栈大小单位：**words（字，不是字节）**。STM32 32位MCU，1 word = 4 Byte。
- 栈大小估算：局部变量、函数嵌套、中断压栈都会占用栈空间，预留余量，过小直接栈溢出。
> 项目踩坑：FOC电机控制任务，一开始栈设置太小，高速运行时随机死机；增大栈空间并开启栈水位检测定位问题。
```c
// 示例：创建电机控制任务
xTaskCreate(vMotorControlTask, "MotorTask", 256, NULL, 4, &xMotorTaskHandle);
```
- 任务优先级：数字越大优先级越高，`0`为最低优先级。空闲任务优先级固定为0。

### 1.2 任务延时 `vTaskDelay()` / `vTaskDelayUntil()`
1. `vTaskDelay(n)`：相对延时，从**本次执行完毕**开始计时；受任务抢占影响，周期不稳定。
2. `vTaskDelayUntil()`：绝对周期延时，适合需要**固定周期执行**的业务（传感器采集、电机控制），推荐周期性任务优先使用。
> 考点：做周期采集任务，为什么优先`vTaskDelayUntil`而不是`vTaskDelay`。

## 2. 任务间通信
### 2.1 队列 Queue
- 作用：任务之间传递数据，支持拷贝传递，数据存入队列缓冲区。
- 特点：FIFO，多任务读写安全；可以传递简单变量、结构体。
- 两种传递方式：
  1. **值拷贝**：直接复制数据到队列，数据独立，开销稍大，安全；
  2. **传递指针**：队列存放地址，节省拷贝开销，**必须保证内存有效，防止野指针**。
> 踩坑：传递局部变量指针入队，任务切换后局部变量栈内存失效，读取乱数据。

```c
// 创建队列，存储10个int类型数据
xQueueHandle xUartQueue = xQueueCreate(10, sizeof(uint8_t));
```

### 2.2 信号量
1. **二值信号量**：适合事件通知（中断通知任务：收到串口数据、外部按键触发）。
    > 典型场景：EXTI外部中断置位二值信号量，唤醒任务处理业务。
2. **计数信号量**：资源计数，例如多个硬件资源、多次事件累积。
3. **互斥量 Mutex**：保护共享资源（全局变量、硬件外设），**自带优先级继承，解决优先级反转**。
    > ⚠️互斥量只能在任务内使用，**中断中禁止使用Mutex**。

> 区分记忆：
> - 事件通知 → 二值信号量
> - 资源保护 → 互斥锁Mutex
> - 多份资源 → 计数信号量

## 3. 中断内FreeRTOS使用规则（高频面试题）
> 核心规则：**中断里面只能调用带 `FromISR` 后缀的API**
- 允许：`xSemaphoreGiveFromISR()`、`xQueueSendFromISR()`
- 禁止：`xSemaphoreGive()`、`xQueueSend()`、`vTaskDelay()`、`xTaskCreate()`
- `pxHigherPriorityTaskWoken`：标记是否唤醒更高优先级任务；退出中断前手动执行`portYIELD_FROM_ISR()`进行任务切换。

```c
// 中断内发送信号量示例
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
```
> 踩坑：中断误用不带FromISR的API，直接造成系统崩溃。

## 4. 经典问题：优先级反转
- 现象：高优先级任务被低优先级任务阻塞，中间优先级任务抢占，高优先级任务迟迟得不到运行。
- 解决方案：使用**互斥量Mutex**，自带优先级继承机制；普通二值信号量**没有优先级继承，无法解决优先级反转**。

## 5. 常见Bug与踩坑清单
1. **栈溢出**
    现象：程序随机死机、hardfault，无固定复现场景。
    定位：开启`uxTaskGetStackHighWaterMark()`查看栈剩余水位，预留足够栈空间。
2. **死锁**
    场景：任务A持有锁1等待锁2；任务B持有锁2等待锁1。
    规避：所有任务获取多个锁，保持统一获取顺序。
3. **中断中调用非FromISR接口** → 系统崩溃。
4. 全局共享变量，多任务访问缺少保护，数据错乱。
    解决：短变量关中断；长业务操作使用互斥锁。
5. 任务函数内部`return`，任务退出，FreeRTOS会自动调用空闲任务释放资源，容易被忽略。
6. 不要在任务内无限while死循环不加阻塞，**裸跑空循环会把CPU占满，空闲任务无法执行**。
    ```c
    // ❌错误
    while(1){}
    // ✅正确
    while(1){
        //业务代码
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    ```

## 6. 任务划分思路（FOC项目实例）
1. 高优先级：电机FOC控制任务，固定周期运行，保证电流环实时性；
2. 中优先级：串口数据解析、状态机处理；
3. 低优先级：按键扫描、屏幕刷新、日志打印；
> 设计原则：**高优先级任务执行时间尽量短，不要做耗时运算**。

## 7. 调试工具
1. `uxTaskGetStackHighWaterMark()`：查询任务最小剩余栈，评估栈是否充足。
2. 任务列表函数`vTaskList()`：输出任务状态、优先级、栈信息，用于串口打印调试。
3. 钩子函数：空闲钩子、栈溢出钩子，用于捕获异常。

## 关联项目
- STM32无刷电机FOC控制系统：多任务划分、固定周期任务、信号量事件通知、栈水位调试。

## 📌返回导航
[⬅ 返回作品集首页](../../README.md)