#include "stm32f1xx_hal.h"
#include "delay.h"
/**
*******************************
* @brief : 初始化基于DWT的delay
* @parm  ：void
* @return ：void
*******************************
*/
void Delay_Init(void)
{
	/* 关闭TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
	/* 开启TRC */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	
	/* 关闭计数功能 */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	/* 开启计数功能 */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	/* 计数清零 */
	DWT->CYCCNT = 0;
}
/**
*******************************
* @brief : 微秒延时
* @parm  ：nus---要延时的秒数
* @return ：void
*******************************
*/
void DelayNus(uint32_t nUs)
{
	uint32_t tickStart = DWT->CYCCNT;
	
	nUs *= ( 720000000 /10000000 );
	
	
	while((DWT->CYCCNT - tickStart) < nUs);
}
/**
*******************************
* @brief : 毫秒延时
* @parm  ：nms---要延时的秒数
* @return ：void
*******************************
*/
void DelayNums(uint32_t nMs)
{
	for(uint32_t i = 0; i < nMs;i++)
	{
		DelayNus(1000);
	}
}


void HAL_Delay(uint32_t nMs)
{
	for(uint32_t i = 0; i < nMs;i++)
	{
		DelayNus(1000);
	}
}


