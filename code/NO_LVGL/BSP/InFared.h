#ifndef INFARED_H
#define INFARED_H

#include "stm32f1xx_hal.h"
#include "queue.h"
#include <stdio.h>

/**
*******************************
* @brief : 红外遥控接受初始化
* @parm  ：void
* @return ：void
*******************************
*/
void InFare_Init(void);

void InFared_Task(void);
#endif


