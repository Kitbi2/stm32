#include "secor.h"
#include "Temp.h"
#include <stdio.h>
/**
*******************************
* @brief : 初始化传感器
* @parm  ：void
* @return ：void
*******************************
*/
void Secor_Init(void)
{
	NTR_Init();
	
}	

void Secor_Init(void);
/**
*******************************
* @brief : 驱动传感器进行数据转化
* @parm  ：void
* @return ：void
*******************************
*/
void Secor_Driver(void)
{
	NTR_Driver();
}
/**
*******************************
* @brief : 获取传感器数据
* @parm  ：buf---用于保存传感器数据的地址
* @return ：void
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

