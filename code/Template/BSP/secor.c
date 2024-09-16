#include "secor.h"
#include "Temp.h"
#include <stdio.h>
/**
*******************************
* @brief : ��ʼ��������
* @parm  ��void
* @return ��void
*******************************
*/
void Secor_Init(void)
{
	NTR_Init();
	
}	

void Secor_Init(void);
/**
*******************************
* @brief : ������������������ת��
* @parm  ��void
* @return ��void
*******************************
*/
void Secor_Driver(void)
{
	NTR_Driver();
}
/**
*******************************
* @brief : ��ȡ����������
* @parm  ��buf---���ڱ��洫�������ݵĵ�ַ
* @return ��void
*******************************
*/
bool Get_Secor_Data(sec_t * buf)
{
	if(buf == NULL)
			return false;
	buf->temp = Get_Temp();
	if(!Get_AP3426(&(buf->ap)))
	{
		return false;
	}	
	return true;
		
}

