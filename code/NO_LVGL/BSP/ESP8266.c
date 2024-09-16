#include "stm32f1xx_hal.h"



#include "ESP8266.h"
#include "delay.h"
#include "led_driver.h"

#include <string.h>
#include <stdio.h>


#define WIFI_COM_OK     0
#define WIFI_COM_WAIT   1


char RecvWifiRetbuf[200] = {'\0'};
UART_HandleTypeDef huart3_handle;
DMA_HandleTypeDef  dam1_3_handle;

/**
*******************************
* @brief ： 初始化ESP8266依赖的gpio
* @parm  ：void
* @return ：void
*******************************
*/
static void ESP8266_GPIOInit(void)
{
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_USART3_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	/* 初始化GPIO */
	/* 使能时钟和配置为推挽输出模式 */
	GPIO_InitTypeDef tmp;
	tmp.Mode = GPIO_MODE_AF_PP;
	tmp.Pin = GPIO_PIN_8;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD,&tmp);
	
	/* 使能时钟和配置为上拉输入模式 */
	
	tmp.Mode = GPIO_MODE_INPUT;
	tmp.Pin = GPIO_PIN_9;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	tmp.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD,&tmp);	

	
}

/**
*******************************
* @brief ： 初始化ESP8266依赖的dma
* @parm  ：void
* @return ：void
*******************************
*/
static void ESP8266_dmaInit(void)
{
	/* 使能DMA的时钟 */
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* 配置DMA1 */
	dam1_3_handle.Instance = DMA1_Channel3;
	dam1_3_handle.Init.Direction = DMA_PERIPH_TO_MEMORY; /* 外设到内存 */
	dam1_3_handle.Init.PeriphInc = DMA_PINC_DISABLE;  /* 外设不自增 */
	dam1_3_handle.Init.MemInc = DMA_MINC_ENABLE;      /* 内存自增 */
	dam1_3_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* 内存为8位 */
	dam1_3_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    /* 外设也是8位 */
	dam1_3_handle.Init.Mode = DMA_NORMAL; /* 非循环模式 */
	dam1_3_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;  /* 优先级非常高 */
	
	HAL_DMA_Init(&dam1_3_handle);
}
/**
*******************************
* @brief ： 初始化ESP8266依赖的uart
* @parm  ：void
* @return ：void
*******************************
*/
static void ESP8266_UARTInit(void)
{



	
	/* 使能uart1的时钟 */
	__HAL_RCC_USART3_CLK_ENABLE();
	/* 配置uart1 */
	huart3_handle.Instance = USART3;   /* 要使用的外设 */
	huart3_handle.Init.BaudRate = 115200;  /* 波特率 */
	huart3_handle.Init.StopBits = UART_STOPBITS_1; /* 停止位 */
	huart3_handle.Init.WordLength = UART_WORDLENGTH_8B; /* 字长 */
	huart3_handle.Init.Parity = UART_PARITY_NONE; /* 不适用校验 */
	huart3_handle.Init.Mode = UART_MODE_TX_RX;  /* 使用发送与接受 */
	huart3_handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart3_handle);

	
	HAL_NVIC_SetPriority(USART3_IRQn,1,1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

  __HAL_UART_ENABLE_IT(&huart3_handle,UART_IT_IDLE);


	/* 进行unlink操作 */
	__HAL_LINKDMA(&huart3_handle,hdmarx,dam1_3_handle);
	/* 使能DMA接受 */
	
	
	HAL_UART_Receive_DMA(&huart3_handle,(uint8_t *)RecvWifiRetbuf,200);
}

/**
*******************************
* @brief ： 通过usart3发送数据给esp8266
* @parm  ：buf---数据缓冲区
           len---数据长度
* @return ：void
*******************************
*/
void ESP8266_Write(char *buf, int len)
{
	

	HAL_UART_Transmit(&huart3_handle,(uint8_t *)buf,len,0XFFFF);

}
/**
*******************************
* @brief ： 获取eps8266的返回数据
* @parm  ：	void
						
* @return ：char --- 返回数据的地址
*******************************
*/
char * EPS8266_READ(uint16_t * len)
{
	*len = strlen(RecvWifiRetbuf);
	return RecvWifiRetbuf;
}
/**
*******************************
* @brief ： 清空缓冲区
* @parm  ：	void
						
* @return ：char --- 返回数据的地址
*******************************
*/
void EPS8266_Clear(void)
{
		memset(&RecvWifiRetbuf[0],0,200);

}

void ESP8266_Init(void)
{
	ESP8266_GPIOInit();
	ESP8266_dmaInit();
	ESP8266_UARTInit();

	

}

void USART3_IRQHandler(void)
{
	

	if(__HAL_UART_GET_FLAG(&huart3_handle,UART_FLAG_IDLE))
	{
		/* 这里标志位 */
		__HAL_UART_CLEAR_FLAG(&huart3_handle,UART_FLAG_IDLE);
		
		
		HAL_UART_DMAStop(&huart3_handle);
		uint16_t data_len = 200 - __HAL_DMA_GET_COUNTER(&dam1_3_handle);
		memset(&RecvWifiRetbuf[data_len],0,200 - data_len);
		HAL_UART_Receive_DMA(&huart3_handle,(uint8_t *)RecvWifiRetbuf,200);
		
		
		
	}
	
	
}



