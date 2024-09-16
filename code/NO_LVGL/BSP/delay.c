#include "stm32f1xx_hal.h"
#include "delay.h"
/**
*******************************
* @brief : ��ʼ������DWT��delay
* @parm  ��void
* @return ��void
*******************************
*/
void Delay_Init(void)
{
	/* �ر�TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
	/* ����TRC */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	
	/* �رռ������� */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	/* ������������ */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	/* �������� */
	DWT->CYCCNT = 0;
}
/**
*******************************
* @brief : ΢����ʱ
* @parm  ��nus---Ҫ��ʱ������
* @return ��void
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
* @brief : ������ʱ
* @parm  ��nms---Ҫ��ʱ������
* @return ��void
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


