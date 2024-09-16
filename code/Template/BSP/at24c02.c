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
* @brief : ��ȡACK�ź�
* @parm  ��addr---Ҫ�����ȡ��eeprom�ĵ�ַ
           pbuffer---���ݴ�ŵĵ�ַ
					 num---��ȡ�����ݸ���
* @return : bool --- �Ƿ��ȡ�ɹ�
*******************************
**/

bool EEPROM_Read(uint8_t addr,uint8_t * pbuffer,uint8_t num)
{
	uint8_t i = 0;
	/* ��ָ��������ݹ��� */
	if(pbuffer == NULL || addr + num > 255)
		return false;
	for(i = 0;i < 10;i++)
	{
		IIC_Start();                                /* ��ʼ�ź� */
		IIC_SEND_Byte(EEPROM_ADDR | EEPROM_WR);     /* �����豸��ַ */
		if(IIC_Get_ACK())
			break;
		
	}
	if(i == 10)
	{
		goto iic_err;
	}
	IIC_SEND_Byte(addr);                        /* ���ͼĴ�����ַ */
	if(!IIC_Get_ACK())
		goto iic_err;
	
	IIC_Start();                                /* ��ʼ�ź� */
	IIC_SEND_Byte(EEPROM_ADDR | EEPROM_RD);     /* �����豸��ַ */
	if(!IIC_Get_ACK())
		goto iic_err;	
	
  /* ��ȡnum���ֽ����� */
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
* @brief : ��ȡACK�ź�
* @parm  ��addr---Ҫ����д���eeprom�ĵ�ַ
           pbuffer---���ݴ�ŵĵ�ַ
					 num---д������ݸ���
* @return : bool --- �Ƿ�д��ɹ�
*******************************
**/

bool EEPROM_Write(uint8_t addr,uint8_t * pbuffer,uint8_t num)
{
	/* ��ָ��������ݹ��� */
	if(pbuffer == NULL || addr + num > 255)
		return false;
	
	
	uint8_t check = 0;
	for(uint8_t i = 0;i < num;i++)
	{
		if(addr == 0 || addr %8 == 0) /* ��ʼ��ַ����˵��һҳ����ʼ��ַ��ʱ�� */
		{
			IIC_STOP();
			for(check  = 0;check < 200;check++)
			{
				IIC_Start();                                /* ��ʼ�ź� */
				IIC_SEND_Byte(EEPROM_ADDR | EEPROM_WR);     /* �����豸��ַ */
				if(IIC_Get_ACK())
					break;
			}
			if(check == 200)
			{
				goto iic_err;
			}
			IIC_SEND_Byte(addr);                        /* ���ͼĴ�����ַ */
			if(!IIC_Get_ACK())
				goto iic_err;			
		}
		IIC_SEND_Byte(pbuffer[i]);                        /* �������� */
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
