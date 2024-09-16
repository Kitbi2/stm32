#include "at24c02.h"
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "IIC.h"
#include "delay.h"
#define EEPROM_ADDR 0xA0
#define EEPROM_WR   0x00
#define EEPROM_RD   0x01
#define EEPROM_PAGE_SIZE 0x80

/**
*******************************
* @brief : 读取ACK信号
* @parm  ：addr---要进入读取的eeprom的地址
           pbuffer---数据存放的地址
					 num---读取的数据个数
* @return : bool --- 是否读取成功
*******************************
**/

bool EEPROM_Read(uint8_t addr,uint8_t * pbuffer,uint8_t num)
{
	uint8_t i = 0;
	/* 空指针或者数据过长 */
	if(pbuffer == NULL || addr + num > 255)
		return false;
	for(i = 0;i < 10;i++)
	{
		IIC_Start();                                /* 起始信号 */
		IIC_SEND_Byte(EEPROM_ADDR | EEPROM_WR);     /* 发送设备地址 */
		if(IIC_Get_ACK())
			break;
		
	}
	if(i == 10)
	{
		goto iic_err;
	}
	IIC_SEND_Byte(addr);                        /* 发送寄存器地址 */
	if(!IIC_Get_ACK())
		goto iic_err;
	
	IIC_Start();                                /* 起始信号 */
	IIC_SEND_Byte(EEPROM_ADDR | EEPROM_RD);     /* 发送设备地址 */
	if(!IIC_Get_ACK())
		goto iic_err;	
	
  /* 读取num个字节数据 */
	num--;
	while(num)
	{
		*pbuffer = IIC_RECIVE_Byte();
		IIC_Send_ACK();
		pbuffer++;
		num--;
	}
	*pbuffer = IIC_RECIVE_Byte();
	pbuffer++;
	IIC_Send_NACK();
	IIC_STOP();
	
	return true;
iic_err:
		printf("the revier error\r\n");

	return false;
}

/**
*******************************
* @brief : 读取ACK信号
* @parm  ：addr---要进入写入的eeprom的地址
           pbuffer---数据存放的地址
					 num---写入的数据个数
* @return : bool --- 是否写入成功
*******************************
**/

bool EEPROM_Write(uint8_t addr,uint8_t * pbuffer,uint8_t num)
{
	/* 空指针或者数据过长 */
	if(pbuffer == NULL || addr + num > 255)
		return false;
	
	
	uint8_t check = 0;
	for(uint8_t i = 0;i < num;i++)
	{
		if(addr == 0 || addr %8 == 0) /* 起始地址或者说是一页的起始地址的时候 */
		{
			IIC_STOP();
			for(check  = 0;check < 200;check++)
			{
				IIC_Start();                                /* 起始信号 */
				IIC_SEND_Byte(EEPROM_ADDR | EEPROM_WR);     /* 发送设备地址 */
				if(IIC_Get_ACK())
					break;
			}
			if(check == 200)
			{
				goto iic_err;
			}
			IIC_SEND_Byte(addr);                        /* 发送寄存器地址 */
			if(!IIC_Get_ACK())
				goto iic_err;			
		}
		IIC_SEND_Byte(pbuffer[i]);                        /* 发送数据 */
		if(!IIC_Get_ACK())
			goto iic_err;	
		
		addr++;
	}
	IIC_STOP();
	
	
	return true;
iic_err:
	printf("the send error\r\n");
	return false;
}

void at24c02_test(void)
{
	uint8_t write_buf[255] = {0xbb};
	uint8_t read_buf[255] = {0x00};

//	EEPROM_Write(0,write_buf,1);

	EEPROM_Read(0,read_buf,1);
	for(uint8_t i = 0;i < 1;i++)
	{
		printf("the data[%d] is %x\r\n",i,read_buf[i]);
	}	
	
}
