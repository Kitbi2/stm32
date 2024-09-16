#include "stm32f1xx_hal.h"
#include "delay.h"
#include "stdio.h"
#include "LCD.h"


extern uint8_t  assiic8x16 [][16] ;
SRAM_HandleTypeDef LCD_FSCM_Handler;
FSMC_NORSRAM_TimingTypeDef LCD_FSCM_time;


/* LCD依赖的背光定时器 */
TIM_HandleTypeDef tim_handle;
	TIM_OC_InitTypeDef pwm_mode;

//设置x
#define LCD_SETX_CODE  			0x2A
//设置y
#define LCD_SETY_CODE  			0x2B
//设置扫描方式
#define LCD_SETSCAN_CODE 		0x36
//写数据
#define LCD_WRITEDATA_CODE 	0x2C
//设备ID
#define LCD_DEVID           0x9488


//320 480
typedef struct
{
	__IO uint16_t reg;
	__IO uint16_t data;
}LCD_t;



#define LCD  ((__IO LCD_t*)(0x6C000000 + (1<<24) - 2))

/**
*******************************
* @brief : 对LCD进行写入命令操作
* @parm  ：void
* @return ：void
*******************************
*/
static void LCD_Write_Cmd(uint16_t cmd)
{
	LCD->reg = cmd;
}
/**
*******************************
* @brief : 对LCD进行写入数据操作
* @parm  ：void
* @return ：void
*******************************
*/
static void LCD_Write_Data(uint16_t data)
{
  LCD->data = data;
}
/**
*******************************
* @brief : 对LCD进行读取数据操作
* @parm  ：void
* @return ：void
*******************************
*/
uint16_t LCD_Read_data(void)
{
	uint16_t ram; 
	ram = LCD->data;
	return ram;
}
/**
*******************************
* @brief : 设置一个LCD的矩阵
* @parm  ：void
* @return ：void
*******************************
*/
static void LCD_SetCursor(uint16_t x_start,uint16_t x_end,uint16_t y_start,uint16_t y_end)
{
	LCD_Write_Cmd(LCD_SETX_CODE);
	/* 发送x_start的高8位 */
	LCD_Write_Data((uint8_t)(x_start >> 8));
	/* 发送x_start的低8位 */
	LCD_Write_Data((uint8_t)(x_start & 0xff));
	/* 发送x_end的高8位 */
	LCD_Write_Data((uint8_t)((x_end - 1) >> 8));
	/* 发送x_end的低8位 */
	LCD_Write_Data((uint8_t)((x_end - 1) & 0xff));
	
	LCD_Write_Cmd(LCD_SETY_CODE);
	/* 发送x_start的高8位 */
	LCD_Write_Data(y_start >> 8);
	/* 发送x_start的低8位 */
	LCD_Write_Data(y_start & 0xff);
	/* 发送x_end的高8位 */
	LCD_Write_Data((y_end - 1) >> 8);
	/* 发送x_end的低8位 */
	LCD_Write_Data((y_end - 1)& 0xff);	
	
}
//uint16_t * addr = NULL;
//uint16_t dma_x_start = 0;
//uint16_t dma_x_end = 0;

//uint16_t dma_y_start = 0;
//uint16_t dma_y_end = 0;

DMA_HandleTypeDef LCD_DMA;
static void LCD_DMA_Init(void)
{
	/* 使艤DMA謩时讚 */
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* 皮變DMA1 */
	LCD_DMA.Instance = DMA1_Channel4;
	LCD_DMA.Init.Direction = DMA_MEMORY_TO_MEMORY; /* 微狮战艢咋 */
	LCD_DMA.Init.PeriphInc = DMA_PINC_DISABLE;  /* 微狮一財斩 */
	LCD_DMA.Init.MemInc = DMA_MINC_ENABLE;      /* 艢咋財斩 */
	
	LCD_DMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* 艢咋为8位 */
	LCD_DMA.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* 微狮也藝8位 */
	
	LCD_DMA.Init.Mode = DMA_NORMAL; /* 貒循鄯模式 */
	LCD_DMA.Init.Priority = DMA_PRIORITY_VERY_HIGH;  /* 詤袌芏貒樱贌 */
	HAL_DMA_Init(&LCD_DMA);
	
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn,1,1);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

//void test_handler(void)
//{
//		LCD_SetCursor(dma_x_start,dma_x_end,dma_y_start ,dma_y_end);
//		
//		HAL_DMA_Start_IT(&LCD_DMA,(uint32_t)addr,(uint32_t)LCD->data,dma_x_end - dma_x_start);
//		addr = addr + dma_x_end - dma_x_start;
//		dma_y_start++;
//}

//void DMA1_Channel4_IRQHandler(void)
//{
//	if(__HAL_DMA_GET_FLAG(&LCD_DMA,DMA_FLAG_TC4))
//	{
//		__HAL_DMA_CLEAR_FLAG(&LCD_DMA,DMA_FLAG_TC4);
//		
//		

//	}
//}
/**
*******************************
* @brief : 对某个域区填充单个颜色
* @parm  ：void
* @return ：uint16_t ---返回读取到的数据
*******************************
*/
void LCD_Fill_PureColor(uint16_t x_start,uint16_t x_end,uint16_t y_start,uint16_t y_end,uint16_t* color)
{
	
	uint32_t index  = 0;

//	LCD_SetCursor(x_start,x_end,y_start,y_end);
//	LCD_Write_Cmd(LCD_WRITEDATA_CODE);
//	HAL_DMA_Start_IT(&LCD_DMA,(uint32_t)color,(uint32_t)&LCD->data,(x_end - x_start + 1) * (y_end - y_start + 1));
//	addr  = color + x_end - x_start;
//	dma_x_start =  x_start;
//	dma_x_end = x_end;
//	dma_y_start = y_start + 1;
//	dma_y_end = y_end;
	for(uint32_t i = 0;i < (y_end - y_start + 1);i++)
	{
		/* 从新定位 */
		LCD_SetCursor(x_start,x_end,y_start + i,y_end);
		LCD_Write_Cmd(LCD_WRITEDATA_CODE);

		for(uint32_t j = 0;j < (x_end - x_start + 1);j++)
		{
				LCD_Write_Data(color[index]);
				index++;
		}
	}
	
}
/**
*******************************
* @brief : 清屏操作
* @parm  ：void
* @return ：void
*******************************
*/
void LCD_Clear(uint16_t color)
{
	LCD_SetCursor(0,340,0,480);
	LCD_Write_Cmd(LCD_WRITEDATA_CODE);
	for(uint32_t i = 0;i < (340 - 1) * (480 - 1);i++)
	{
		LCD_Write_Data(color);
	}	
}

/**
*******************************
* @brief : 画点
* @parm  ：void
* @return :
*******************************
*/
void LCD_Write_Poin(uint16_t x_start,uint16_t y_start,uint16_t color)
{
	LCD_SetCursor(x_start,x_start,y_start,y_start);
	LCD_Write_Cmd(LCD_WRITEDATA_CODE);
	LCD_Write_Data(color);
	
}

/**
*******************************
* @brief : 显示图片
* @parm  ：void
* @return ：void
*******************************
*/
void LCD_Show_Pic(uint16_t x_start,uint16_t x_end,uint16_t y_start,uint16_t y_end,uint16_t *color)
{
	uint32_t buf_targ = 0;
//	LCD_SetCursor(x_start,x_end,y_start,y_end);
//	LCD_Write_Cmd(LCD_WRITEDATA_CODE);
	for(uint32_t i = 0;i < (y_end - y_start - 1);i++)
	{
		/* 从新定位 */
		LCD_SetCursor(x_start,x_end,y_start + i,y_end);
		LCD_Write_Cmd(LCD_WRITEDATA_CODE);
		for(uint32_t j = 0;j < (x_end - x_start - 1);j++)
		{
				LCD_Write_Data(color[buf_targ]);
				buf_targ++;
		}
	}		

}
/**
*******************************
* @brief : 显示ASSCI字符
* @parm  ：void
* @return ：void
*******************************
*/
void LCD_Show_assci(uint16_t x_start,uint16_t y_start,uint16_t charsize,char targ)
{
		if(charsize != ASSCII16 && charsize != ASSCII24)
			return;
		
			
		uint16_t char_width = charsize / 2 ; /* 宽度 */
		uint16_t char_hight = charsize;     /* 高度 */
		uint16_t char_Index =  targ - ' ' ;              /* 下标 */
		uint8_t  * char_buf  = NULL;
		uint16_t fontHasByte =  (char_width /8 + (char_width % 8 ? 1: 0))*  charsize  ; /* 总共需要多少字节 */

		if(charsize == ASSCII16)
		{
			char_buf =  assiic8x16[char_Index];   
		}
		else if(charsize == ASSCII24)
		{
			
		}
		
		

		
		uint16_t char_width_tmp =  0 ;
		for(uint16_t i = 0;i < fontHasByte;i++)
		{
			
			uint8_t data = char_buf[i];
			/* 一个字符的一行lcd显示完成 */
			if(char_width_tmp == 0)
			{				
				
				/* 调整位置 */
				char_width_tmp = charsize / 2;
				LCD_SetCursor(x_start,x_start + charsize/2 - 1,y_start + i,y_start + char_hight);			
				LCD_Write_Cmd(LCD_WRITEDATA_CODE);	
			
			}
			
			for(uint16_t j = 0;j < 8;j++)
			{
				
				if(data & 0x80)
				{
					LCD_Write_Data(0);
				}
				else
				{
					LCD_Write_Data(0xffff);
				}
				data <<= 1;
				char_width_tmp--;
				/* 显示完成 */
				if(char_width_tmp == 0)
					break;
				
			}
		}
	
}





/**
*******************************
* @brief : 初始化LCD需要的IO 也就是只有一个复位线
* @parm  ：void
* @return ：void
*******************************
*/
static void LCD_GPIO_Init(void)
{
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	/* 初始化GPIO */
	GPIO_InitTypeDef gpio_int_struct;
	gpio_int_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_int_struct.Pull = GPIO_PULLUP;
	gpio_int_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_int_struct.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOF,&gpio_int_struct);
}
/**
*******************************
* @brief : 初始化定时器的背光
* @parm  ：void
* @return ：void
*******************************
*/
static void Backlight_Init(void)
{
	/* IO初始化 */
	__HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC的时钟
	__HAL_RCC_AFIO_CLK_ENABLE();    //使能AFIO的时钟
	
	GPIO_InitTypeDef Gpio_Init_struct;
	Gpio_Init_struct.Mode = GPIO_MODE_AF_PP; //复用推挽输出
	Gpio_Init_struct.Pin = GPIO_PIN_6; 
	Gpio_Init_struct.Pull = GPIO_PULLDOWN;
	Gpio_Init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC,&Gpio_Init_struct);
	

	/* 配置定时器 */
	__HAL_AFIO_REMAP_TIM3_ENABLE(); //定时器通道重映射
	__HAL_RCC_TIM3_CLK_ENABLE();    //使能定时器3的时钟
	
	tim_handle.Instance = TIM3;         //基地址
	tim_handle.Init.Prescaler = 72 - 1; //预分频
	tim_handle.Init.Period = 1000;      //计数器
	tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;  //计数模式
	HAL_TIM_PWM_Init(&tim_handle);     //配置定时器为pwm模式
	
	pwm_mode.OCMode = TIM_OCMODE_PWM1;
	pwm_mode.Pulse = 500;
	pwm_mode.OCPolarity = TIM_OCPOLARITY_LOW;
	HAL_TIM_PWM_ConfigChannel(&tim_handle,&pwm_mode,TIM_CHANNEL_1);
	
	HAL_TIM_PWM_Start(&tim_handle,TIM_CHANNEL_1);
	
	
	
}
/**
*******************************
* @brief : 初始化LCD依赖的FSMC
* @parm  ：void
* @return ：void
*******************************
*/
static void LCD_FSMC_Init(void)
{

	/* LCD依赖的FSCM */

	__HAL_RCC_FSMC_CLK_ENABLE();
	LCD_FSCM_Handler.Instance = FSMC_NORSRAM_DEVICE;
	LCD_FSCM_Handler.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	LCD_FSCM_Handler.Init.NSBank =  FSMC_NORSRAM_BANK4; /* 第三区域  */
	LCD_FSCM_Handler.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM; /* 存储截至位sram */
	LCD_FSCM_Handler.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16; /* 数据宽度 */
	LCD_FSCM_Handler.Init.BurstAccessMode = FSMC_NORSRAM_FLASH_ACCESS_DISABLE; /* 忽略突发情况 */
	LCD_FSCM_Handler.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	LCD_FSCM_Handler.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
	LCD_FSCM_Handler.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE; /* 不使能复用 */

	LCD_FSCM_time.AccessMode = FSMC_ACCESS_MODE_A;
	LCD_FSCM_time.AddressHoldTime = 0x00;
	LCD_FSCM_time.AddressSetupTime = 0x00;
	LCD_FSCM_time.DataSetupTime = 0x02;	
	
	if(HAL_SRAM_Init(&LCD_FSCM_Handler,&LCD_FSCM_time,&LCD_FSCM_time) != HAL_OK)
	{
		printf("fsmc init error\r\n");
	}
	LCD_DMA_Init();

}


/**
*******************************
* @brief : 初始化LCD
* @parm  ：void
* @return ：void
*******************************
*/
void LCD_Init(void)
{
	LCD_GPIO_Init();
	Backlight_Init();
	LCD_FSMC_Init();
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_RESET);
	DelayNums(50);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_SET);
	DelayNums(50);

	uint16_t ret = 0;
	LCD_Write_Cmd(0xD3);
	LCD_Read_data();
	LCD_Read_data();
	
	ret = LCD_Read_data();
	ret  <<= 8;
	LCD_Read_data();
	ret |= LCD_Read_data();
	if(ret == LCD_DEVID)
	{
		printf("the lcd id is %x\r\n",ret);
		
 LCD_Write_Cmd(0xE0); //P-Gamma   
	LCD_Write_Data(0x00);// LCD_Write_Data(0x00);   
	LCD_Write_Data(0x07);// LCD_Write_Data(0x13);   
	LCD_Write_Data(0x10);// LCD_Write_Data(0x18);   
	LCD_Write_Data(0x09);// LCD_Write_Data(0x04);   
	LCD_Write_Data(0x17);// LCD_Write_Data(0x0F);   
	LCD_Write_Data(0x0B);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x41);// LCD_Write_Data(0x3A);   
	LCD_Write_Data(0x89);// LCD_Write_Data(0x56);   
	LCD_Write_Data(0x4B);// LCD_Write_Data(0x4D);   
	LCD_Write_Data(0x0A);// LCD_Write_Data(0x03);   
	LCD_Write_Data(0x0C);// LCD_Write_Data(0x0A);   
	LCD_Write_Data(0x0E);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x18);// LCD_Write_Data(0x30);   
	LCD_Write_Data(0x1B);// LCD_Write_Data(0x3E);   
	LCD_Write_Data(0x0F);// LCD_Write_Data(0x0F);   
	   
	LCD_Write_Cmd(0xE1); //N-Gamma   
	LCD_Write_Data(0x00);// LCD_Write_Data(0x00);   
	LCD_Write_Data(0x17);// LCD_Write_Data(0x13);   
	LCD_Write_Data(0x1A);// LCD_Write_Data(0x18);   
	LCD_Write_Data(0x04);// LCD_Write_Data(0x01);   
	LCD_Write_Data(0x0E);// LCD_Write_Data(0x11);   
	LCD_Write_Data(0x06);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x2F);// LCD_Write_Data(0x38);   
	LCD_Write_Data(0x45);// LCD_Write_Data(0x34);   
	LCD_Write_Data(0x43);// LCD_Write_Data(0x4D);   
	LCD_Write_Data(0x02);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x0A);// LCD_Write_Data(0x0D);   
	LCD_Write_Data(0x09);// LCD_Write_Data(0x0B);   
	LCD_Write_Data(0x32);// LCD_Write_Data(0x31);   
	LCD_Write_Data(0x36);// LCD_Write_Data(0x37);   
	LCD_Write_Data(0x0F);// LCD_Write_Data(0x0F);    
	   
    // DB_EN = 0
    LCD_Write_Cmd(0xE9);  
    LCD_Write_Data(0x00);
    
    // Adjust Control
    LCD_Write_Cmd(0xF2);
    LCD_Write_Data(0x58);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x12);
    LCD_Write_Data(0x02);
    LCD_Write_Data(0x22);
    LCD_Write_Data(0x42);
    LCD_Write_Data(0xFF);
    LCD_Write_Data(0x0A);
    LCD_Write_Data(0x90);
    LCD_Write_Data(0x14);
    LCD_Write_Data(0x88);
    
    // Adjust Control 3
    LCD_Write_Cmd(0xF7);      
	LCD_Write_Data(0xA9);   
	LCD_Write_Data(0x51);   
	LCD_Write_Data(0x2C);   
	LCD_Write_Data(0x82);  // D7 stream, loose   
    
    // Adjust Control 4
    LCD_Write_Cmd(0xF8);      
	LCD_Write_Data(0x21);   
	LCD_Write_Data(0x04); 
    
	LCD_Write_Cmd(0xC0);   //Power Control 1   
	LCD_Write_Data(0x11); //Vreg1out   
	LCD_Write_Data(0x09); //Verg2out   
	   
	LCD_Write_Cmd(0xC1);   //Power Control 2   
	LCD_Write_Data(0x41); //VGH,VGL   
	   
    
	LCD_Write_Cmd(0xC5);   //VCOM Control  
	LCD_Write_Data(0x00);  
    LCD_Write_Data(0x0A);    
	LCD_Write_Data(0x80); //Vcom   
	   
	LCD_Write_Cmd(0x36);   //Memory Access   
	LCD_Write_Data(0x08);   //48   
	   
	LCD_Write_Cmd(0x3A);   // Interface Pixel Format   
	LCD_Write_Data(0x55); //16bit   
	   
	LCD_Write_Cmd(0xB0);   // Interface Mode Control   
	LCD_Write_Data(0x00);   
	   
	LCD_Write_Cmd(0xB1);
	LCD_Write_Data(0xB0);
	LCD_Write_Data(0x11);
	   
	LCD_Write_Cmd(0xB4);   //Display Inversion Control   
	LCD_Write_Data(0x02); //2-dot   
	   
	LCD_Write_Cmd(0xB6);   //RGB/MCU Interface Control   
	LCD_Write_Data(0x02); //MCU RGB   
	LCD_Write_Data(0x22); //Source,Gate scan dieection  

	/* Entry Mode Set (B7h)  */
	LCD_Write_Cmd(0xB7);
	LCD_Write_Data(0xc6);

	/* HS Lanes Control (BEh) */
	LCD_Write_Cmd(0xBE);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x04);
    
  LCD_Write_Cmd(0xE9);    // Set Image Function   
	LCD_Write_Data(0x00);  //disable 24 bit data input  
	   
//	LCD_Write_Cmd(0x20);    //Normal Black   
	LCD_Write_Cmd(0x11);    //Sleep out   
	DelayNums(120);   
	LCD_Write_Cmd(0x29); 

//	LCD_Write_Cmd(0x36); // Memory Access Control
//	LCD_Write_Data(0x08); //?? scan_mode ???? LCD ??,? 0-7 ???
		

	}
	
}










