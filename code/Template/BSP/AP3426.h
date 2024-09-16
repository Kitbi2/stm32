#ifndef IIC_HW_H
#define IIC_HW_H
#include <stdbool.h>
typedef struct
{
	uint16_t ALS; /* �������� */
	uint16_t PS;  /* �ӽ������� */
	uint16_t IR;  /* ����⴫���� */
}AP3426_t;
/**
*******************************
* @brief : ��ʼ��AP3426
* @parm  ��void
* @return ��void
*******************************
*/
void AP3426_Init(void);
/**
*******************************
* @brief : ��ȡAP3426������
* @parm  ��buf---������ݵĵ�ַ
* @return ��bool --- �Ƿ�ɹ�
*******************************
*/
bool Get_AP3426(AP3426_t * buf);
void IIC_HW_Test(void);





#endif
