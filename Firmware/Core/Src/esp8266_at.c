#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp8266_at.h"

#define RX_BUFFER_SIZE 128

extern UART_HandleTypeDef huart2;

uint8_t usr_data[RX_BUFFER_SIZE];
uint8_t rx_data[RX_BUFFER_SIZE];
uint16_t rx_len;
uint16_t usr_len;

osThreadId_t recv_task_handle;
osThreadId_t usr_task_handle;
osSemaphoreId_t recv_sem;
osSemaphoreId_t usr_sem;

struct esp8266_at my_handle;

static int esp8266_at_config(esp8266_at_t handle);
static void rx_data_proc(void);
static void wait_to_ok(void);

int esp8266_at_init(void)
{
	memset(&my_handle, 0, sizeof(my_handle));
	my_handle.config = esp8266_at_config;
	return 0;
}

static int esp8266_at_config(esp8266_at_t handle)
{
	const char *at_cwmode_cmd = "AT+CWMODE=3\r\n";
	const char *at_cwjap_cmd = "AT+CWJAP=\"Deadline Driven\",\"ykc2662350117\"\r\n";
	const char *at_cfg_cmd = "AT+MQTTUSERCFG=0,1,\"NULL\",\"dev_1&a1n97WMOcWB\",\"6e007fb9789cf6cbf1f0a33ab4a17a34d778788601b03da03d6abbac0fca37a4\",0,0,\"\"\r\n";
	const char *at_client_cmd = "AT+MQTTCLIENTID=0,\"a1n97WMOcWB.dev_1|securemode=2\\,signmethod=hmacsha256\\,timestamp=1683679787004|\"\r\n";
	const char *at_conn_cmd = "AT+MQTTCONN=0,\"a1n97WMOcWB.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n";
	const char *at_sub_cmd = "AT+MQTTSUB=0,\"/a1n97WMOcWB/dev_1/user/get\",1\r\n";
	
	HAL_UART_Transmit(&huart2, (uint8_t *)at_cwmode_cmd, strlen(at_cwmode_cmd), HAL_MAX_DELAY);
	wait_to_ok();
	handle->is_ok = 0;
	
	HAL_UART_Transmit(&huart2, (uint8_t *)at_cwjap_cmd, strlen(at_cwjap_cmd), HAL_MAX_DELAY);
	wait_to_ok();
	handle->is_ok = 0;
	
	HAL_UART_Transmit(&huart2, (uint8_t *)at_cfg_cmd, strlen(at_cfg_cmd), HAL_MAX_DELAY);
	wait_to_ok();
	handle->is_ok = 0;
	
	HAL_UART_Transmit(&huart2, (uint8_t *)at_client_cmd, strlen(at_client_cmd), HAL_MAX_DELAY);
	wait_to_ok();
	handle->is_ok = 0;
	
	HAL_UART_Transmit(&huart2, (uint8_t *)at_conn_cmd, strlen(at_conn_cmd), HAL_MAX_DELAY);
	wait_to_ok();
	handle->is_ok = 0;
	
	HAL_UART_Transmit(&huart2, (uint8_t *)at_sub_cmd, strlen(at_sub_cmd), HAL_MAX_DELAY);
	wait_to_ok();
	handle->is_ok = 0;
	
	return 0;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
	{
		usr_len = Size;
		osSemaphoreRelease(usr_sem);
		HAL_UARTEx_ReceiveToIdle_IT(huart, usr_data, RX_BUFFER_SIZE);
	}
	
	if (huart->Instance == USART2)
	{
		rx_len = Size;
		osSemaphoreRelease(recv_sem);
		HAL_UARTEx_ReceiveToIdle_IT(huart, rx_data, RX_BUFFER_SIZE);
	}
}

void recv_task(void *argument)
{	
	while (1)
	{
		osSemaphoreAcquire(recv_sem, osWaitForever);
		rx_data[rx_len] = '\0';
		printf("[ESP_MSG]:\r\n%s\r\n", rx_data);
		rx_data_proc();
	}
}

void usr_task(void *argument)
{
	while (1)
	{
		osSemaphoreAcquire(usr_sem, osWaitForever);
		HAL_UART_Transmit(&huart2, usr_data, usr_len, HAL_MAX_DELAY);
	}
}

static void rx_data_proc(void)
{
	const char *ok_line = strstr((const char *)rx_data, "OK");
	const char *get_line = strstr((const char *)rx_data, "get");
	int len, code;
	if (ok_line)
	{
		my_handle.is_ok = 1;
		printf("[LOG_MSG]: is_ok\r\n");
	}
	
	if (get_line)
	{
		sscanf(rx_data, "+MQTTSUBRECV:0,\"/a1n97WMOcWB/dev_1/user/get\",%d,%d", &len, &code);
		printf("[LOG_MSG]: len: %d, code: %d\r\n", len, code);
	}
}

static void wait_to_ok(void)
{
	while (!my_handle.is_ok)
	{
		osDelay(100);
	}
}

