#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "delay.h"
#include <stdbool.h>
#include "AP3426.h"
#include "IIC.h"

#define AP3426_ADDR   (0x1E)
#define AP3426_WR 		0x00  
#define AP3426_RD 		0x01  


#define AP3426_CONFIG_REG_ADDR 0x00

#define AP3426_IR_REG_LOW_ADDR 0x0A
#define AP3426_IR_REG_HIGH_ADDR 0x0B

#define AP3426_ALS_REG_LOW_ADDR 0x0C
#define AP3426_ALS_REG_HIGH_ADDR 0x0D

#define AP3426_PS_REG_LOW_ADDR 0x0E
#define AP3426_PS_REG_HIGH_ADDR 0x0F

#define AP3426_SW_RESET_CODE 0x04
#define AP3426_ALS_IR_PS    0x03


//I2C_HandleTypeDef I2C1_Handler = {0};
///**
//*******************************
//* @brief : ³õÊ¼»¯iicµÄio¿Ú
//* @parm  £ºvoid
//* @return £ºvoid
//*******************************
//*/
//static void IIC_IW_GPIO_Init(void)
//{
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//	GPIO_InitTypeDef Init_GPIO;
//  Init_GPIO.Mode = GPIO_MODE_AF_OD;
//	Init_GPIO.Pin = GPIO_PIN_6 ;
//	Init_GPIO.Pull = GPIO_NOPULL;
//	Init_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOB,&Init_GPIO);
//	
//  Init_GPIO.Mode = GPIO_MODE_AF_OD;
//	Init_GPIO.Pin =  GPIO_PIN_7;
//	Init_GPIO.Pull = GPIO_NOPULL;
//	Init_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOB,&Init_GPIO);	
//}
///**
//*******************************
//* @brief : ³õÊ¼»¯iic
//* @parm  £ºvoid
//* @return £ºvoid
//*******************************
//*/
//static void IIC_HW_Init(void)
//{

//	
//	/* Ê¹ÄÜI2CÊ±ÖÓ */
//	__HAL_RCC_I2C1_CLK_ENABLE();

//	/* ¶ÔI2C½øÐÐ¸´Î»²Ù×÷ */
//	__HAL_RCC_I2C1_FORCE_RESET();
//	__HAL_RCC_I2C1_RELEASE_RESET();
//	

//    I2C1_Handler.Instance             = I2C1;

//    I2C1_Handler.Init.ClockSpeed      = 400000;          // ÉèÖÃSCLÊ±ÖÓÆµÂÊ(×î¸ß400000)
//    I2C1_Handler.Init.DutyCycle       = I2C_DUTYCYCLE_2;          // ÉèÖÃI2CµÄSCLÊ±ÖÓµÄÕ¼¿Õ±È(¶¼¿ÉÒÔ)
//    I2C1_Handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;  // ÉèÖÃ¹ã²¥ºô½ÐÄ£Ê½(¹Ø±Õ) 
//    I2C1_Handler.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;    // ÉèÖÃ½ûÖ¹Ê±ÖÓÑÓ³¤Ä£Ê½(¹Ø±Õ) 
//		I2C1_Handler.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;  // ÉèÖÃI2CÑ°Ö·³¤¶ÈÄ£Ê½(Í¨³£7bit)
//		I2C1_Handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;  // ÊÇ·ñÊ¹ÓÃÁ½¸öSTM32µÄÉè±¸µØÖ·(¹Ø±Õ) 
//    I2C1_Handler.Init.OwnAddress1     = 0x0A;                     // STM32µÄÉè±¸µØÖ·1(Ö§³Ö7bit»ò10bit)
//    I2C1_Handler.Init.OwnAddress2     = 0;                        // STM32µÄÉè±¸µØÖ·2(Ö»Ö§³Ö7bit)

//    if(HAL_I2C_Init(&I2C1_Handler) != HAL_OK)
//    {
//       printf("no ok\r\n");
//    }
//	

//	
//}
/**
*******************************
* @brief : ¶ÔAP3426½øÐÐÐ´Èë²Ù×÷
* @parm  £ºreg---¼Ä´æÆ÷µØÖ·£
           pbuffer---Ä¿±êÊý¾Ý
					 num---Êý¾Ý¸öÊý
* @return £ºbool --- Ð´ÈëÊÇ·ñ³É¹¦
*******************************
*/
static bool AP3426_Wrtie_Byet(uint8_t reg,uint8_t data)
{
	/* ·¢ËÍÉè±¸µØÖ· */
	IIC_Start();
	IIC_SEND_Byte((AP3426_ADDR << 1) | AP3426_WR);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	/* ·¢ËÍ¼Ä´æÆ÷µØÖ· */
	IIC_SEND_Byte(reg);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	IIC_SEND_Byte(data);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	IIC_STOP();
	return true;
}
/**
*******************************
* @brief : ¶ÔAP3426½øÐÐ¶ÁÈ¡²Ù×÷
* @parm  £ºreg---¼Ä´æÆ÷µØÖ·
           pbuffer---Ä¿±êÊý¾Ý
					 num---Êý¾Ý¸öÊý
* @return £ºvoid
*******************************
*/
static bool AP3426_Read_Byte(uint8_t reg,uint8_t * pbuffer)
{
	/* ¿ÕÖ¸Õë */
	if(pbuffer == NULL)
			return false;
	
	/* ·¢ËÍÉè±¸µØÖ· */
	IIC_Start();
	IIC_SEND_Byte((AP3426_ADDR << 1) | AP3426_WR);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	/* ·¢ËÍ¼Ä´æÆ÷µØÖ· */
	IIC_SEND_Byte(reg);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	
	/* ×¼±¸¶ÁÈ¡Êý¾Ý */
	IIC_Start();
	IIC_SEND_Byte((AP3426_ADDR << 1) | AP3426_RD);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	*pbuffer = IIC_RECIVE_Byte();
	IIC_Send_NACK();
	IIC_STOP();
	

	return true;
}

/**
*******************************
* @brief : ³õÊ¼»¯AP3426
* @parm  £ºvoid
* @return £ºvoid
*******************************
*/
void AP3426_Init(void)
{
	IIC_Init();
	/* ¸´Î»ap3426 */
	uint8_t buf = 0x00;
	AP3426_Wrtie_Byet(AP3426_CONFIG_REG_ADDR,AP3426_SW_RESET_CODE);
	DelayNums(50);
	AP3426_Wrtie_Byet(AP3426_CONFIG_REG_ADDR,AP3426_ALS_IR_PS);
	DelayNums(50);
	AP3426_Read_Byte(AP3426_CONFIG_REG_ADDR,&buf);
	if(buf != 0x03)
	{
		printf("AP3426 Init error\r\n");
	}
}
/**
*******************************
* @brief : »ñÈ¡AP3426µÄÊý¾Ý
* @parm  £ºbuf---´æ·ÅÊý¾ÝµÄµØÖ·
* @return £ºbool --- ÊÇ·ñ³É¹¦
*******************************
*/
bool Get_AP3426(AP3426_t * buf)
{
	uint8_t high = 0;
	uint8_t low = 0;
	if(buf == NULL)
		return false;
	
	/* ¶ÁÈ¡ºìÍâ */
	if(!AP3426_Read_Byte(AP3426_IR_REG_LOW_ADDR,&low))
		return false;
	if(!AP3426_Read_Byte(AP3426_IR_REG_HIGH_ADDR,&high))
		return false;
	buf->PS = (high << 8) | low;
	
	
	/* ¶ÁÈ¡¹âÕÕ */
	if(!AP3426_Read_Byte(AP3426_ALS_REG_LOW_ADDR,&low) )
		return false;
	if(!AP3426_Read_Byte(AP3426_ALS_REG_HIGH_ADDR,&high) )
		return false;
	buf->ALS = (high << 8) | low;
	

	/* ¶ÁÈ¡¿¿½ü */
	if(!AP3426_Read_Byte(AP3426_PS_REG_LOW_ADDR,&low))
		return false;
	if(!AP3426_Read_Byte(AP3426_PS_REG_HIGH_ADDR,&high))
		return false;
	buf->PS = (high << 8) | low;

	

	return true;
}

