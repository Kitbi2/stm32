#ifndef KEY_H
#define KEY_H

#include "stm32f1xx_hal.h"

#define KEY1_GET  0x01
#define KEY2_GET  0x02
#define KEY3_GET  0x03
#define KEY4_GET  0x04

/**
*******************************
* @brief : 按键初始化
* @parm  ：void
* @return ：void
*******************************
*/	
void KEY_Init(void);
/**
*******************************
* @brief : 系统性按键扫描
* @parm  ：void
* @return ：uint8_t 按键状态
*******************************
*/
uint8_t GetKayVal(void);

void KEY_Task(void);
#endif


