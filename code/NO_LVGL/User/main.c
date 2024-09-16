#include "main.h"



static void Driver_Init(void)
{
	HAL_Init();
	SystemClock_Config();	
	Delay_Init();
	Work_Time_Init();
	USBtoComUsart_Init(115200);

	Led_DrvInit();
//	FatfsInit();
//	InFare_Init();
	IIC_Init();
	SPI1_Init();
	Calendar_Init();
//	Modbus_Init();
	ESP8266_Init();
//	NOFlash_Init();
}






int main()
{

	Driver_Init();
	DelayNums(2000);

	printf("ok\r\n");
	//	FatfsInit();

	

	while(1)
	{

		CheckWifiModuleWork();
		DelayNums(100);

	
	}
	return 0;
}

