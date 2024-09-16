
#include "stm32f1xx_hal.h"
#define SPI1_CSS0_H() 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define SPI1_CSS0_L() 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
/**
*******************************
* @brief : SPI读写一个字节
* @parm  ：data---进行发送的数据
* @return ：uint8_t 读取到的数据
************************* ******
*/
uint8_t SPI1_WriteReadBtye(uint8_t data);

/**
*******************************
* @brief : 初始化SPI1
* @parm  ：void
* @return ：void
*******************************
*/
void SPI1_Init(void);



