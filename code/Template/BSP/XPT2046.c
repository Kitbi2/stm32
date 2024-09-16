#include "stm32f1xx_hal.h"
#include "delay.h"
#include "stdio.h"
#include "LCD.h"
#include <stdbool.h>
#define SPI_XP_SCL_H() 			  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET)
#define SPI_XP_SCL_L() 			  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET)

#define SPI_XP_MISO_READ() 	  HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_7)

#define SPI_XP_MOSI_H() 			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_SET)
#define SPI_XP_MOSI_L() 			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_RESET)

#define SPI_XP_CSS_H() 			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET)
#define SPI_XP_CSS_L() 			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET)

/**
*******************************
* @brief : 初始化SPI_XP需要的GPIO
* @parm  ：void
* @return ：void
*******************************
*/
static void SPI_XP_GPIO_Init(void)
{
	
	/* 使能对应GPIO的时钟 */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef Gpio_init_struct;
	
	/* 初始化SCL */
	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	Gpio_init_struct.Pin = GPIO_PIN_10;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOF,&Gpio_init_struct);
	
	/* 初始化MIOS */
	Gpio_init_struct.Mode = GPIO_MODE_INPUT;
	Gpio_init_struct.Pin =  GPIO_PIN_7;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOF,&Gpio_init_struct);	
	
	/* 初始化MOIS */
	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	Gpio_init_struct.Pin = GPIO_PIN_8;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOF,&Gpio_init_struct);	
	
	/* 初始化CSS */
	Gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	Gpio_init_struct.Pin = GPIO_PIN_6;
	Gpio_init_struct.Pull = GPIO_NOPULL;
	Gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE,&Gpio_init_struct);	

	SPI_XP_CSS_H();
	SPI_XP_SCL_L();
	
}

/**
*******************************
* @brief : 初始化SPI_XP
* @parm  ：void
* @return ：void
*******************************
*/
static void SPI_XP_Init(void)
{
	SPI_XP_GPIO_Init();
}
/**
*******************************
* @brief : SPI_XP读写一个字节
* @parm  ：data---进行发送的数据
* @return ：uint8_t 读取到的数据
************************* ******
*/
static uint8_t SPI_XP_WriteReadBtye(uint8_t data)
{
	uint8_t ret = 0;
	for(uint8_t i = 0;i < 8;i++)
	{
		SPI_XP_SCL_L();
		
		if(data & 0x80)
		{
			SPI_XP_MOSI_H();
		}
		else 
		{
			SPI_XP_MOSI_L();
		}
		data <<= 1;
		DelayNus(10);
		SPI_XP_SCL_H();
		DelayNus(10);
		ret <<= 1;
		if(SPI_XP_MISO_READ() == GPIO_PIN_SET)
			ret++;
		
	}
	return ret;
}
/**
*******************************
* @brief : 初始化XPT2046
* @parm  ：void
* @return ：void
*******************************
*/
void XPT2046_Init(void)
{
	SPI_XP_Init();
}

//118 1870  5
//86 1986   3
/**
*******************************
* @brief : 读取x坐标
* @parm  ：void
* @return ：void
************************* ******
*/
bool XPT2046_Read_XY(uint16_t *x1,uint16_t *y1)
{
	
	SPI_XP_CSS_L();
	uint16_t x = 0;
	uint16_t y = 0;
	/* 发送命令 */
	SPI_XP_WriteReadBtye(0xD0);
	DelayNus(8);
	x = SPI_XP_WriteReadBtye(0x00);
	x <<= 8;
	x |= SPI_XP_WriteReadBtye(0x00);
	x >>= 4;
	SPI_XP_WriteReadBtye(0x90);
	DelayNus(8);
	y = SPI_XP_WriteReadBtye(0x00);
	y <<= 8;
	y |= SPI_XP_WriteReadBtye(0x00);
	y >>= 4;
	
	if(x >= 118 && x<= 1870)
		x /= 5.575;
	else
		return false;
	
	if(y >= 86 &&y <= 1986)
		y /= 4.358;
	else
		return false;	
	
	if(x1 != NULL && y1 != NULL)
	{ 
		*x1 = x;
		*y1 = y;
	}
	SPI_XP_CSS_H();
	
	return true;
}
