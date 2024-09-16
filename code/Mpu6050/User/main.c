#include "main.h"

#include "mpu6050.h"
#include "IIC.h"
#include "INIT.h"
#include "imu.h"



void GetMpu(void)
{
		MpuGetData();
	
}

void Check(void)
{
		GetAngle(&MPU6050,&Angle,0.004f);
	
}

void Print(void)
{
	printf("the x %2.2f\r\n",Angle.pitch);
	printf("the y %2.2f\r\n",Angle.roll);
	printf("the z %2.2f\r\n",Angle.yaw);	
}
typedef struct
{
	uint8_t flag;
	uint32_t time;
	uint32_t load_time;
	void (* task) (void);
 
}Task_t;
static  Task_t Task_table[] = 
{  
	 {0,2,2,GetMpu},
   {0,4,4,Check},  /* usart??????????? */
//	 {0,10,10,KEY_Task},        /* ??????????? */
	 {0,500,500,Print},  /* ?????????????? */
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
static void Driver_Init(void)
{
	HAL_Init();
	SystemClock_Config();	
	Delay_Init();
	Work_Time_Init();
	sheck_Init(sheck_task);
	USBtoComUsart_Init(115200);
	IIC_Init();
	MpuInit();

}
int main()
{

	Driver_Init();
	
	DelayNums(2000);
	printf("ok\r\n");

	

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

