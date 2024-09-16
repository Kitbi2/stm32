#ifndef LED_H
#define LED_H

#include "stm32f1xx_hal.h"

#define LED_RED    0
#define LED_GREEN  1
#define LED_BLUE   2
/**
*******************************
* @brief : led��ʼ��
* @parm  ��void
* @return ��void
*******************************
*/
void Led_DrvInit(void);
/**
*******************************
* @brief : ����һ��led
* @parm  ��ledno---Ŀ��led
* @return ��void
*******************************
*/
void TurnOnLed(uint8_t ledNo);
/**
*******************************
* @brief : Ϩ��һ��led
* @parm  ��ledNo---Ŀ��led
* @return ��void
*******************************
*/
void TurnOffLed(uint8_t ledNo);
/**
*******************************
* @brief : led�ƽ��е�ƽ��ת
* @parm  ��ledNo---Ŀ��led
* @return ��void
*******************************
*/
void ToogleLed(uint8_t ledNo);
void GPIO_CLK_ENABLE(GPIO_TypeDef * GPIO_Pro);

#endif


