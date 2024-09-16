#ifndef LED_H
#define LED_H

#include "stm32f1xx_hal.h"

#define LED_RED    0
#define LED_GREEN  1
#define LED_BLUE   2
/**
*******************************
* @brief : led初始化
* @parm  ：void
* @return ：void
*******************************
*/
void Led_DrvInit(void);
/**
*******************************
* @brief : 点亮一个led
* @parm  ：ledno---目标led
* @return ：void
*******************************
*/
void TurnOnLed(uint8_t ledNo);
/**
*******************************
* @brief : 熄灭一个led
* @parm  ：ledNo---目标led
* @return ：void
*******************************
*/
void TurnOffLed(uint8_t ledNo);
/**
*******************************
* @brief : led灯进行电平翻转
* @parm  ：ledNo---目标led
* @return ：void
*******************************
*/
void ToogleLed(uint8_t ledNo);
void GPIO_CLK_ENABLE(GPIO_TypeDef * GPIO_Pro);

#endif


