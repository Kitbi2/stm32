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
* @brief �� ��ʼ��ESP8266������gpio
* @parm  ��void
* @return ��void
*******************************
*/
static void ESP8266_GPIOInit(void)
{
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_USART3_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	/* ��ʼ��GPIO */
	/* ʹ��ʱ�Ӻ�����Ϊ�������ģʽ */
	GPIO_InitTypeDef tmp;
	tmp.Mode = GPIO_MODE_AF_PP;
	tmp.Pin = GPIO_PIN_8;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD,&tmp);
	
	/* ʹ��ʱ�Ӻ�����Ϊ��������ģʽ */
	
	tmp.Mode = GPIO_MODE_INPUT;
	tmp.Pin = GPIO_PIN_9;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	tmp.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD,&tmp);	

	
}

/**
*******************************
* @brief �� ��ʼ��ESP8266������dma
* @parm  ��void
* @return ��void
*******************************
*/
static void ESP8266_dmaInit(void)
{
	/* ʹ��DMA��ʱ�� */
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* ����DMA1 */
	dam1_3_handle.Instance = DMA1_Channel3;
	dam1_3_handle.Init.Direction = DMA_PERIPH_TO_MEMORY; /* ���赽�ڴ� */
	dam1_3_handle.Init.PeriphInc = DMA_PINC_DISABLE;  /* ���費���� */
	dam1_3_handle.Init.MemInc = DMA_MINC_ENABLE;      /* �ڴ����� */
	dam1_3_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* �ڴ�Ϊ8λ */
	dam1_3_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    /* ����Ҳ��8λ */
	dam1_3_handle.Init.Mode = DMA_NORMAL; /* ��ѭ��ģʽ */
	dam1_3_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;  /* ���ȼ��ǳ��� */
	
	HAL_DMA_Init(&dam1_3_handle);
}
/**
*******************************
* @brief �� ��ʼ��ESP8266������uart
* @parm  ��void
* @return ��void
*******************************
*/
static void ESP8266_UARTInit(void)
{



	
	/* ʹ��uart1��ʱ�� */
	__HAL_RCC_USART3_CLK_ENABLE();
	/* ����uart1 */
	huart3_handle.Instance = USART3;   /* Ҫʹ�õ����� */
	huart3_handle.Init.BaudRate = 115200;  /* ������ */
	huart3_handle.Init.StopBits = UART_STOPBITS_1; /* ֹͣλ */
	huart3_handle.Init.WordLength = UART_WORDLENGTH_8B; /* �ֳ� */
	huart3_handle.Init.Parity = UART_PARITY_NONE; /* ������У�� */
	huart3_handle.Init.Mode = UART_MODE_TX_RX;  /* ʹ�÷�������� */
	huart3_handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart3_handle);

	
	HAL_NVIC_SetPriority(USART3_IRQn,1,1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

  __HAL_UART_ENABLE_IT(&huart3_handle,UART_IT_IDLE);


	/* ����unlink���� */
	__HAL_LINKDMA(&huart3_handle,hdmarx,dam1_3_handle);
	/* ʹ��DMA���� */
	
	
	HAL_UART_Receive_DMA(&huart3_handle,(uint8_t *)RecvWifiRetbuf,200);
}

/**
*******************************
* @brief �� ͨ��usart3�������ݸ�esp8266
* @parm  ��buf---���ݻ�����
           len---���ݳ���
* @return ��void
*******************************
*/
void ESP8266_Write(char *buf, int len)
{
	

	HAL_UART_Transmit(&huart3_handle,(uint8_t *)buf,len,0XFFFF);

}
/**
*******************************
* @brief �� ��ȡeps8266�ķ�������
* @parm  ��	void
						
* @return ��char --- �������ݵĵ�ַ
*******************************
*/
char * EPS8266_READ(uint16_t * len)
{
	*len = strlen(RecvWifiRetbuf);
	return RecvWifiRetbuf;
}
/**
*******************************
* @brief �� ��ջ�����
* @parm  ��	void
						
* @return ��char --- �������ݵĵ�ַ
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
		/* �����־λ */
		__HAL_UART_CLEAR_FLAG(&huart3_handle,UART_FLAG_IDLE);
		
		
		HAL_UART_DMAStop(&huart3_handle);
		uint16_t data_len = 200 - __HAL_DMA_GET_COUNTER(&dam1_3_handle);
		memset(&RecvWifiRetbuf[data_len],0,200 - data_len);
		HAL_UART_Receive_DMA(&huart3_handle,(uint8_t *)RecvWifiRetbuf,200);
		
		
		
	}
	
	
}



