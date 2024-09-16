#ifndef KEY_H
#define KEY_H

#include "stm32f1xx_hal.h"

#define KEY1_GET  0x01
#define KEY2_GET  0x02
#define KEY3_GET  0x03
#define KEY4_GET  0x04

/**
*******************************
* @brief : ������ʼ��
* @parm  ��void
* @return ��void
*******************************
*/	
void KEY_Init(void);
/**
*******************************
* @brief : ϵͳ�԰���ɨ��
* @parm  ��void
* @return ��uint8_t ����״̬
*******************************
*/
uint8_t GetKayVal(void);

void KEY_Task(void);
#endif


