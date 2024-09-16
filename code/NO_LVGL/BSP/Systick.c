#include "stm32f1xx_hal.h"

static void (*tiem_sheck)(void) = NULL;

void sheck_Init(void (*sheck)(void))
{
	tiem_sheck = sheck;
}

volatile uint64_t work_time = 0;
void Work_Time_Init(void)
{
	
	
	//SysTick_Config();
	HAL_SYSTICK_Config(72000);
	
	HAL_NVIC_SetPriority(SysTick_IRQn,3,3);
	HAL_NVIC_EnableIRQ(SysTick_IRQn);
}


void SysTick_Handler(void)
{
	if(tiem_sheck != NULL)
		tiem_sheck();
	work_time++;

}

uint64_t Get_Work_Time(void)
{
	
	return work_time;
}



