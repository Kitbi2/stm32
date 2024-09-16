#include "stm32f1xx_hal.h"
#include "IIC.h"
#include "delay.h"
#include <stdio.h>
#include <stdbool.h>
#define SDA_H()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
#define SDA_L()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)
#define G_SDA()   HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)
#define SCL_H()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)
#define SCL_L()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)

#define IIC_DELAY() DelayNus(4)
/**
*******************************
* @brief : 初始化IIC
* @parm  ：void
* @return ：void
*******************************
*/
void IIC_Init(void)
{
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef Init_GPIO;
  Init_GPIO.Mode = GPIO_MODE_OUTPUT_OD;
	Init_GPIO.Pin = GPIO_PIN_6 ;
	Init_GPIO.Pull = GPIO_NOPULL;
	Init_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,&Init_GPIO);
	
  Init_GPIO.Mode = GPIO_MODE_OUTPUT_OD;
	Init_GPIO.Pin =  GPIO_PIN_7;
	Init_GPIO.Pull = GPIO_NOPULL;
	Init_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,&Init_GPIO);	
}

/**
*******************************
* @brief : 发送起始信号
* @parm  ：void
* @return ：void
*******************************
*/
void IIC_Start(void)
{
	SCL_H();
	SDA_H();
	IIC_DELAY();
	SDA_L();
	IIC_DELAY();
	SCL_L();
	IIC_DELAY();
	
}
/**
*******************************
* @brief : 发送停止信号
* @parm  ：void
* @return ：void
*******************************
*/
void IIC_STOP(void)
{
	SDA_L();
	IIC_DELAY();
	
	SCL_H();
	IIC_DELAY();
	
	SDA_H();
	IIC_DELAY();
}
/**
*******************************
* @brief :  发送一字节数据
* @parm  ：uint8_t --- 要进行发送的数据
* @return ：void
*******************************
*/
void IIC_SEND_Byte(uint8_t data)
{
	for(uint8_t i = 0;i < 8;i++)
	{
		if(data & 0x80)
		{
			SDA_H();
		}
		else
		{
			SDA_L();
		}
		data <<= 1;
		SCL_H();
		IIC_DELAY();
		SCL_L();
		IIC_DELAY();		
	}

}
/**
*******************************
* @brief : 接受一字节数据
* @parm  ：void
* @return ：uint8_t --- 读取到的数据
*******************************
*/
uint8_t IIC_RECIVE_Byte(void)
{
	uint8_t ret = 0;
	SDA_H();/* 手动释放sda */
	
	for(uint8_t i = 0;i < 8;i++)
	{
		SCL_H();
		IIC_DELAY();
		ret <<= 1;
    if(G_SDA())
		{
			ret++;
		}
		
		SCL_L();
		IIC_DELAY();
	}

	return ret;
}
/**
*******************************
* @brief : 发送ACK信号
* @parm  ：void
* @return : void
*******************************
*/
void IIC_Send_ACK(void)
{
	SDA_L();
	IIC_DELAY();
	SCL_H();
	IIC_DELAY();
	SCL_L();
	IIC_DELAY();		
	SDA_H();
	IIC_DELAY();		

}
/**
*******************************
* @brief : 发送NACK信号
* @parm  ：void
* @return : void
*******************************
*/
void IIC_Send_NACK(void)
{
	SDA_H();
	IIC_DELAY();	
	SCL_H();
	IIC_DELAY();
	SCL_L();
	IIC_DELAY();		
	
}
/**
*******************************
* @brief : 读取ACK信号
* @parm  ：void
* @return : void
*******************************
*/
bool IIC_Get_ACK(void)
{
	uint8_t timeout = 0;
	SDA_H();/* 手动释放sda */	
	IIC_DELAY();
	SCL_H();
	IIC_DELAY();
	while(G_SDA())
	{
		timeout++;
		if(timeout > 200)
		{
			IIC_STOP();
			return false;
	
		}
	}
	SCL_L();
	IIC_DELAY();	
	
	return true;
}



