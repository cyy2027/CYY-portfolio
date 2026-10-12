/**********************************
作者：特纳斯电子
网站：https://www.mcude.com
联系方式：46580829(QQ)
淘宝店铺：特纳斯电子
**********************************/
//头文件
#include "./HAL/ESP8266/esp8266.h"
#include "usart.h"
#include "./HAL/OLED/OLED_NEW.H"


unsigned char esp8266_buf[4096];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;
char device_data[128];
uint8_t SmartConfig;									//配网标志位
uint8_t wifi_status;									//wifi状态标志位
uint8_t uart1_value,UART1_Flag;				//串口2接收缓存变量
uint16_t time_flag_1ms,sec_3;

/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart_SendString(unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		HAL_UART_Transmit(&huart1, str++, 1, 999);
	}
}
//==========================================================
//	函数名称：	ESP8266_Clear
//	函数功能：	清空缓存
//	入口参数：	无
//	返回参数：	无
//==========================================================
void ESP8266_Clear(void)
{
	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;
}

//==========================================================
//	函数名称：	ESP8266_WaitRecive
//	函数功能：	等待接收完成
//	入口参数：	无
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;

	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0;							//清0接收计数
		return REV_OK;								//返回接收完成标志
	}
	esp8266_cntPre = esp8266_cnt;					//置为相同
	return REV_WAIT;								//返回接收未完成标志

}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//	函数功能：	发送命令
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//	返回参数：	0-成功	1-失败
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	unsigned short timeOut = 300;

	Usart_SendString((unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				ESP8266_Clear();									//清空缓存
				return 0;
			}
		}
		DelayXms(10);
	}
	return 1;
}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//	函数功能：	获取平台返回的数据
//	入口参数：	等待的时间(乘以10ms)
//	返回参数：	平台返回的原始数据
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{
	char *ptrIPD = NULL;
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
      //如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			if(ptrIPD == NULL)											
			{
          
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
			}
		}
		DelayXms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针
}

//==========================================================
//	函数名称：	Esp8266LinkAp
//	函数功能：	连接WiFi
//	入口参数：	无
//	返回参数：	无
//==========================================================
void Esp8266LinkAp(void)
{
	ESP8266_SendCmd("AT+RST\r\n", "ready");
	DelayXms(100);
	
	ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK");
	DelayXms(100);

//	sprintf(device_data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, Password);
//	ESP8266_SendCmd(device_data, "OK");
	DelayXms(100);
}

//==========================================================
//	函数名称：	Esp8266LinkloTExplorer
//	函数功能：	连接腾讯云，并且订阅主题
//	入口参数：	无
//	返回参数：	无
//==========================================================
void Esp8266LinkloTExplorer(void)
{
	ESP8266_SendCmd("AT+TCMQTTDISCONN\r\n", "");
	DelayXms(100);

	sprintf(device_data, "AT+TCDEVINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", \
							PRODUCT_ID, DEVUICE_NAME, DEVICE_SECRET);
	ESP8266_SendCmd(device_data, "OK");
	DelayXms(100);
		
	ESP8266_SendCmd("AT+TCMQTTCONN=1,5000,240,0,1\r\n", "");
	DelayXms(1000);
  ESP8266_SendCmd("AT+CIPSNTPCFG=1,8\r\n", "OK");			//设置时区
	DelayXms(1000);

	/* 订阅主题 */
  sprintf((char *)device_data, "AT+TCMQTTSUB=\"$thing/down/property/%s/%s\",0\r\n", PRODUCT_ID, DEVUICE_NAME);
	ESP8266_SendCmd(device_data, "+TCMQTTSUB:OK");
}

//==========================================================
//	函数名称：	TenCent_MQTT_Publish
//	函数功能：	向腾讯云发布消息
//	入口参数：	无
//	返回参数：	无
//==========================================================
extern uint8_t price;														//单价
extern uint8_t flag_switch;											//开关继电器标志位
extern uint16_t voltage;												//电压值
extern uint8_t battery;													//电量
void TenCent_MQTT_Publish(void)
{
	char buf[300];
	char txt[300];
	memset(txt, 0, sizeof txt);
	strcat(txt, "{\"method\":\"report\",\"clientToken\":\"123456\",\"params\":{");
	/* 数据点整合 */
	sprintf(buf, "\"price\":%d,", price);
	strcat(txt, buf);
	sprintf(buf, "\"voltage\":%.2f,", voltage/100.00);
	strcat(txt, buf);
  sprintf(buf, "\"battery\":%d,", battery);
	strcat(txt, buf);
	sprintf(buf, "\"flag_switch\":%d", flag_switch);
	strcat(txt, buf);
	strcat(txt, "}}\r\n");
	//发送数据
	sprintf(buf, "AT+TCMQTTPUBL=\"$thing/up/property/%s/%s\",0,%d\r\n", \
																			PRODUCT_ID, DEVUICE_NAME, strlen(txt)-2);
	ESP8266_SendCmd(buf, ">");
	
	ESP8266_SendCmd(txt, "");
}

//==========================================================
//	函数名称：	TenCent_MQTT_Recevie
//	函数功能：	接收腾讯云下发的消息
//	入口参数：	无
//	返回参数：	无
//==========================================================

void TenCent_MQTT_Recevie(void)
{
	char *Start;
  Start = strstr((char *)esp8266_buf, "price");
	if(Start != NULL)
	{
		Start += 7;
		if(*(Start+1)=='}')
      price = *Start - 0x30;
    else if(*(Start+2)=='}')
      price = (*Start - 0x30)*10+(*(Start+1)- 0x30);
    else if(*(Start+3)=='}')
      price = (*Start - 0x30)*100+(*(Start+1)- 0x30)*10+(*(Start+2)- 0x30);
		ESP8266_Clear();
	}
}

//==========================================================
//	函数名称：	ESP8266_SmartConfig
//	函数功能：	ESP8266智能配网
//	入口参数：	无
//	返回参数：	无
//==========================================================
void ESP8266_SmartConfig(void)				
{
	if(SmartConfig == 1)								//进行一次配网
	{
		wifi_status = 0;
		ESP8266_SendCmd("AT+RESTORE\r\n", "ready");
		DelayXms(100);
		ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK");
		DelayXms(100);
		sprintf(device_data, "AT+TCDEVINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", \
								PRODUCT_ID, DEVUICE_NAME, DEVICE_SECRET);
		ESP8266_SendCmd(device_data, "OK");			//配置设备信息
		DelayXms(100);
		ESP8266_Clear();
		Usart_SendString("AT+TCSTARTSMART\r\n", strlen("AT+TCSTARTSMART\r\n"));	//开始配网
		SmartConfig = 2;	
	}
	else if(SmartConfig == 2)
	{
		if(strstr((char *)esp8266_buf, "TCSTARTSMART:OK") != NULL)
		{
			SmartConfig = 3;
			ESP8266_Clear();
		}
		else if(strstr((char *)esp8266_buf, "TCSTARTSMART:FAIL") != NULL)
		{
			HAL_Delay(1000);
			SmartConfig = 0;
			ESP8266_Clear();
		}
	}
	else if(SmartConfig == 3)
	{
		if(strstr((char *)esp8266_buf, "WIFI_CONNECT_SUCCESS") != NULL)
		{
			HAL_Delay(1000);
			SmartConfig = 0;
      OLED_Clear();
			ESP8266_Clear();
		}
		else if(strstr((char *)esp8266_buf, "WIFI_CONNECT_FAILED") != NULL)
		{
			HAL_Delay(1000);
			SmartConfig = 0;
			ESP8266_Clear();
      OLED_Clear();
		}
	}
}

//==========================================================
//	函数名称：	ESP8266_Status
//	函数功能：	ESP8266状态判断
//	入口参数：	无
//	返回参数：	0-连接状态     1-断开状态
//AT+CIPSTA_CUR?
//+CIPSTA_CUR:ip:"192.168.2.12"
//+CIPSTA_CUR:gateway:"192.168.2.1"
//+CIPSTA_CUR:netmask:"255.255.255.0"
//OK
//==========================================================
_Bool ESP8266_Status(void)
{

	unsigned int timeOut;
	char *Status;
	/* IP查询 */
	Usart_SendString((unsigned char *)"AT+CIPSTA?\r\n", strlen((const char *)"AT+CIPSTA?\r\n"));
	timeOut = 50;
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, "OK") != NULL)		//如果检索到关键词
			{
				Status = strstr((const char *)esp8266_buf, "ip:");
				if(*(Status+4)!='0')
				{
					ESP8266_Clear();									//清空缓存
					return 0;
				}
				else
				{
					ESP8266_Clear();									//清空缓存
					return 1;
				}
			}
		}
		DelayXms(10);
	}
	ESP8266_Clear();
	return 1;
}

//==========================================================
//	函数名称：	ESP8266_Get_Time
//	函数功能：	获取网络时间
//	入口参数：	无
//	返回参数：	无
//==========================================================
Time_Get ESP8266_Get_Time(void)
{
	unsigned short timeOut;
	char *Start;
	Time_Get Time;
	
	Usart_SendString((unsigned char *)"AT+CIPSNTPTIME?\r\n", strlen((const char *)"AT+CIPSNTPTIME?\r\n"));
	timeOut = 2000;
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, "OK") != NULL)		//如果检索到关键词
			{
				Start = strstr((const char *)esp8266_buf, "TIME:") + 5;
				
				if(strstr(Start, "Mon") != NULL)				//星期获取
					Time.week = 1;
				else if(strstr(Start, "Tue") != NULL)
					Time.week = 2;
				else if(strstr(Start, "Wed") != NULL)
					Time.week = 3;
				else if(strstr(Start, "Thu") != NULL)
					Time.week = 4;
				else if(strstr(Start, "Fri") != NULL)
					Time.week = 5;
				else if(strstr(Start, "Sat") != NULL)
					Time.week = 6;
				else if(strstr(Start, "Sun") != NULL)
					Time.week = 7;
				
				if(strstr(Start, "Jan") != NULL)				//月份获取
					Time.month = 1;
				else if(strstr(Start, "Feb") != NULL)
					Time.month = 2;
				else if(strstr(Start, "Mar") != NULL)
					Time.month = 3;
				else if(strstr(Start, "Apr") != NULL)
					Time.month = 4;
				else if(strstr(Start, "May") != NULL)
					Time.month = 5;
				else if(strstr(Start, "Jun") != NULL)
					Time.month = 6;
				else if(strstr(Start, "Jul") != NULL)
					Time.month = 7;
				else if(strstr(Start, "Aug") != NULL)
					Time.month = 8;
				else if(strstr(Start, "Sep") != NULL)
					Time.month = 9;
				else if(strstr(Start, "Oct") != NULL)
					Time.month = 10;
				else if(strstr(Start, "Nov") != NULL)
					Time.month = 11;
				else if(strstr(Start, "Dec") != NULL)
					Time.month = 12;
				
				Start = strstr(Start, " ") + 1;		//月份首地址
				Start = strstr(Start, " ") + 1;		//日期首地址
				if(*Start==' ')
				{
				  Start = strstr(Start, " ") + 1;		//日期首地址
				  Time.day = (*Start-'0');		//获取日期
				}
				else
				{
				  Time.day = (*Start-'0')*10 + (*(Start+1)-'0');		//获取日期
				}
				Start = strstr(Start, " ") + 1;		//小时首地址
				Time.hour = (*Start-'0')*10 + (*(Start+1)-'0');		//获取小时
				Start = strstr(Start, ":") + 1;		//分钟首地址
				Time.minute = (*Start-'0')*10 + (*(Start+1)-'0');		//获取分钟
				Start = strstr(Start, ":") + 1;		//秒首地址
				Time.second = (*Start-'0')*10 + (*(Start+1)-'0');		//获取秒
				Start = strstr(Start, " ") + 1;		//年首地址
				Time.year = (*Start-'0')*1000 + (*(Start+1)-'0')*100 + (*(Start+2)-'0')*10 + (*(Start+3)-'0');//获取年
				
				ESP8266_Clear();									//清空缓存
				break;
			}
		}
		
		DelayXms(10);
	}
	return Time;
}

//==========================================================
//	函数名称：	ESP8266_APP
//	函数功能：	主函数调用函数，封装上传以及下发
//	入口参数：	无
//	返回参数：	无
//==========================================================
void  ESP8266_App(void)
{
  if(SmartConfig == 0)
	{
		if(sec_3 == 1)							//每3秒上报一次数据
		{
			sec_3 = 0;
			if(ESP8266_Status() == 0)		//WiFi连接状态
			{
				HAL_Delay(10);
				if(wifi_status == 0)
				{
					wifi_status = 1;
					Esp8266LinkloTExplorer();
				}
				/* 上报数据 */
				TenCent_MQTT_Publish();
			}
			else
			{
				if(wifi_status == 1)		//WiFi未连接-显示断开
				{
					wifi_status = 0;
				}
			}
		}
	}
	TenCent_MQTT_Recevie();
	ESP8266_SmartConfig();
}
