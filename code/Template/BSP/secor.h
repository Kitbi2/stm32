#ifndef SECOR_H
#define SECOR_H
#include "Temp.h"
#include "stm32f1xx_hal.h"
#include "AP3426.h"
typedef struct
{	
	float temp;
	uint8_t hum;
	AP3426_t ap;
}sec_t;
/**
*******************************
* @brief : ��ʼ��������
* @parm  ��void
* @return ��void
*******************************
*/
void Secor_Init(void);
/**
*******************************
* @brief : ������������������ת��
* @parm  ��void
* @return ��void
*******************************
*/
void Secor_Driver(void);
/**
*******************************
* @brief : ��ȡ����������
* @parm  ��buf---���ڱ��洫�������ݵĵ�ַ
* @return ��void
*******************************
*/

bool Get_Secor_Data(sec_t * buf);
#endif





