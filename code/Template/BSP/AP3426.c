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
//* @brief : ��ʼ��iic��io��
//* @parm  ��void
//* @return ��void
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
//* @brief : ��ʼ��iic
//* @parm  ��void
//* @return ��void
//*******************************
//*/
//static void IIC_HW_Init(void)
//{

//	
//	/* ʹ��I2Cʱ�� */
//	__HAL_RCC_I2C1_CLK_ENABLE();

//	/* ��I2C���и�λ���� */
//	__HAL_RCC_I2C1_FORCE_RESET();
//	__HAL_RCC_I2C1_RELEASE_RESET();
//	

//    I2C1_Handler.Instance             = I2C1;

//    I2C1_Handler.Init.ClockSpeed      = 400000;          // ����SCLʱ��Ƶ��(���400000)
//    I2C1_Handler.Init.DutyCycle       = I2C_DUTYCYCLE_2;          // ����I2C��SCLʱ�ӵ�ռ�ձ�(������)
//    I2C1_Handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;  // ���ù㲥����ģʽ(�ر�) 
//    I2C1_Handler.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;    // ���ý�ֹʱ���ӳ�ģʽ(�ر�) 
//		I2C1_Handler.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;  // ����I2CѰַ����ģʽ(ͨ��7bit)
//		I2C1_Handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;  // �Ƿ�ʹ������STM32���豸��ַ(�ر�) 
//    I2C1_Handler.Init.OwnAddress1     = 0x0A;                     // STM32���豸��ַ1(֧��7bit��10bit)
//    I2C1_Handler.Init.OwnAddress2     = 0;                        // STM32���豸��ַ2(ֻ֧��7bit)

//    if(HAL_I2C_Init(&I2C1_Handler) != HAL_OK)
//    {
//       printf("no ok\r\n");
//    }
//	

//	
//}
/**
*******************************
* @brief : ��AP3426����д�����
* @parm  ��reg---�Ĵ�����ַ�
           pbuffer---Ŀ������
					 num---���ݸ���
* @return ��bool --- д���Ƿ�ɹ�
*******************************
*/
static bool AP3426_Wrtie_Byet(uint8_t reg,uint8_t data)
{
	/* �����豸��ַ */
	IIC_Start();
	IIC_SEND_Byte((AP3426_ADDR << 1) | AP3426_WR);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	/* ���ͼĴ�����ַ */
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
* @brief : ��AP3426���ж�ȡ����
* @parm  ��reg---�Ĵ�����ַ
           pbuffer---Ŀ������
					 num---���ݸ���
* @return ��void
*******************************
*/
static bool AP3426_Read_Byte(uint8_t reg,uint8_t * pbuffer)
{
	/* ��ָ�� */
	if(pbuffer == NULL)
			return false;
	
	/* �����豸��ַ */
	IIC_Start();
	IIC_SEND_Byte((AP3426_ADDR << 1) | AP3426_WR);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	/* ���ͼĴ�����ַ */
	IIC_SEND_Byte(reg);
	if(!IIC_Get_ACK())
	{
		printf("the get ack error\r\n");
		return false;
	}
	
	/* ׼����ȡ���� */
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
* @brief : ��ʼ��AP3426
* @parm  ��void
* @return ��void
*******************************
*/
void AP3426_Init(void)
{
	IIC_Init();
	/* ��λap3426 */
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
* @brief : ��ȡAP3426������
* @parm  ��buf---������ݵĵ�ַ
* @return ��bool --- �Ƿ�ɹ�
*******************************
*/
bool Get_AP3426(AP3426_t * buf)
{
	uint8_t high = 0;
	uint8_t low = 0;
	if(buf == NULL)
		return false;
	
	/* ��ȡ���� */
	if(!AP3426_Read_Byte(AP3426_IR_REG_LOW_ADDR,&low))
		return false;
	if(!AP3426_Read_Byte(AP3426_IR_REG_HIGH_ADDR,&high))
		return false;
	buf->PS = (high << 8) | low;
	
	
	/* ��ȡ���� */
	if(!AP3426_Read_Byte(AP3426_ALS_REG_LOW_ADDR,&low) )
		return false;
	if(!AP3426_Read_Byte(AP3426_ALS_REG_HIGH_ADDR,&high) )
		return false;
	buf->ALS = (high << 8) | low;
	

	/* ��ȡ���� */
	if(!AP3426_Read_Byte(AP3426_PS_REG_LOW_ADDR,&low))
		return false;
	if(!AP3426_Read_Byte(AP3426_PS_REG_HIGH_ADDR,&high))
		return false;
	buf->PS = (high << 8) | low;

	

	return true;
}

