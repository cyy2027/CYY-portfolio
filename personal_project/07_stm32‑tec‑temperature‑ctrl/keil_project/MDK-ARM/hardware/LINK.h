#ifndef __LINK_H
#define __LINK_H

#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 外部目标温度变量（在 main.c 中定义）
extern float target_temperature;

// 函数声明
void LINK_Init(void);                      // 初始化蓝牙模块（开启接收中断）
void UART_Receive_Init(void);               // 启动串口接收中断
void UART_ProcessReceivedData(void);        // 处理接收到的数据（需在主循环调用）
void UART_CheckTimeout(void);               // 检查接收超时

// 新增：恒温状态通知
void LINK_SendStable(float temp);           // 发送恒温成功消息
void LINK_SendUnstable(void);                // 发送恒温失败消息
void LINK_SetTargetChanged(void);            // 标记目标温度被修改
uint8_t LINK_GetAndClearTargetChanged(void); // 检查并清除目标修改标记
// 在现有声明后添加
void LINK_ClearTargetChanged(void);         // 清除目标修改标记
// 响应缓冲区（供主程序显示，可选）
extern char response_buffer[32];

#endif

