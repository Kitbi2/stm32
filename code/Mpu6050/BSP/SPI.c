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
//* @brief : 初始化SPI1需要的GPIO
//* @parm  ：void
//* @return ：void
//*******************************
//*/
//static void SPI1_GPIO_Init(void)
//{
//	
//	/* 使能对应GPIO的时钟 */
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	GPIO_InitTypeDef Gpio_init_struct;
//	
//	/* 初始化SCL */
//	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
//	Gpio_init_struct.Pin = GPIO_PIN_5;
//	Gpio_init_struct.Pull = GPIO_NOPULL;
//	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);
//	
//	/* 初始化MIOS */
//	Gpio_init_struct.Mode = GPIO_MODE_INPUT;
//	Gpio_init_struct.Pin = GPIO_PIN_6;
//	Gpio_init_struct.Pull = GPIO_NOPULL;
//	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);	
//	
//	/* 初始化MOIS */
//	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
//	Gpio_init_struct.Pin = GPIO_PIN_7;
//	Gpio_init_struct.Pull = GPIO_NOPULL;
//	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);	
//	
//	/* 初始化CSS */
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
//* @brief : 初始化SPI1
//* @parm  ：void
//* @return ：void
//*******************************
//*/
//void SPI1_Init(void)
//{
//	SPI1_GPIO_Init();
//}
///**
//*******************************
//* @brief : SPI1读写一个字节
//* @parm  ：data---进行发送的数据
//* @return ：uint8_t 读取到的数据
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
* @brief : 初始化硬件SPI1需要的引脚口
* @parm  ：void
* @return ：void
*******************************
*/
static void SPI1_GPIO_Init(void)
{
	/* 使能时钟 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef Gpio_init_struct;	
	
	/* 配置SCK引脚 */
	Gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	Gpio_init_struct.Pin = GPIO_PIN_5;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);
	/* 配置MISO */
	Gpio_init_struct.Mode = GPIO_MODE_INPUT;
	Gpio_init_struct.Pin = GPIO_PIN_6;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);
	/* 配置MOIS */
	Gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	Gpio_init_struct.Pin = GPIO_PIN_7;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);

	/* 配置CSS0 */
	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	Gpio_init_struct.Pin = GPIO_PIN_4;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&Gpio_init_struct);	
	
	SPI1_CSS0_H();
}

/**
*******************************
* @brief : 初始化硬件SPI1
* @parm  ：void
* @return ：void
*******************************
*/
void SPI1_Init(void)
{
	SPI1_GPIO_Init();
	/* 使能时钟 */
	__HAL_RCC_SPI1_CLK_ENABLE();
	Spi_Handler.Instance = SPI1;
	Spi_Handler.Init.Mode = SPI_MODE_MASTER; /* 配置为主模式 */
	Spi_Handler.Init.Direction = SPI_DIRECTION_2LINES;/* 配置为全双工 */
	Spi_Handler.Init.DataSize = SPI_DATASIZE_8BIT;     /* 数据位数是8位 */
	Spi_Handler.Init.CLKPhase = SPI_PHASE_1EDGE;
	Spi_Handler.Init.CLKPolarity = SPI_POLARITY_LOW;
	Spi_Handler.Init.NSS = SPI_NSS_SOFT;        /* 软件管理NSS */
	Spi_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; /* 波特率 */
	Spi_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;  /* 高位先传输 */
	Spi_Handler.Init.TIMode = SPI_TIMODE_DISABLE; /* 不使用TI模式 */
	HAL_SPI_Init(&Spi_Handler);
}

/**
*******************************
* @brief : SPI读写一个字节
* @parm  ：data---进行发送的数据
* @return ：uint8_t 读取到的数据
************************* ******
*/
uint8_t SPI1_WriteReadBtye(uint8_t data)
{
	uint8_t ret = 0;
	HAL_SPI_TransmitReceive(&Spi_Handler,&data,&ret,1,200);
	return ret;
}


