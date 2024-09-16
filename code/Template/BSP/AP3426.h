#ifndef IIC_HW_H
#define IIC_HW_H
#include <stdbool.h>
typedef struct
{
	uint16_t ALS; /* 光照数据 */
	uint16_t PS;  /* 接近传感器 */
	uint16_t IR;  /* 红外光传感器 */
}AP3426_t;
/**
*******************************
* @brief : 初始化AP3426
* @parm  ：void
* @return ：void
*******************************
*/
void AP3426_Init(void);
/**
*******************************
* @brief : 获取AP3426的数据
* @parm  ：buf---存放数据的地址
* @return ：bool --- 是否成功
*******************************
*/
bool Get_AP3426(AP3426_t * buf);
void IIC_HW_Test(void);





#endif
