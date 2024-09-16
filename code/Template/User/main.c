#include <time.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_clk.h"
#include "delay.h"
#include "led_driver.h"
#include "KEY.h"
#include "USBtoCom.h"
#include "USBtoComApp.h"
#include "Systick.h"
#include "tim_driver.h"
#include "pwm_driver.h"
#include "him.h"
#include "Calendar.h"
#include "Temp.h"
#include "secor.h"
#include "IIC.h"
#include "at24c02.h"
#include "AP3426.h"
#include "noFlash.h"
#include "FSMC.h"
#include "LCD.h"
#include "XPT2046.h"
typedef struct
{
	uint8_t flag;
	uint32_t time;
	uint32_t load_time;
	void (* task) (void);
 
}Task_t;
static  Task_t Task_table[] = 
{  
	 {0,2,2,Him_LCD_Task},
   {0,500,500,USBtoComTask},  /* usart���ݴ������� */
	 {0,10,10,KEY_Task},        /* ����ɨ������ */
	 {0,500,500,at24c02_test},  /* �������������� */
//	 {0,1000,1000,Him_Ouput},
//	 {0,1,1,XPT2046_Read_XY}
	// {0,10,10,ParseTask},
	 
};
#define TASK_MAX()    sizeof(Task_table)/sizeof(Task_table[0])
void sheck_task(void)
{
	for(uint8_t i = 0;i <TASK_MAX();i++ )
	{
		if(Task_table[i].time == 0)
		{
			Task_table[i].flag = 1;
			Task_table[i].time = Task_table[i].load_time ;
			
		}
		Task_table[i].time--;
	}
	
}

void DriInit(void)
{
	HAL_Init();
	SystemClock_Config();
	Delay_Init();
	Led_DrvInit();
	KEY_Init();
	USBtoComUsart_Init(115200);
	FSMC_Init();

	Him_Init();
	Secor_Init();
	AP3426_Init();
	Calendar_Init();
	NOFlash_Init();
//	Sram_test();
//	LCD_Init();
	
//	LCD_Fill_PureColor(0,160,100,300,NULL);
	
}
void App_Init(void)
{
		USBtoComAppInit();
		sheck_Init(sheck_task);

}
int main()
{
	DriInit();
	App_Init();


	
	while(1)
	{
			for(uint8_t i = 0;i <TASK_MAX();i++)
			{
				if(Task_table[i].flag == 1)
				{
					
					Task_table[i].task();
					Task_table[i].flag = 0;
				}
			}
	}
	return 0;
}

