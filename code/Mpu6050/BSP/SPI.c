#include "stm32f1xx_hal.h"
#include "SPI.h"
#include <stdio.h>
#include "noFlash.h"
#include "delay.h"
#include <stdbool.h>


SPI_HandleTypeDef Spi_Handler;

#define SPI1_SCL_H() 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define SPI1_SCL_L() 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)

#define SPI1_MISO_READ() 	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)

#define SPI1_MOSI_H() 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)
#define SPI1_MOSI_L() 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)



///**
//*******************************
//* @brief : ��ʼ��SPI1��Ҫ��GPIO
//* @parm  ��void
//* @return ��void
//*******************************
//*/
//static void SPI1_GPIO_Init(void)
//{
//	
//	/* ʹ�ܶ�ӦGPIO��ʱ�� */
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	GPIO_InitTypeDef Gpio_init_struct;
//	
//	/* ��ʼ��SCL */
//	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
//	Gpio_init_struct.Pin = GPIO_PIN_5;
//	Gpio_init_struct.Pull = GPIO_NOPULL;
//	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);
//	
//	/* ��ʼ��MIOS */
//	Gpio_init_struct.Mode = GPIO_MODE_INPUT;
//	Gpio_init_struct.Pin = GPIO_PIN_6;
//	Gpio_init_struct.Pull = GPIO_NOPULL;
//	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);	
//	
//	/* ��ʼ��MOIS */
//	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
//	Gpio_init_struct.Pin = GPIO_PIN_7;
//	Gpio_init_struct.Pull = GPIO_NOPULL;
//	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);	
//	
//	/* ��ʼ��CSS */
//	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
//	Gpio_init_struct.Pin = GPIO_PIN_4;
//	Gpio_init_struct.Pull = GPIO_NOPULL;
//	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);	

//	SPI1_SCL_H();
//	SPI1_SCL_L();
//	
//}

///**
//*******************************
//* @brief : ��ʼ��SPI1
//* @parm  ��void
//* @return ��void
//*******************************
//*/
//void SPI1_Init(void)
//{
//	SPI1_GPIO_Init();
//}
///**
//*******************************
//* @brief : SPI1��дһ���ֽ�
//* @parm  ��data---���з��͵�����
//* @return ��uint8_t ��ȡ��������
//************************* ******
//*/
//uint8_t SPI1_WriteReadBtye(uint8_t data)
//{
//	uint8_t ret = 0;
//	for(uint8_t i = 0;i < 8;i++)
//	{
//		SPI1_SCL_L();
//		
//		if(data & 0x80)
//		{
//			SPI1_MOSI_H();
//		}
//		else 
//		{
//			SPI1_MOSI_L();
//		}
//		data <<= 1;
//		DelayNus(10);
//		SPI1_SCL_H();
//		DelayNus(10);
//		ret <<= 1;
//		if(SPI1_MISO_READ() == GPIO_PIN_SET)
//			ret++;
//		
//	}
//	return ret;
//}


/**
*******************************
* @brief : ��ʼ��Ӳ��SPI1��Ҫ�����ſ�
* @parm  ��void
* @return ��void
*******************************
*/
static void SPI1_GPIO_Init(void)
{
	/* ʹ��ʱ�� */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef Gpio_init_struct;	
	
	/* ����SCK���� */
	Gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	Gpio_init_struct.Pin = GPIO_PIN_5;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);
	/* ����MISO */
	Gpio_init_struct.Mode = GPIO_MODE_INPUT;
	Gpio_init_struct.Pin = GPIO_PIN_6;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);
	/* ����MOIS */
	Gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	Gpio_init_struct.Pin = GPIO_PIN_7;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);

	/* ����CSS0 */
	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	Gpio_init_struct.Pin = GPIO_PIN_4;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);	
	
	SPI1_CSS0_H();
}

/**
*******************************
* @brief : ��ʼ��Ӳ��SPI1
* @parm  ��void
* @return ��void
*******************************
*/
void SPI1_Init(void)
{
	SPI1_GPIO_Init();
	/* ʹ��ʱ�� */
	__HAL_RCC_SPI1_CLK_ENABLE();
	Spi_Handler.Instance = SPI1;
	Spi_Handler.Init.Mode = SPI_MODE_MASTER; /* ����Ϊ��ģʽ */
	Spi_Handler.Init.Direction = SPI_DIRECTION_2LINES;/* ����Ϊȫ˫�� */
	Spi_Handler.Init.DataSize = SPI_DATASIZE_8BIT;     /* ����λ����8λ */
	Spi_Handler.Init.CLKPhase = SPI_PHASE_1EDGE;
	Spi_Handler.Init.CLKPolarity = SPI_POLARITY_LOW;
	Spi_Handler.Init.NSS = SPI_NSS_SOFT;        /* �������NSS */
	Spi_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; /* ������ */
	Spi_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;  /* ��λ�ȴ��� */
	Spi_Handler.Init.TIMode = SPI_TIMODE_DISABLE; /* ��ʹ��TIģʽ */
	HAL_SPI_Init(&Spi_Handler);
}

/**
*******************************
* @brief : SPI��дһ���ֽ�
* @parm  ��data---���з��͵�����
* @return ��uint8_t ��ȡ��������
************************* ******
*/
uint8_t SPI1_WriteReadBtye(uint8_t data)
{
	uint8_t ret = 0;
	HAL_SPI_TransmitReceive(&Spi_Handler,&data,&ret,1,200);
	return ret;
}


