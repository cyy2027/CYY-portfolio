/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "./HAL/key/key.h"
#include "./HAL/OLED/OLED_NEW.H"
#include "./HAL/ds18b20/ds18b20.h"
#include "./HAL/RC522/mfrc522.h"
#include "./HAL/ESP8266/esp8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void Key_function(void);								//按键函数
void Monitor_function(void);						//监测函数
void Display_function(void);						//显示函数
void Manage_function(void);							//处理函数
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t USART1_TX_BUF[200];
#define u1_printf(...)  HAL_UART_Transmit(&huart1,USART1_TX_BUF,sprintf((char *)USART1_TX_BUF,__VA_ARGS__),0xffff)
                                                                                                                                                                        
uint8_t USART2_TX_BUF[200];
#define u2_printf(...)  HAL_UART_Transmit(&huart2,USART2_TX_BUF,sprintf((char *)USART2_TX_BUF,__VA_ARGS__),0xffff)
  
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t key_num,flag_display;       		  //按键与显示变量
uint8_t flag_stop;												//结算界面标志位
uint16_t time_1ms,time_500ms;       		  //计时变量1ms,500ms
	
uint16_t voltage;													//电压值
uint16_t voltage_max = 600;								//电压阈值
uint8_t battery;													//电量
	
uint16_t temp, temp_init;									//温度变量
uint16_t temp_max	= 500;									//温度阈值	
uint8_t voice_flag1,mode;//语音播报标志位，模式标志位
uint16_t ms, s, min;											//充电时长变量

//串口2的数据获取
uint8_t uart2_value;      //串口传的单个数据
//串口的储存数组，串口的接收时间，串口存值的数量
uint8_t uart2_buf[36],uart2_time,uart2_num;
uint8_t uart2_rx_flag;//串口的获取值的标志位
/* RFID相关变量 */
uint8_t Temp[4],UID[4];
uint8_t const DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	//校验密码
uint8_t RC522_Buf[16];										//RC522接收数据存储数组
uint8_t User_Name[6];											//用户数组
uint8_t User_Num;
uint8_t Card_flag;												//卡标志位
uint16_t RC522_time;
uint8_t flag_err_card;
uint8_t cnt=1;
uint16_t flag_card;												//卡模式标志位
uint8_t add_id = 1;												//添加的卡号

uint8_t add_money = 30;										//充值金额
uint8_t sub_money = 10;										//消耗金额
uint8_t df_money = 3;                     //
uint8_t dis_money;												//显示的消耗金额
uint8_t price = 1;												//单价

uint8_t flag_switch;											//开关继电器标志位
uint8_t flag_clean;												//清屏标志位

uint16_t set_time,set_time1=1;								//设定的时长
uint8_t time_start;										//计时标志位
uint16_t Relay_ms, Relay_s, Relay_min;		//时间计时变量
uint16_t uart_ms,uart_2s;                //蓝牙发送数据计时
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/****
*******按键设置函数
*****/
void Key_function(void)
{
	key_num = Chiclet_Keyboard_Scan();	//按键扫描
	if(key_num != 0)										//有按键按下
	{
		switch(key_num)
		{
			case 1:													//按键1，切换模式
				if(flag_display != 5)
				{
					(flag_display == 4) ? (flag_display = 0) :(flag_display++);
					OLED_Clear();									//按一下，清屏一次
				}
			
				if(flag_display == 0)
					flag_card = 0;
				else if(flag_display == 1)
					flag_card = 1;
				if(flag_display == 0 && flag_stop == 1)
					Oled_ShowCHinese(24, 6, (uint8_t *)"请刷卡支付");
			break;

			case 2:													//按键2
				if(flag_display == 0)
				{
					mode==1?mode=0:(mode=1);
				}
				if(flag_display == 4)
				{
					OLED_Clear();
					(flag_card == 1) ? (flag_card = 2) : (flag_card = 1);
				}
				if(flag_display == 2 && set_time1 <60)
					set_time1++;
				if(flag_display == 3 && df_money <100)
					df_money++;
				if(flag_display == 5 && add_money < 200)
					add_money+=10;
			break;

			case 3:													//按键3
				if(flag_display == 0)
				{
					if(mode==0)
					{
						time_start=1;
						flag_switch=1;
					}
					else
					{
						OLED_Clear();
						flag_display=6;
					}
				}
				if(flag_display == 2 && set_time1 >0)
					set_time1--;
				if(flag_display == 3 && df_money >0)
					df_money--;
				if(flag_display == 4 && flag_card == 1)
					(add_id == 3) ? (add_id = 1) : (add_id++);
				if(flag_display == 5 && add_money > 10)
					add_money-=10;
			break;	
				
			case 4:													//按键4
				OLED_Clear(); 
				(flag_display == 0) ? (flag_display = 5) : (flag_display = 0);
			
				if(flag_display == 0)
						flag_card = 0;
				else if(flag_display == 5)
				{
					flag_card = 3;
					add_money = 30;
				}
				if(flag_display == 0 && flag_stop == 1)
					Oled_ShowCHinese(24, 6, (uint8_t *)"请刷卡支付");
			break;

			default:
			break;
		}
	}
}

/****
*******监测函数
*****/
void Monitor_function(void)
{
	if(time_500ms == 1)																								//每500ms执行一次
	{
		time_500ms = 0;
		HAL_ADC_Start(&hadc1);																					//获取电压
		if(HAL_ADC_PollForConversion(&hadc1, 999) == HAL_OK)
			voltage = HAL_ADC_GetValue(&hadc1)/4096.00*330*2;
		HAL_ADC_Stop(&hadc1);
		
		battery = voltage/100.00;													//获取电流

		
		if((min%1 == 0) && s == 0)																				//计算充电费用，每1分钟起算，不满1分钟按1分钟计算
			dis_money = min/1 * price;
		else
			dis_money = (min/1+1)*price;
		
		set_time=df_money/price;
	}
	if(uart_2s == 1)                      
  {
    uart_2s = 0;
    u2_printf("充电状态:%d 电压:%.2fV 电流:%dA 模式:%d 余额:%d 定时时间:%d 定费金额:%d 单价:%d\r\n",flag_switch,voltage/100.00,battery,mode,RC522_Buf[1],set_time1,df_money,price);
  }
}

/****
*******显示函数
*****/
void Display_function(void)
{
	switch(flag_display)								//根据不同的显示模式标志位，显示不同
	{
		case 0:		
			if(flag_stop == 0)
			{
				Oled_ShowCHinese(0, 0, (uint8_t *)"电池电压：");
				OLED_Show_Voltage(80, 0, voltage);
				Oled_ShowCHinese(0, 2, (uint8_t *)"电池电流：");
				OLED_ShowNum(80, 2, battery, 2, 16);
				Oled_ShowString(96, 2, (uint8_t *)"A");
				if(flag_switch==1)
					Oled_ShowCHinese(104, 2, (uint8_t *)"充");
				else
					Oled_ShowString(104, 2, (uint8_t *)"  ");
				Oled_ShowCHinese(0, 4, (uint8_t *)"充电时长");			
				OLED_ShowNum(64, 4, min/60, 2, 16);
				Oled_ShowString(80, 4, (uint8_t *)":");
				OLED_ShowNum(88, 4, min%60, 2, 16);
				Oled_ShowString(104, 4, (uint8_t *)":");
				OLED_ShowNum(112, 4, s, 2, 16);
				if(mode==0)
				{
					Oled_ShowCHinese(0, 6, (uint8_t *)"定时");	
          Oled_ShowString(32, 6, (uint8_t *)":");					
				  OLED_ShowNum(40, 6, set_time1, 2, 16);
				}
				else
				{
					Oled_ShowCHinese(0, 6, (uint8_t *)"定费");
          Oled_ShowString(32, 6, (uint8_t *)":");					
				  OLED_ShowNum(40, 6, df_money, 2, 16);
				}
				Oled_ShowCHinese(72, 6, (uint8_t *)"单价");
				Oled_ShowString(104, 6, (uint8_t *)":");					
				OLED_ShowNum(112, 6, price, 2, 16);
			}
			else
			{
				Oled_ShowCHinese(32, 0, (uint8_t *)"充电结束");
				Oled_ShowCHinese(24, 3, (uint8_t *)"费用：");
				OLED_Show_RMB(72, 3, sub_money);
			}
		break;
			
		case 1:	
			Oled_ShowCHinese(8, 6, (uint8_t *)"卡内余额：");
			OLED_Show_RMB(88, 6, RC522_Buf[1]);	
		break;
		
		case 2:	
			Oled_ShowCHinese(16, 1, (uint8_t *)"设置定时时间");		
			OLED_ShowNum(56, 4, set_time1,2,16);
		break;
		
		case 3:	
			Oled_ShowCHinese(16, 1, (uint8_t *)"设置定费金额");		
			OLED_ShowNum(56, 4, df_money,2,16);
		break;

		case 4:	
			if(flag_card == 1)
			{
				Oled_ShowCHinese(32, 1, (uint8_t *)"写入卡片");		
				OLED_ShowNum(60, 4, add_id, 1, 16 );		
			}
			if(flag_card == 2)
				Oled_ShowCHinese(32, 3, (uint8_t *)"删除卡片");	
		break;

		case 5:	
			Oled_ShowCHinese(32, 1, (uint8_t *)"充值卡片");		
			OLED_ShowNum(52, 4, add_money, 3, 16 );	
		break;
		
		case 6:	
			Oled_ShowCHinese(32, 1, (uint8_t *)"定费金额");
			OLED_Show_RMB(52, 4, df_money);	
		break;
	}
}


/****
*******处理函数
*****/
void Manage_function(void)
{
	SWITCH(flag_switch);
	
	if(PcdRequest(0x52,Temp)==MI_OK)//寻卡，成功后Temp数组为卡类型
	{
		if(PcdAnticoll(UID)==MI_OK)//防冲突，UID数组数据为卡序列号
		{
			if(PcdSelect(UID) == MI_OK)//选择该序列号卡
			{
				if(PcdAuthState(0x60, 2,  (unsigned char *)DefaultKey,UID) == MI_OK)//校验卡密码
				{
					if(PcdRead(2, RC522_Buf) == MI_OK)//读取块2中当前数据
					{
						if(flag_card==0 && flag_stop == 1)									
						{
							if(RC522_Buf[0] == 1 || RC522_Buf[0] == 2 || RC522_Buf[0] == 3)					//识别到是卡1、卡2、卡3
							{				
								if(RC522_Buf[1] >= sub_money)
								{
									RC522_Buf[1] -= sub_money;
									memcpy(RC522_Buf + 9, &cnt, 6);		//将数据写入卡内
									User_Num = 0;
									PcdWrite(2, RC522_Buf);
									
									OLED_Clear2();
									Oled_ShowCHinese(8, 6, (uint8_t *)"卡内余额：");
									OLED_Show_RMB(88, 6, RC522_Buf[1]);
									
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									HAL_Delay(50);
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									
									HAL_Delay(3000);
									OLED_Clear();
									flag_stop = 0;
									voice_flag1=0;
								}
								else
								{
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									HAL_Delay(50);
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									
									OLED_Clear2();
									Oled_ShowCHinese(8, 6, (uint8_t *)"余额不足请充值");
								}
								
							}
						}
						if(flag_card==0&& mode==1)
						{
							if(RC522_Buf[0] == 1 || RC522_Buf[0] == 2 || RC522_Buf[0] == 3)					//识别到是卡1、卡2、卡3
							{				
								if(RC522_Buf[1] >= df_money)
								{
									RC522_Buf[1] -= df_money;
									memcpy(RC522_Buf + 9, &cnt, 6);		//将数据写入卡内
									User_Num = 0;
									PcdWrite(2, RC522_Buf);
									
									OLED_Clear2();
									Oled_ShowCHinese(8, 6, (uint8_t *)"卡内余额：");
									OLED_Show_RMB(88, 6, RC522_Buf[1]);
									
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									HAL_Delay(50);
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									
									HAL_Delay(3000);
									OLED_Clear();
									flag_stop = 0;
									voice_flag1=0;
									time_start=1;
									flag_switch=1;
									flag_display=0;
									if(voice_flag1==0)
									{
										voice_flag1=1;
										u1_printf("%c%c%c%c%c", 0xAA, 0X55, 0x01, 0x55, 0XAA);
									}
								 }
								else
								{
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									HAL_Delay(50);
									BEEP(1);
									HAL_Delay(100);
									BEEP(0);
									
									OLED_Clear2();
									Oled_ShowCHinese(8, 6, (uint8_t *)"余额不足请充值");
								}
							}
						}
						if(flag_display==1)
						{
							Oled_ShowCHinese(8, 6, (uint8_t *)"卡内余额：");
							OLED_Show_RMB(88, 6, RC522_Buf[1]);
							
							BEEP(1);
							HAL_Delay(100);
							BEEP(0);
							HAL_Delay(50);
							BEEP(1);
							HAL_Delay(100);
							BEEP(0);
							
							HAL_Delay(3000);
							OLED_Clear();
							flag_display=0;
						}
						//添加或者注销卡
						else if(flag_card == 1 || flag_card == 2 || flag_card == 3)																				
						{												
							if(flag_card == 1)	//写入卡
							{
								RC522_Buf[0] = add_id;
								memcpy(RC522_Buf + 9, &cnt, 6);		//将数据写入卡内
								User_Num = 0;
								PcdWrite(2, RC522_Buf);
								BEEP(1);
								HAL_Delay(500);
								BEEP(0);
								OLED_Clear(); 
								flag_card = 0;
								flag_display = 0;
								if(flag_stop == 1)
									Oled_ShowCHinese(24, 6, (uint8_t *)"请刷卡支付");
							}
							else if(flag_card == 2)	//删除卡
							{
								memset(RC522_Buf, 0, sizeof(RC522_Buf));
								memcpy(RC522_Buf + 9, &cnt, 6);		//将数据写入卡内
								User_Num = 0;
								PcdWrite(2, RC522_Buf);
								BEEP(1);
								HAL_Delay(500);
								BEEP(0);
								OLED_Clear(); 
								flag_card = 0;
								flag_display = 0;
								if(flag_stop == 1)
									Oled_ShowCHinese(24, 6, (uint8_t *)"请刷卡支付");
							}
							else if(flag_card == 3)		//充值卡
							{
								if((RC522_Buf[1]+add_money) <= 250)
									RC522_Buf[1] += add_money;
								else
									RC522_Buf[1] = 250;
								
								memcpy(RC522_Buf + 9, &cnt, 6);		//将数据写入卡内
								User_Num = 0;
								PcdWrite(2, RC522_Buf);
								BEEP(1);
								HAL_Delay(500);
								BEEP(0);
								OLED_Clear(); 
								flag_card = 0;
								flag_display = 0;
								if(flag_stop == 1)
									Oled_ShowCHinese(24, 6, (uint8_t *)"请刷卡支付");
							}
						}
					}
				}
			}
		}
	}
	
	if(time_start==2&& flag_stop == 0&&mode==0)
	{
		time_start=0;
		flag_switch=0;
		voice_flag1=0;
		OLED_Clear();
		flag_stop = 1;
		sub_money = dis_money;
		Oled_ShowCHinese(24, 6, (uint8_t *)"请刷卡支付");
		if(voice_flag1==0)
		{
			voice_flag1=1;
			u1_printf("%c%c%c%c%c", 0xAA, 0X55, 0x02, 0x55, 0XAA);
		}
	}
	
	if(time_start==2&& flag_stop == 0&&mode==1)
	{
		time_start=0;
		flag_switch=0;
		voice_flag1=0;
		if(voice_flag1==0)
		{
			voice_flag1=1;
			u1_printf("%c%c%c%c%c", 0xAA, 0X55, 0x02, 0x55, 0XAA);
		}
	}
		
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);										//定时器1中断开启
	HAL_UART_Receive_IT(&huart2, &uart2_value, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  OLED_Init();																			//显示屏初始化																		
  OLED_Clear();         														//清屏
	
	/* RFID相关初始化 */
	RC522_GPIO_Init();
	PcdReset();
	PcdAntennaOff(); 
	HAL_Delay(5);
	PcdAntennaOn();  
	M500PcdConfigISOType('A');
	
  while (1)
  {
    Key_function();							//按键函数
		Monitor_function();					//监测函数
		Display_function();					//显示函数
		Manage_function();					//处理函数
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)		//定时器1触发中断
	{
		time_1ms++;
		if(time_1ms>= 500)										//计时500ms
		{
			time_1ms= 0;
			time_500ms = 1;
		}
		
		if(time_start == 1)											//开始计时
    {
      Relay_ms++;
      if(Relay_ms >= 1000)
      {
        Relay_ms = 0;
        Relay_s++;
      }
      if(Relay_s >= 60)
      {
        Relay_s = 0;
        Relay_min++;
      }
			if(mode==0)
			{	
				if(Relay_min >= set_time1)
				{
					Relay_min = 0;
					time_start=2;
				}
			}
			else
			{
				if(Relay_min >= set_time)
				{
					Relay_min = 0;
					time_start=2;
				}
			}
    }
    else
    {
      Relay_ms = 0;
      Relay_s = 0;
      Relay_min = 0;
    }
		
		if(flag_switch)												//充电计时
		{
			ms++;
			if(ms >= 1000)
			{
				ms = 0;
				s++;
			}
			if(s >= 60)
			{
				s = 0;
				min++;
			}
			if(min >= 5999)
				min = 5999;
		}
		else
		{
			ms = 0;
			s = 0;
			min = 0;
		}
		 uart_ms++;
		if(uart_ms>= 2000)
		{
			uart_ms= 0;
			uart_2s = 1;											//2s标志位
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart2.Instance)//串口1触发中断
	{
		HAL_UART_Receive_IT(&huart2, &uart2_value, 1);				//对蓝牙串口接收到的字符进行判断，
		switch(uart2_value)
		{
			case '1':
				price = 1;
			break;
			
			case '2':
				price = 2;
			break;
			
			case '3':
				price = 3;
			break;
			
			case '4':
				price = 4;
			break;
			
			case '5':
				price = 5;
			break;
			
			case '6':
				price = 6;
			break;
			
			case '7':
				mode = 0;
			break;
			
			case '8':
				mode = 1;
			break;
		}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
