#ifndef ADC_H
#define ADC_H

/**
*******************************
* @brief : ��ȡ�¶�
* @parm  ��void
* @return ��float ---�¶ȵı���ֵ
*******************************
*/
float Get_Temp(void);
/**
*******************************
* @brief : NTR
* @parm  ��void
* @return ��void
*******************************
*/
void NTR_Init(void);
/**
*******************************
* @brief : ����NTR����ת��
* @parm  ��void
* @return ��uint16_t ---��ѹ�ı���ֵ
*******************************
*/
void NTR_Driver(void);

#endif



