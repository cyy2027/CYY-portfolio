/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "ntc.h"
#include "CONS.h"
#include "MENU.h"
#include "INA226.h"
#include <math.h>
#include "link.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// 风扇调速参数
#define FAN_TEMP_DIFF_START   2.2f      // 启动温差（℃）
#define FAN_DUTY_START        30        // 启动占空比（%）
#define FAN_TEMP_DIFF_MAX     8.0f      // 最大温差（占空比达 100%）
#define FAN_DUTY_MAX          100       // 最大占空比（%）
  

    #define STABLE_ERR_THRESHOLD   0.25f   // 允许的最大误差（℃）
    #define STABLE_RATE_THRESHOLD  0.08f   // 允许的最大变化率（℃/s）
    #define STABLE_REQUIRED_SEC    10      // 需要持续稳定的秒数
// ========== 新增 VOFA+ JustFloat 曲线数据发送间隔（毫秒） ==========
//#define CURVE_SEND_INTERVAL_MS  1000     // VOFA+上位机发送间隔，建议 50ms 或 100ms，根据实际调整		
		
		
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern ADC_HandleTypeDef hadc;
extern I2C_HandleTypeDef hi2c1;

extern float current_heat_duty, current_cool_duty;

float target_temperature = 25.0f;   // 默认目标温度，上电后不自动运行，无所谓

volatile uint8_t fan_pwm_duty = 0;
float current_avg_temp = 0.0f;

extern volatile uint8_t buzzer_active;

volatile uint8_t system_stopped = 1;     // 上电后处于 STOP 状态，不输出任何功率
float current_ambient_temp = 25.0f;      // 存储最近一次读取的环境温度 (L2)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t MENU_IsAdjustMode(void);
float   MENU_GetAdjustTemp(void);
uint8_t MENU_GetBlinkState(void);
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
  MX_I2C1_Init();
  MX_ADC_Init();
  MX_TIM2_Init();
  MX_TIM21_Init();
  MX_TIM22_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
 
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   // 初始关闭风扇
  NTC_Init();
  OLED_Init();
  CONS_Init();
  INA226_Init();
  LINK_Init();  
  MENU_Init();   

  HAL_TIM_Base_Start_IT(&htim22);   // 使能 TIM22 更新中断，用于风扇 PWM 和蜂鸣器

float L1_init, L2_init, L3_init, L4_init;
NTC_ReadTemperatures(&L1_init, &L2_init, &L3_init, &L4_init, 3435);
//target_temperature = (float)((int)(L2_init + 0.5f)); // 四舍五入取整

// 初始化稳定滤波温度（使用第一次读取的温度）
float avg_temp_init = (L3_init + L4_init) / 2.0f;


  HAL_Delay(100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		
		
		
// 用于检测菜单调节模式下降沿
    static uint8_t last_adjust_mode = 0;
    uint8_t current_adjust_mode = MENU_IsAdjustMode();
    if (last_adjust_mode && !current_adjust_mode)
    {
        target_temperature = MENU_GetAdjustTemp();
    }
    last_adjust_mode = current_adjust_mode;

//NTC温度读取 
float L1, L2, L3, L4;
NTC_ReadTemperatures(&L1, &L2, &L3, &L4, 3435);
current_ambient_temp = L2;               // 更新环境温度，供 RESET 命令使用
float avg_temp = (L3 + L4) / 2.0f;

    /* OLED 显示 */
    OLED_ShowFloatNum(80, 0, L1, 2, 1, OLED_6X8);
    OLED_ShowString(115, 0, "C", OLED_6X8);
    OLED_ShowFloatNum(80, 8, L2, 2, 1, OLED_6X8);
    OLED_ShowString(115, 8, "C", OLED_6X8);

    OLED_ShowString(0, 0, "SET:", OLED_8X16);
    if (MENU_IsAdjustMode())
    {
        if (MENU_GetBlinkState())
            OLED_ShowString(33, 0, " ", OLED_8X16);
        else
            OLED_ShowNum(33, 0, MENU_GetAdjustTemp(), 2, OLED_8X16);
    }
    else
    {
        OLED_ShowNum(33, 0, target_temperature, 2, OLED_8X16);
    }
    OLED_ShowString(57, 0, "C°", OLED_8X16);

    MENU_Process();

    // 温度控制
    CONS_TempControl(target_temperature, avg_temp, L2, L1);

    OLED_ShowFloatNum(0, 16, L3, 2, 1, OLED_8X16);
    OLED_ShowFloatNum(64, 16, L4, 2, 1, OLED_8X16);

    /* 功率显示 */
    float power = INA226_ReadPowerFiltered();
    float volt = INA226_ReadBusVoltage();
    float curr = INA226_ReadCurrent();

    OLED_ShowString(55, 48, "V", OLED_8X16);
    OLED_ShowFloatNum(0, 48, volt, 2, 2, OLED_8X16);
    OLED_ShowString(120, 48, "A:", OLED_8X16);
    OLED_ShowFloatNum(74, 48, curr, 1, 2, OLED_8X16);

    OLED_ShowString(0, 32, "PW:", OLED_8X16);
    OLED_ShowFloatNum(32, 32, power, 2, 1, OLED_8X16);
    OLED_ShowString(80, 32, "W", OLED_8X16);
    OLED_ShowNum(96, 32, fan_pwm_duty, 3, OLED_6X8);








// ==================== 稳定判断（基于真实时间，独立于循环频率） ====================
    static uint32_t stable_start_ms = 0;
    static uint8_t stable_cond_met = 0;
    static float filtered_temp_1s = 0.0f;
    static uint32_t last_rate_time = 0;
    static float last_filtered_temp = 0.0f;

    float cur_error = fabsf(avg_temp - target_temperature);
    uint32_t now_ms = HAL_GetTick();

    // 每秒更新一次温度变化率（使用一阶滤波平滑温度）
    if (now_ms - last_rate_time >= 100) {           
        last_rate_time = now_ms;
        // 温度一阶滤波，时间常数约1秒
        filtered_temp_1s = 0.7f * avg_temp + 0.3f * filtered_temp_1s;
        float rate = fabsf(filtered_temp_1s - last_filtered_temp);
        last_filtered_temp = filtered_temp_1s;

        // 判断是否满足稳定条件（误差小且变化率小）
        if (cur_error < STABLE_ERR_THRESHOLD && rate < STABLE_RATE_THRESHOLD) {
            if (!stable_cond_met) {
                stable_cond_met = 1;
                stable_start_ms = now_ms;
            }
        } else {
            stable_cond_met = 0;
        }
    }

    uint8_t current_stable = 0;
    if (stable_cond_met && (now_ms - stable_start_ms) >= (STABLE_REQUIRED_SEC * 1000)) {
        current_stable = 1;
    }

		
		
		
		
		
		
		
		
    //稳定输出 + 通知 
    static uint8_t prev_stable = 0;
    if (current_stable != prev_stable) {
        if (current_stable) {
            HAL_GPIO_WritePin(CONS_GPIO_Port, CONS_Pin, GPIO_PIN_SET);
            LINK_SendStable(avg_temp);
            LINK_ClearTargetChanged();
        } else {
            HAL_GPIO_WritePin(CONS_GPIO_Port, CONS_Pin, GPIO_PIN_RESET);
            if (!LINK_GetAndClearTargetChanged()) {
                LINK_SendUnstable();
            }
        }
        prev_stable = current_stable;
    }

		


//	/*			


		
// ========== VOFA+ JustFloat 曲线数据发送 ==========
		
		
//VOFA+ JustFloat 曲线数据发送/调试信息发送 二选一，不要同时解除注释。
static uint32_t last_curve_send = 0;
uint32_t now_curve = HAL_GetTick();
if (now_curve - last_curve_send >= 500)//  发送速度 MS  if (now_curve - last_curve_send >= CURVE_SEND_INTERVAL_MS)
{
    last_curve_send = now_curve;
    
    // 修改为只需要 4 个参数：设定温度、当前温度、环境温度、散热片温度
    float curve_data[4] = {
        target_temperature,   // 设定温度
        avg_temp,             // 当前控温温度 (L3+L4平均)
        L1,                   // 环境温度
        L2                    // 散热片温度
    };
    
    uint8_t buffer[4*4 + 4];   // 4个float(16字节) + 4字节帧尾
    
    // 直接内存拷贝（STM32小端序，与JustFloat兼容）
    memcpy(buffer, curve_data, 4*4);
    
    // JustFloat 标准帧尾
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x80;
    buffer[19] = 0x7F;
    
    HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), 100);  
}
	


	//*/	




	
    /*

//调试信息发送 

//VOFA+ JustFloat 曲线数据发送/调试信息发送 二选一，不要同时解除注释。只能使用其中一个，非调试需要只需要解除这里就够了

static uint32_t last_debug_send = 0;
if (HAL_GetTick() - last_debug_send >= 3005)//每3.005秒一次
{
    last_debug_send = HAL_GetTick();

    char dbg[128];

    snprintf(dbg, sizeof(dbg),
        "Target=%5.2fC  Current=%5.2fC  Erro=%5.2fC  "
        "HSink(L1)=%5.1fC  Amb(L2)=%5.1fC  "
        "Heat=%5.1f%%  Cool=%5.1f%%  Fan=%3d%%\r\n",
        target_temperature,
        avg_temp,                                   // 当前控温值 = L3+L4平均
        fabsf(avg_temp - target_temperature),
        L1,                                         // 环境温度
        L2,                                         // 散热片温度
        current_heat_duty,
        current_cool_duty,
        fan_pwm_duty);

    HAL_UART_Transmit(&huart2, (uint8_t*)dbg, strlen(dbg), 100);
}


  */		






    // 处理串口接收数据
    UART_ProcessReceivedData();
    current_avg_temp = avg_temp;

		
		
		
		
//风扇控制 
float temp_diff = fabsf(L1 - L2);
uint8_t duty = 0;
if (system_stopped) {
    duty = 0;                           // STOP 状态强制关闭风扇
} else if (temp_diff >= FAN_TEMP_DIFF_START) {
    if (temp_diff >= FAN_TEMP_DIFF_MAX) {
        duty = FAN_DUTY_MAX;
    } else {
        duty = FAN_DUTY_START + (uint8_t)(
            (temp_diff - FAN_TEMP_DIFF_START) *
            (FAN_DUTY_MAX - FAN_DUTY_START) /
            (FAN_TEMP_DIFF_MAX - FAN_TEMP_DIFF_START)
        );
    }
}
fan_pwm_duty = duty;

    OLED_Update();
    HAL_Delay(20);
		
	
		







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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM22)
    {
        static uint16_t pwm_cnt = 0;      // PWM 周期计数器 (0~99)
        static uint16_t pb3_cnt = 0;      // PB3 方波分频计数器

        // 风扇 PWM 控制 (PB1)
        pwm_cnt++;
        if (pwm_cnt >= 100) {
            pwm_cnt = 0;
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
        }
        if (pwm_cnt == fan_pwm_duty && fan_pwm_duty < 100) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
        }

        // PB3 蜂鸣器控制（仅在 buzzer_active 时输出方波）
        if (buzzer_active) {
            pb3_cnt++;
            if (pb3_cnt >= BUZZER_FREQ_DIV) {   // 使用宏控制音调
                pb3_cnt = 0;
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
            }
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
            pb3_cnt = 0;
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
