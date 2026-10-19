#include "link.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"




// 控温变量
extern float target_temperature;
extern float current_ambient_temp;   
extern volatile uint8_t system_stopped;  

// 声明外部全局变量
extern float target_temperature;
extern float current_avg_temp;          // 新增：当前平均温度（取自 L3 和 L4 的平均值）

// 串口接收相关变量
static uint8_t uart_rx_buffer[16];
static uint16_t uart_rx_index = 0;
static uint8_t uart_rx_flag = 0;
static uint8_t uart_rx_byte;

// 超时检测变量
static uint32_t last_rx_time = 0;
#define RX_TIMEOUT_MS 100

// 响应缓冲区
char response_buffer[32] = {0};

// 蓝牙始终在线
static const uint8_t bluetooth_enabled = 1;

// 目标修改标记
static uint8_t target_changed = 0;

// 初始化
void LINK_Init(void)
{
    UART_Receive_Init();
}

void UART_Receive_Init(void)
{
    HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
}

// 解析 SETxxTMP 命令
static uint8_t ParseSetCommand(char *cmd, uint8_t *temp)
{
    if (strncmp(cmd, "SET", 3) != 0)
        return 0;

    if (strlen(cmd) < 8)
        return 0;

    char num_str[3] = {cmd[3], cmd[4], '\0'};
    if (num_str[0] < '0' || num_str[0] > '9' || num_str[1] < '0' || num_str[1] > '9')
        return 0;

    uint8_t value = (num_str[0] - '0') * 10 + (num_str[1] - '0');

				// 温度上限
				// 温度上限
				// 温度上限
				// 温度上限
    if (value > 56)
			

        return 0;

    if (strncmp(cmd + 5, "TMP", 3) != 0)
        return 0;

    *temp = value;
    return 1;
}

// 接收中断回调
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2 && bluetooth_enabled)
    {
        char received_char = uart_rx_byte;
        if (received_char >= 'a' && received_char <= 'z') {
            received_char = received_char - 'a' + 'A';
        }

        last_rx_time = HAL_GetTick();

        if ((received_char >= 'A' && received_char <= 'Z') ||
            (received_char >= '0' && received_char <= '9') ||
            received_char == '\r' || received_char == '\n')
        {
            if (uart_rx_index < sizeof(uart_rx_buffer) - 1)
            {
                uart_rx_buffer[uart_rx_index++] = received_char;

                if (received_char == '\r' || received_char == '\n' ||
                    uart_rx_index >= sizeof(uart_rx_buffer) - 1)
                {
                    uart_rx_buffer[uart_rx_index] = '\0';
                    uart_rx_flag = 1;
                }
            }
            else
            {
                uart_rx_index = 0;
                memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));
            }
        }

        HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    }
}

// 超时检查
void UART_CheckTimeout(void)
{
    if (uart_rx_index > 0 && !uart_rx_flag)
    {
        uint32_t now = HAL_GetTick();
        if (now - last_rx_time > RX_TIMEOUT_MS)
        {
            uart_rx_buffer[uart_rx_index] = '\0';
            uart_rx_flag = 1;
        }
    }
}




// 标记目标被修改
void LINK_SetTargetChanged(void)
{
    target_changed = 1;
}

// 获取并清除标记
uint8_t LINK_GetAndClearTargetChanged(void)
{
    uint8_t ret = target_changed;
    target_changed = 0;
    return ret;
}

// 发送恒温失败
void LINK_SendUnstable(void)
{
    const char* msg = "UNSTABLE\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

// 发送恒温成功（带温度）
void LINK_SendStable(float temp)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "STABLE %.1f C\r\n", temp);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
}

// 清除目标修改标记
void LINK_ClearTargetChanged(void)
{
    target_changed = 0;
}



void UART_ProcessReceivedData(void)
{
    UART_CheckTimeout();

    if (uart_rx_flag && bluetooth_enabled)
    {
        // 将接收到的字符串转为大写（便于比较）
        for (int i = 0; i < uart_rx_index; i++) {
            if (uart_rx_buffer[i] >= 'a' && uart_rx_buffer[i] <= 'z')
                uart_rx_buffer[i] -= 32;
        }

        // 1. 检查 WHO 命令（设备识别）
        if (strncmp((char*)uart_rx_buffer, "WHO", 3) == 0) {
            const char *ack = "TCONS\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t*)ack, strlen(ack), 100);
        }
        // 2. 检查 STOP 命令
        else if (strncmp((char*)uart_rx_buffer, "STOP", 4) == 0) {
            system_stopped = 1;
            const char *ack = "STOPPED\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t*)ack, strlen(ack), 100);
        }
        // 3. 检查 SETxxTMP 命令
        else {
            uint8_t temp_value = 0;
            if (ParseSetCommand((char*)uart_rx_buffer, &temp_value))
            {
                target_temperature = (float)temp_value;
                LINK_SetTargetChanged();
                
                // 如果之前处于 STOP 状态，现在收到新目标温度，则清除停止标志并恢复输出
                if (system_stopped) {
                    system_stopped = 0;
                }

                snprintf(response_buffer, sizeof(response_buffer),
                         "TARGET=%d CURRENT:%.1f\r\n", temp_value, current_avg_temp);
                HAL_UART_Transmit(&huart2, (uint8_t*)response_buffer, strlen(response_buffer), 100);
            }
            else
            {
                const char *err_msg = "ERR:INVALID\r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)err_msg, strlen(err_msg), 100);
            }
        }

        uart_rx_flag = 0;
        uart_rx_index = 0;
        memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));
    }
}









