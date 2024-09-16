#ifndef IIC_H
#define IIC_H
#include <stdbool.h>
/**
*******************************
* @brief : 初始化IIC
* @parm  ：void
* @return ：void
*******************************
*/
void IIC_Init(void);
/**
*******************************
* @brief : 发送起始信号
* @parm  ：void
* @return ：void
*******************************
*/
void IIC_Start(void);

/**
*******************************
* @brief : 发送停止信号
* @parm  ：void
* @return ：void
*******************************
*/
void IIC_STOP(void);

/**
*******************************
* @brief : 发送一字节数据
* @parm  ：uint8_t --- 要进行发送的数据
* @return ：void
*******************************
*/
void IIC_SEND_Byte(uint8_t data);

/**
*******************************
* @brief : 接受一字节数据
* @parm  ：void
* @return ：uint8_t --- 读取到的数据
*******************************
*/
uint8_t IIC_RECIVE_Byte(void);

/**
*******************************
* @brief : 发送ACK信号
* @parm  ：void
* @return : void
*******************************
*/
void IIC_Send_ACK(void);
	
/**
*******************************
* @brief : 发送NACK信号
* @parm  ：void
* @return : void
*******************************
*/
void IIC_Send_NACK(void);

/**
*******************************
* @brief : 读取ACK信号
* @parm  ：void
* @return : void
*******************************
*/
bool IIC_Get_ACK(void);


void iic_task(void);

#endif

