 
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
  * role:��ʼ�����ڵ�GPIO 
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
	/* ��ʼ��GPIO */
	/* ʹ��ʱ�Ӻ�����Ϊ�������ģʽ */
	GPIO_CLK_ENABLE(USBcom.UART_TX_PRO);
	GPIO_InitTypeDef tmp;
	tmp.Mode = GPIO_MODE_AF_PP;
	tmp.Pin = USBcom.UART_TX_PIN;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(USBcom.UART_TX_PRO,&tmp);
	
	/* ʹ��ʱ�Ӻ�����Ϊ��������ģʽ */
	GPIO_CLK_ENABLE(USBcom.UART_RX_PRO);
	
	tmp.Mode = GPIO_MODE_INPUT;
	tmp.Pin = USBcom.UART_RX_PIN;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	tmp.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(USBcom.UART_RX_PRO,&tmp);	
}
/**
  * role:��ʼ�����ڵ�����
	* parm:blk----������
	* ret:void
	*
	*
  **/
void USBtoComUsart_UsartInit(uint32_t blk)
{
	USBcom.UsbtoComapp = USBtoComApp;
	/* ʹ��uart1��ʱ�� */
	__HAL_RCC_USART1_CLK_ENABLE();
	
	/* ����uart1 */
	USBcom.USBtoComHuart.Instance = USART1;   /* Ҫʹ�õ����� */
	USBcom.USBtoComHuart.Init.BaudRate = blk;  /* ������ */
	USBcom.USBtoComHuart.Init.StopBits = UART_STOPBITS_1; /* ֹͣλ */
	USBcom.USBtoComHuart.Init.WordLength = UART_WORDLENGTH_8B; /* �ֳ� */
	USBcom.USBtoComHuart.Init.Parity = UART_PARITY_NONE; /* ������У�� */
	USBcom.USBtoComHuart.Init.Mode = UART_MODE_TX_RX;  /* ʹ�÷�������� */
	
	HAL_UART_Init(&USBcom.USBtoComHuart);
	/* �������������ж� */
	__HAL_UART_ENABLE_IT(&USBcom.USBtoComHuart,UART_IT_IDLE);
	
	HAL_NVIC_SetPriority(USART1_IRQn,1,1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
	/* ����unlink���� */
	__HAL_LINKDMA(&USBcom.USBtoComHuart,hdmarx,USBcom.USBBtoComDMA);
	/* ʹ��DMA���� */
	HAL_UART_Receive_DMA(&USBcom.USBtoComHuart,USBcom.re_buf,1024);
}

/**
  * role:��ʼ�����ڵ�DMA
	* parm:blk----������
	* ret:void
	*
	*
  **/

void USBtoCom_DMA(void)
{
	/* ʹ��DMA��ʱ�� */
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* ����DMA1 */
	USBcom.USBBtoComDMA.Instance = DMA1_Channel5;
	USBcom.USBBtoComDMA.Init.Direction = DMA_PERIPH_TO_MEMORY; /* ���赽�ڴ� */
	USBcom.USBBtoComDMA.Init.PeriphInc = DMA_PINC_DISABLE;  /* ���費���� */
	USBcom.USBBtoComDMA.Init.MemInc = DMA_MINC_ENABLE;      /* �ڴ����� */
	USBcom.USBBtoComDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* �ڴ�Ϊ8λ */
	USBcom.USBBtoComDMA.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    /* ����Ҳ��8λ */
	USBcom.USBBtoComDMA.Init.Mode = DMA_NORMAL; /* ��ѭ��ģʽ */
	USBcom.USBBtoComDMA.Init.Priority = DMA_PRIORITY_VERY_HIGH;  /* ���ȼ��ǳ��� */
	
	HAL_DMA_Init(&USBcom.USBBtoComDMA);
} 


/**
  * role:��ʼ������
	* parm:blk---������
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
	

	/* ��ȡ��־λ,�Ƿ�Ϊ�����жϱ�־ */
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



