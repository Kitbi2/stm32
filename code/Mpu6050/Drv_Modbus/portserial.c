/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#include "port.h"

 #include  "stm32f1xx_hal.h"
 
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

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
static void Modbus_GPIOInit(void)
{
	USBcom.UART_RX_PRO =  GPIOA;
	USBcom.UART_RX_PIN =  GPIO_PIN_3;
	USBcom.UART_TX_PRO =  GPIOA;
	USBcom.UART_TX_PIN =  GPIO_PIN_2;
	/* 初始化GPIO */
	/* 使能时钟和配置为推挽输出模式 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef tmp;
	tmp.Mode = GPIO_MODE_AF_PP;
	tmp.Pin = USBcom.UART_TX_PIN;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(USBcom.UART_TX_PRO,&tmp);
	
	/* 使能时钟和配置为上拉输入模式 */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	tmp.Mode = GPIO_MODE_INPUT;
	tmp.Pin = USBcom.UART_RX_PIN;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	tmp.Pull = GPIO_PULLUP;
	
	HAL_GPIO_Init(USBcom.UART_RX_PRO,&tmp);		
}

static void Modbus_SwitchInit(void)
{
		GPIO_InitTypeDef tmp;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	tmp.Mode = GPIO_MODE_OUTPUT_PP;
	tmp.Pin = GPIO_PIN_5;
	tmp.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC,&tmp);	
}
/**
  * role:初始化串口的属性
	* parm:blk----波特率
	* ret:void
	*
	*
  **/
static void Modbus_UsartInit(uint32_t blk)
{
	/* 使能uart1的时钟 */
	__HAL_RCC_USART1_CLK_ENABLE();
	
	/* 配置uart1 */
	USBcom.USBtoComHuart.Instance = USART2;   /* 要使用的外设 */
	USBcom.USBtoComHuart.Init.BaudRate = blk;  /* 波特率 */
	USBcom.USBtoComHuart.Init.StopBits = UART_STOPBITS_1; /* 停止位 */
	USBcom.USBtoComHuart.Init.WordLength = UART_WORDLENGTH_8B; /* 字长 */
	USBcom.USBtoComHuart.Init.Parity = UART_PARITY_NONE; /* 不适用校验 */
	USBcom.USBtoComHuart.Init.Mode = UART_MODE_TX_RX;  /* 使用发送与接受 */
	
	HAL_UART_Init(&USBcom.USBtoComHuart);
		/* 使能接受中断和发送中断 */
	__HAL_UART_ENABLE_IT(&USBcom.USBtoComHuart,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&USBcom.USBtoComHuart,UART_IT_TC);

	HAL_NVIC_SetPriority(USART2_IRQn,1,1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}


/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	
		if(TRUE == xRxEnable)
		{
			__HAL_UART_ENABLE_IT(&USBcom.USBtoComHuart,UART_IT_RXNE);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);	
		}
		else
		{
			__HAL_UART_DISABLE_IT(&USBcom.USBtoComHuart,UART_IT_RXNE);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);				
		}
		
		if(TRUE == xTxEnable)
		{
			__HAL_UART_ENABLE_IT(&USBcom.USBtoComHuart,UART_IT_TC);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	
		
		}
		else
		{
			__HAL_UART_DISABLE_IT(&USBcom.USBtoComHuart,UART_IT_TC);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);				
		
		}
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
}


void USART2_IRQHandler(void)
{
	

	/* 获取标志位,是否为TC中断标志 */
	if(__HAL_UART_GET_FLAG(&USBcom.USBtoComHuart, UART_IT_TC))
	{
		prvvUARTRxISR();
		__HAL_UART_CLEAR_FLAG(&USBcom.USBtoComHuart,UART_FLAG_TC);

		
	}
	
	if(__HAL_UART_GET_FLAG(&USBcom.USBtoComHuart, UART_IT_RXNE))
	{
		prvvUARTTxReadyISR();
		__HAL_UART_CLEAR_FLAG(&USBcom.USBtoComHuart,UART_IT_RXNE);
	}
	
  	
		
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	Modbus_SwitchInit();
	Modbus_GPIOInit();
	Modbus_UsartInit(9600);
  return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	
		HAL_UART_Transmit(&USBcom.USBtoComHuart,(uint8_t*)&ucByte,1,1000);
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
		
		HAL_UART_Receive(&USBcom.USBtoComHuart,(uint8_t*)&pucByte,1,1000);
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
