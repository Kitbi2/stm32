 
 #include  "stm32f1xx_hal.h"
#include "led_driver.h"
#include "USBtoComApp.h"
#include <stdio.h>


typedef struct
{
	UART_HandleTypeDef USBtoComHuart;
	DMA_HandleTypeDef   USBBtoComDMA;
	GPIO_TypeDef * UART_TX_PRO;   
	uint16_t  UART_TX_PIN;
	GPIO_TypeDef * UART_RX_PRO;
	uint16_t  UART_RX_PIN;
	void (*UsbtoComapp)(uint8_t*,uint32_t);
	uint8_t re_buf[1024];

}UsbtoCom_t;

static UsbtoCom_t USBcom;
														

/**
  * role:初始化串口的GPIO 
	* parm:void
	* ret:void
	*
	*
  **/
void USBtoCom_GPIOInit(void)
{
	USBcom.UART_RX_PRO =  GPIOA;
	USBcom.UART_RX_PIN =  GPIO_PIN_10;
	USBcom.UART_TX_PRO =  GPIOA;
	USBcom.UART_TX_PIN =  GPIO_PIN_9;
	/* 初始化GPIO */
	/* 使能时钟和配置为推挽输出模式 */
	GPIO_CLK_ENABLE(USBcom.UART_TX_PRO);
	GPIO_InitTypeDef tmp;
	tmp.Mode = GPIO_MODE_AF_PP;
	tmp.Pin = USBcom.UART_TX_PIN;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(USBcom.UART_TX_PRO,&tmp);
	
	/* 使能时钟和配置为上拉输入模式 */
	GPIO_CLK_ENABLE(USBcom.UART_RX_PRO);
	
	tmp.Mode = GPIO_MODE_INPUT;
	tmp.Pin = USBcom.UART_RX_PIN;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	tmp.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(USBcom.UART_RX_PRO,&tmp);	
}
/**
  * role:初始化串口的属性
	* parm:blk----波特率
	* ret:void
	*
	*
  **/
void USBtoComUsart_UsartInit(uint32_t blk)
{
	USBcom.UsbtoComapp = USBtoComApp;
	/* 使能uart1的时钟 */
	__HAL_RCC_USART1_CLK_ENABLE();
	
	/* 配置uart1 */
	USBcom.USBtoComHuart.Instance = USART1;   /* 要使用的外设 */
	USBcom.USBtoComHuart.Init.BaudRate = blk;  /* 波特率 */
	USBcom.USBtoComHuart.Init.StopBits = UART_STOPBITS_1; /* 停止位 */
	USBcom.USBtoComHuart.Init.WordLength = UART_WORDLENGTH_8B; /* 字长 */
	USBcom.USBtoComHuart.Init.Parity = UART_PARITY_NONE; /* 不适用校验 */
	USBcom.USBtoComHuart.Init.Mode = UART_MODE_TX_RX;  /* 使用发送与接受 */
	
	HAL_UART_Init(&USBcom.USBtoComHuart);
	/* 开启空闲总线中断 */
	__HAL_UART_ENABLE_IT(&USBcom.USBtoComHuart,UART_IT_IDLE);
	
	HAL_NVIC_SetPriority(USART1_IRQn,1,1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
	/* 进行unlink操作 */
	__HAL_LINKDMA(&USBcom.USBtoComHuart,hdmarx,USBcom.USBBtoComDMA);
	/* 使能DMA接受 */
	HAL_UART_Receive_DMA(&USBcom.USBtoComHuart,USBcom.re_buf,1024);
}

/**
  * role:初始化串口的DMA
	* parm:blk----波特率
	* ret:void
	*
	*
  **/

void USBtoCom_DMA(void)
{
	/* 使能DMA的时钟 */
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* 配置DMA1 */
	USBcom.USBBtoComDMA.Instance = DMA1_Channel5;
	USBcom.USBBtoComDMA.Init.Direction = DMA_PERIPH_TO_MEMORY; /* 外设到内存 */
	USBcom.USBBtoComDMA.Init.PeriphInc = DMA_PINC_DISABLE;  /* 外设不自增 */
	USBcom.USBBtoComDMA.Init.MemInc = DMA_MINC_ENABLE;      /* 内存自增 */
	USBcom.USBBtoComDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* 内存为8位 */
	USBcom.USBBtoComDMA.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    /* 外设也是8位 */
	USBcom.USBBtoComDMA.Init.Mode = DMA_NORMAL; /* 非循环模式 */
	USBcom.USBBtoComDMA.Init.Priority = DMA_PRIORITY_VERY_HIGH;  /* 优先级非常高 */
	
	HAL_DMA_Init(&USBcom.USBBtoComDMA);
} 


/**
  * role:初始化串口
	* parm:blk---波特率
	* ret:void
	*
	*
  **/
void USBtoComUsart_Init(uint32_t blk)
{

	USBtoCom_GPIOInit();
	USBtoCom_DMA();
	USBtoComUsart_UsartInit(blk);
	
	
}


void USART1_IRQHandler(void)
{
	

	/* 获取标志位,是否为空闲中断标志 */
	if(__HAL_USART_GET_FLAG(&USBcom.USBtoComHuart, USART_FLAG_IDLE))
	{
		__HAL_USART_CLEAR_IDLEFLAG(&USBcom.USBtoComHuart);
		HAL_UART_DMAStop(&USBcom.USBtoComHuart);
		uint16_t data_len = 1024 - __HAL_DMA_GET_COUNTER(&USBcom.USBBtoComDMA);
		USBcom.UsbtoComapp(USBcom.re_buf,data_len);
		HAL_UART_Receive_DMA(&USBcom.USBtoComHuart,USBcom.re_buf,1024);
		
	}
	
  	
		
}


int fputc(int ch, FILE *f)
{
	uint8_t temp[1] = {ch};
	HAL_UART_Transmit(&USBcom.USBtoComHuart, temp, 1, 2);//huart1??????????
	return ch;
}


void USBtocomtest(void)
{
	
	for(uint8_t i = 0;i < 250;i++)
	{
		HAL_UART_Transmit(&USBcom.USBtoComHuart,&i,1,0xff);
	}
}



