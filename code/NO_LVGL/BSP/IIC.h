#ifndef IIC_H
#define IIC_H
#include <stdbool.h>
/**
*******************************
* @brief : ��ʼ��IIC
* @parm  ��void
* @return ��void
*******************************
*/
void IIC_Init(void);
/**
*******************************
* @brief : ������ʼ�ź�
* @parm  ��void
* @return ��void
*******************************
*/
void IIC_Start(void);

/**
*******************************
* @brief : ����ֹͣ�ź�
* @parm  ��void
* @return ��void
*******************************
*/
void IIC_STOP(void);

/**
*******************************
* @brief : ����һ�ֽ�����
* @parm  ��uint8_t --- Ҫ���з��͵�����
* @return ��void
*******************************
*/
void IIC_SEND_Byte(uint8_t data);

/**
*******************************
* @brief : ����һ�ֽ�����
* @parm  ��void
* @return ��uint8_t --- ��ȡ��������
*******************************
*/
uint8_t IIC_RECIVE_Byte(void);

/**
*******************************
* @brief : ����ACK�ź�
* @parm  ��void
* @return : void
*******************************
*/
void IIC_Send_ACK(void);
	
/**
*******************************
* @brief : ����NACK�ź�
* @parm  ��void
* @return : void
*******************************
*/
void IIC_Send_NACK(void);

/**
*******************************
* @brief : ��ȡACK�ź�
* @parm  ��void
* @return : void
*******************************
*/
bool IIC_Get_ACK(void);


void iic_task(void);

#endif

