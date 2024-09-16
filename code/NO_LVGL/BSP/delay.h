#ifndef DELAY_H
#define DELAY_H
#include "stm32f1xx_hal.h"

/**
*******************************
* @brief : ��ʼ������DWT��delay
* @parm  ��void
* @return ��void
*******************************
*/
void Delay_Init(void);
/**
*******************************
* @brief : ΢����ʱ
* @parm  ��nus---Ҫ��ʱ������
* @return ��void
*******************************
*/
void DelayNus(uint32_t nUs);
/**
*******************************
* @brief : ������ʱ
* @parm  ��nms---Ҫ��ʱ������
* @return ��void
*******************************
*/
void DelayNums(uint32_t nMs);

#endif

