#include "stm32f1xx_hal.h"
#include "stdio.h"

#define SRAM_ADDR       0X68000000

//static uint8_t sram_buf[255] __attribute__ ((at(SRAM_ADDR)));
SRAM_HandleTypeDef Sram_Handler;
FSMC_NORSRAM_TimingTypeDef Sram_time;
	
// PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD11 PD12 PD13 PD14 PD15
#define FSMC_GPIOD_PIN ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 \
	| GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
// PE0 PE1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15
#define FSMC_GPIOE_PIN ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 \
	| GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
// PF0 PF1 PF2 PF3 PF4 PF5 PF12 PF13 PF14 PF15
#define FSMC_GPIOF_PIN ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 \
	| GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
// PG0 PG1 PG2 PG3 PG4 PG5 PG10
#define FSMC_GPIOG_PIN ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10 | GPIO_PIN_12)

/**
*******************************
* @brief : FSMC_GPIO的初始化
* @parm  ：void
* @return ：void
*******************************
*/

static void FSMC_GPIO_Init(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	/* 数据线 */
	GPIO_InitTypeDef gpio_int_struct;
	gpio_int_struct.Mode = GPIO_MODE_AF_PP;
	gpio_int_struct.Pull = GPIO_PULLUP;
	gpio_int_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	gpio_int_struct.Pin = FSMC_GPIOD_PIN;
	HAL_GPIO_Init(GPIOD,&gpio_int_struct);
	
	gpio_int_struct.Pin = FSMC_GPIOE_PIN;
	HAL_GPIO_Init(GPIOE,&gpio_int_struct);
	
	gpio_int_struct.Pin = FSMC_GPIOF_PIN;
	HAL_GPIO_Init(GPIOF,&gpio_int_struct);

	gpio_int_struct.Pin = FSMC_GPIOG_PIN;
	HAL_GPIO_Init(GPIOG,&gpio_int_struct);

}
/**
*******************************
* @brief : FSMC初始化
* @parm  ：void
* @return ：void
*******************************
*/
void FSMC_Init(void)
{

	
	FSMC_GPIO_Init();
	__HAL_RCC_FSMC_CLK_ENABLE();
	Sram_Handler.Instance = FSMC_NORSRAM_DEVICE;
	Sram_Handler.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	Sram_Handler.Init.NSBank =  FSMC_NORSRAM_BANK3; /* 第三区域  */
	Sram_Handler.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM; /* 存储截至位sram */
	Sram_Handler.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16; /* 数据宽度 */
	Sram_Handler.Init.BurstAccessMode = FSMC_NORSRAM_FLASH_ACCESS_DISABLE; /* 忽略突发情况 */
	Sram_Handler.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	Sram_Handler.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
	Sram_Handler.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE; /* 不使能复用 */

	Sram_time.AccessMode = FSMC_ACCESS_MODE_A;
	Sram_time.AddressHoldTime = 0x00;
	Sram_time.AddressSetupTime = 0x00;
	Sram_time.DataSetupTime = 0x02;	
	
	if(HAL_SRAM_Init(&Sram_Handler,&Sram_time,&Sram_time) != HAL_OK)
	{
		printf("fsmc init error\r\n");
	}
}

void Sram_Write(uint8_t * data,uint8_t len,uint32_t addr)
{
	uint8_t *really_addr = (uint8_t*)SRAM_ADDR + addr;
	
	for(uint8_t i = 0;i < len;i++)
	{
				printf("ok\r\n");

		*really_addr = *data;
		really_addr++;
		data++;
	}
	
}
void Sram_Read(uint8_t * data,uint8_t len,uint32_t addr)
{
	uint8_t *really_addr = (uint8_t*)SRAM_ADDR + addr;
	
	for(uint8_t i = 0;i < len;i++)
	{
		
		*data = *really_addr;
		really_addr++;
		data++;
	}
	
}

void Sram_test(void)
{
	uint8_t sram_buf[200] = {0};
	for(uint8_t i = 0;i < 200;i++)
	{
		sram_buf[i] = i;
	}

	
	for(uint8_t i = 0;i < 200;i++)
	{
		printf("the r_arr[%d] --- %d\r\n",i,sram_buf[i]);
	}	
}


