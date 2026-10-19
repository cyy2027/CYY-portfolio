# 非阻塞FSM设计：从阻塞Delay到Tick驱动
> 📄 文档路径：`docs/realtime‑fsm.md`
> 
> ✍️ 个人实践总结：裸机前后台、FreeRTOS环境下有限状态机(FSM)实践；消除`HAL_Delay`类阻塞延时，基于系统Tick实现事件驱动任务调度，记录工程踩坑与代码示例。

## 概述
嵌入式开发新手经常大量使用阻塞延时`HAL_Delay()`、`for(;;)延时循环`做等待逻辑。**阻塞延时会卡死整个主循环，其他外设扫描、按键、通信、采样全部停滞，系统响应完全丢失**。

本文讲解**Tick驱动非阻塞有限状态机FSM**，分别覆盖裸机前后台架构与FreeRTOS两种环境；把“等待”变成状态+时间戳判断，不阻塞CPU，实现多逻辑并行运行；对比FSM三种实现方式，提供可复用伪代码，总结实际项目踩坑点。

### 覆盖能力
- 理解阻塞延时带来的系统缺陷
- 裸机前后台架构：1ms Tick时基、软件定时器、非阻塞时间判断
- 有限状态机FSM三种实现方式：switch‑case、状态转移表、函数指针FSM
- FreeRTOS下状态机实践：任务、事件组、队列解耦业务逻辑
- 状态机建模、进入/退出动作、超时防护、异常状态处理
- 工程踩坑：标志位竞争、中断与主循环数据同步、状态死锁

## 一、为什么要抛弃阻塞Delay
### 阻塞代码反面示例（不推荐）
```c
// 反面示例：阻塞延时
led_on();
HAL_Delay(500);   // ❌ 500ms内CPU原地等待，按键、串口、ADC全部停止
led_off();
HAL_Delay(500);
```


### 阻塞Delay带来的问题

1. **系统响应停滞**：延时期间无法扫描按键、解析串口报文、采集传感器数据；
2. **无法并行多业务**：多个需要定时的业务串行排队；
3. RTOS环境下：`HAL_Delay`依赖SysTick，会与FreeRTOS内核时钟冲突，出现卡死现象；
4. 延时嵌套时代码可读性急剧恶化，难以维护、增加功能困难。

> 
> 💡 核心思想：**不要等待时间流逝，只记录时间戳，每次循环判断“时间是否到点”**，CPU可以去处理别的业务。

## 二、Tick时基与非阻塞延时原理

### 裸机前后台架构时基来源

使用硬件定时器产生1ms中断，维护全局系统滴答`sys_tick`（前台中断只做计数，不执行业务）。

```
volatile uint32_t sys_tick = 0;

// TIM 1ms中断(前台)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        sys_tick++; // 仅计数，禁止在中断写复杂业务
    }
}

```
### 非阻塞延时工具函数（不阻塞）

```
// 判断是否超时，start_tick为记录的起始时间戳
bool tick_expired(uint32_t start_tick, uint32_t delay_ms)
{
    return (sys_tick - start_tick) >= delay_ms;
}
```

### 非阻塞LED闪烁示例

```
uint32_t led_tick = 0;

void led_task(void)
{
    if(tick_expired(led_tick,500))
    {
        led_tick = sys_tick; // 更新时间戳
        HAL_GPIO_TogglePin(GPIOA,GPIO_PIN5);
    }
}
```

> 
> 该函数调用立刻返回，不会卡死，主循环可以继续跑按键、串口等其他逻辑。

## 三、有限状态机FSM基础概念

- **状态 State**：系统当前所处模式，如`STATE_IDLE`空闲、`STATE_WAIT_ACK`等待应答、`STATE_ERROR`异常
- **事件 Event**：触发状态切换条件：按键按下、数据接收完成、超时、指令到来
- **转换 Transition**：从A状态跳转到B状态
- **Action动作**：进入状态、状态循环、退出状态执行的业务逻辑

> 
> 三种常用FSM实现对比

| 实现方式 | 优点 | 缺点 | 适用场景 |
| --- | --- | --- | --- |
| Switch‑Case状态机 | 代码直观，上手简单，调试方便 | 状态变多后case块臃肿 | 中小型模块，推荐项目首选 |
| 状态转移查表法 | 跳转逻辑数据化，新增状态只改表格 | 需要维护转移表，ROM占用略大 | 协议解析、通信模块 |
| 函数指针FSM | 每个状态独立函数，高内聚 | 函数数量多，需要管理函数数组 | 大型复杂业务逻辑 |

### 方式1：Switch‑Case FSM（工程最常用）

```
typedef enum{
    STATE_IDLE,
    STATE_SEND,
    STATE_WAIT_ACK,
    STATE_TIMEOUT,
    STATE_ERROR
}fsm_state_t;

typedef struct{
    fsm_state_t cur_state;
    uint32_t time_stamp;
}demo_fsm_t;

demo_fsm_t fsm_inst;

void demo_fsm_run(void)
{
    switch(fsm_inst.cur_state)
    {
        case STATE_IDLE:
            // 进入状态动作
            if(recv_cmd_flag)
            {
                recv_cmd_flag = 0;
                fsm_inst.cur_state = STATE_SEND;
            }
        break;

        case STATE_SEND:
            uart_send_data();
            fsm_inst.time_stamp = sys_tick;
            fsm_inst.cur_state = STATE_WAIT_ACK;
        break;

        case STATE_WAIT_ACK:
            if(ack_recv_flag)
            {
                ack_recv_flag = 0;
                fsm_inst.cur_state = STATE_IDLE; // 正常回到空闲
            }
            else if(tick_expired(fsm_inst.time_stamp,200))
            {
                fsm_inst.cur_state = STATE_TIMEOUT; // 超时跳转
            }
        break;

        case STATE_TIMEOUT:
            // 超时处理
            fsm_inst.cur_state = STATE_IDLE;
        break;

        case STATE_ERROR:
            // 错误复位
            fsm_inst.cur_state = STATE_IDLE;
        break;
        default:
            fsm_inst.cur_state = STATE_IDLE; // 异常状态兜底
        break;
    }
}
```

> 
> 主循环调用

```
while(1)
{
    key_scan();
    uart_process();
    demo_fsm_run(); // 状态机周期调度
    led_task();
}
```

### 方式2：状态转移查表（事件驱动）

把状态‑事件‑动作‑下一个状态写成表格，适合通信协议解析。

```
typedef enum{
    EVT_NONE,
    EVT_CMD_RECV,
    EVT_ACK_OK,
    EVT_TIMEOUT,
    EVT_ERR
}event_t;

typedef struct{
    fsm_state_t cur;
    event_t event;
    void (*action)(void);
    fsm_state_t next;
}trans_table_t;

const trans_table_t fsm_table[] = {
    {STATE_IDLE, EVT_CMD_RECV, action_send, STATE_SEND},
    {STATE_SEND, EVT_NONE, NULL, STATE_WAIT_ACK},
    {STATE_WAIT_ACK, EVT_ACK_OK, action_ok, STATE_IDLE},
    {STATE_WAIT_ACK, EVT_TIMEOUT, action_timeout, STATE_TIMEOUT},
};
```

## 四、FreeRTOS环境下FSM与任务调度

FreeRTOS环境下依然**不推荐大量使用`osDelay()`做业务等待**，优先采用「状态机+队列/事件组」实现事件驱动，避免任务长时间阻塞。

### 推荐架构

1. **FSM业务任务**：死循环，阻塞在队列读取，收到事件后驱动状态机流转；
2. **中断/底层任务**：产生事件，发送消息队列，不执行业务；
3. 超时逻辑：任务内部记录tick时间戳，判断超时，不调用长延时。

伪代码示例

```
void fsm_task(void *arg)
{
    QueueHandle_t evt_queue = *(QueueHandle_t*)arg;
    event_t evt;
    for(;;)
    {
        // 等待事件，短超时做超时巡检
        if(xQueueReceive(evt_queue,&evt,pdMS_TO_TICKS(10)) == pdPASS)
        {
            fsm_dispatch_event(evt); // 收到事件驱动状态机
        }
        fsm_timeout_check(); // 每10ms检查超时条件
    }
}
```

> 
> ⚠️注意：不要在中断内执行复杂FSM逻辑；中断只置标志/发送队列，业务交给任务处理。

## 五、关键工程设计要点

### 5.1 进入动作 / 退出动作

很多bug根源：状态切换只改状态变量，**缺少进入状态初始化操作**。

- 进入状态：初始化时间戳、清除标志位、硬件IO动作；
- 退出状态：释放资源、关闭外设、清除临时变量。

### 5.2 超时与异常兜底

1. 每个等待状态必须配置超时保护，防止外部报文丢失导致**状态机死锁停在某一状态**；
2. switch‑case必须写`default`分支，异常状态强制切回空闲。

### 5.3 中断与主循环的数据同步

> 
> 问题：中断修改标志位，主循环读取，出现数据撕裂。

- 单bit标志：使用`volatile`修饰；读取后立刻在主循环清除，不在中断清除标志；
- 多字节结构体：关闭中断做拷贝，禁止跨中断直接读写复杂结构体。

### 5.4 多个FSM实例化

把状态、时间戳、私有变量封装进`struct`结构体，支持多套独立状态机，例如两个串口解析、两路按键逻辑，不要全部使用全局零散变量。

## 六、实测踩坑记录

1. **状态机死锁，卡在等待状态不再流转**
   - 现象：设备偶尔不再响应；调试发现停留在`STATE_WAIT_ACK`。
   - 根因：没有超时逻辑，外部报文丢失，永远等不到事件。
   - 解决：所有等待类状态增加时间戳+超时跳转，default兜底复位。
2. **时间戳溢出后逻辑错乱**
   - 现象：运行若干天后，非阻塞延时判断失效。
   - 根因：`uint32_t`溢出；直接做`if(sys_tick >= stamp+delay)`写法溢出会出错。
   - 解决：固定使用减法判断 `sys_tick‑stamp >= delay_ms`，无溢出问题。
3. **中断与主循环标志位竞争，事件丢失**
   - 现象：偶尔事件无法触发状态跳转。
   - 根因：中断置标志，主循环还没读取，中断再次覆盖标志。
   - 解决：简单场景使用事件计数；复杂场景改用消息队列缓存事件。
4. **FreeRTOS任务大量使用osDelay，业务响应卡顿**
   - 现象：一个任务长延时，新事件到来不能及时处理。
   - 根因：把延时当做业务逻辑，而不是时间片。
   - 解决：状态机+队列接收事件，短超时做巡检，业务不要依赖长延时阻塞。
5. **状态切换忘记初始化时间戳，超时条件立即触发**
   - 现象：刚跳转到等待状态立刻报超时。
   - 根因：切换状态没有更新`time_stamp = sys_tick`。
   - 解决：进入等待状态时统一记录时间戳作为计时起点。

## 七、总结

- 阻塞`HAL_Delay`适合简单演示项目，**产品级代码要全部替换为Tick时间戳非阻塞判断**；
- 裸机前后台：定时器产生1ms系统tick，主循环轮询调度多个FSM任务，实现逻辑并行；
- Switch‑case状态机对绝大多数嵌入式项目性价比最高，可读性好，调试便捷；
- 任何等待状态必须加**超时+default异常兜底**，防止状态死锁；
- FreeRTOS不代表就可以随便写长延时，优先事件+队列驱动，减少任务阻塞时间。

> 
> ℹ️本文档只描述设计思路，核心伪代码片段放置于`src/`目录，不提供完整Keil工程。

## 参考资料

- STM32 前后台应用编程笔记 AN1282
- FreeRTOS官方文档：Deferred interrupt handling延迟中断处理模式
- 《嵌入式C语言状态机编程》21IC工业控制实践
- SimpleFSM有限状态机开源实践

## 📌返回导航
[⬅ 返回作品集首页](../README.md)