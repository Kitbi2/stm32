#include "stm32f1xx_hal.h"
#include "led_driver.h"
TIM_HandleTypeDef timer6;

void Time_Init(uint32_t per,uint32_t pre)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	/* 定时器基地址 */
	timer6.Instance = TIM6;
	/* 自动装载值 */
	timer6.Init.Period = per - 1;
	/* 预分配值 */
	timer6.Init.Prescaler = pre - 1;
	/* 计数模式 */
	timer6.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	HAL_TIM_Base_Init(&timer6);
	
	HAL_NVIC_SetPriority(TIM6_IRQn,2,3);
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
	
	HAL_TIM_Base_Start_IT(&timer6);
	
	
	
	
}

void TIM6_IRQHandler(void)
{
	if(__HAL_TIM_GET_FLAG(&timer6,TIM_FLAG_UPDATE))
	{
		__HAL_TIM_CLEAR_FLAG(&timer6,TIM_FLAG_UPDATE);
		static uint16_t i = 0;
		i++;
		if(i == 500)
		{
			i = 0;
			ToogleLed(LED_BLUE);
		}
	}
}
	



