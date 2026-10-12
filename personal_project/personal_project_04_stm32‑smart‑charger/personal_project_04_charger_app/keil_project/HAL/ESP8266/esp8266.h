/**********************************

**********************************/
#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "main.h"

//C库
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define PRODUCT_ID 		"NE6D3O1OOM" 		//产品ID
#define DEVUICE_NAME 	"zncdmk" 	  //设备名称
#define DEVICE_SECRET "EPn2cTg85EJNW37F4Ctm7w=="//设备密钥

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志
#define DelayXms(x) HAL_Delay(x)

extern unsigned char esp8266_buf[4096];
extern unsigned short esp8266_cnt, esp8266_cntPre;

extern uint8_t SmartConfig;
extern uint8_t wifi_status;
extern uint8_t uart1_value,UART1_Flag;				//串口2接收缓存变量
extern uint16_t time_flag_1ms,sec_3;

typedef struct{			//时间结构体
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t week;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}Time_Get;

void Esp8266LinkAp(void);							//连接WiFi
void Esp8266LinkloTExplorer(void);		//连接腾讯云，并订阅主题

_Bool ESP8266_SendCmd(char *cmd, char *res);
void ESP8266_Clear(void);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);
void ESP8266_SmartConfig(void);
_Bool ESP8266_Status(void);
Time_Get ESP8266_Get_Time(void);
void TenCent_MQTT_Publish(void);		//上报消息
void TenCent_MQTT_Recevie(void);

void Usart_SendString(unsigned char *str, unsigned short len);
void  ESP8266_App(void);
//使用方法
//1、放在main函数while(1);
//ESP8266_App();

//2、放在1ms定时器中
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == htim1.Instance)		//定时器1触发中断
//	{
//		time_1ms++;
//		if(time_1ms>= 500)
//		{
//			time_1ms= 0;
//			time_500ms = 1;
//		}
//    time_flag_1ms++;
//		if(time_flag_1ms>3000)
//		{
//			time_flag_1ms=0;
//			sec_3 = 1;
//		}

//	}
//}

//
//3、串口回调函数中
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance == huart2.Instance)//串口2触发
//	{
//		HAL_UART_Receive_IT(&huart2, &uart2_value, 1);
//		if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //防止串口被刷爆
//		esp8266_buf[esp8266_cnt++] = uart2_value;	
//	}
//}
#endif
