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
* @brief : 初始化传感器
* @parm  ：void
* @return ：void
*******************************
*/
void Secor_Init(void);
/**
*******************************
* @brief : 驱动传感器进行数据转化
* @parm  ：void
* @return ：void
*******************************
*/
void Secor_Driver(void);
/**
*******************************
* @brief : 获取传感器数据
* @parm  ：buf---用于保存传感器数据的地址
* @return ：void
*******************************
*/

bool Get_Secor_Data(sec_t * buf);
#endif





