#include  "stm32f1xx_hal.h"
#include "stdio.h"
TIM_HandleTypeDef  timer3;
TIM_IC_InitTypeDef  timer3_input_channl3;

/**
*******************************
* @brief :初始化GPIO
* @parm  ：void
* @return：void 
*******************************
*/
static void GPIO_Init(void)
{
	/* 使能GPIO的时钟 */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	__HAL_AFIO_REMAP_TIM3_ENABLE();
	/* 初始化GPIO */
	GPIO_InitTypeDef gpio_strucd;
	gpio_strucd.Mode = GPIO_MODE_INPUT;  /* 输入 */
	gpio_strucd.Pin = GPIO_PIN_7;				 /* PIN8 */
	gpio_strucd.Pull = GPIO_NOPULL;			 /* 浮空 */
	gpio_strucd.Speed = GPIO_SPEED_FREQ_HIGH;   /* 高速 */
	
	HAL_GPIO_Init(GPIOC,&gpio_strucd);
}


/**
*******************************
* @brief :初始化定时器
* @parm  ：void
* @return：void 
*******************************
*/
static void Timer_Init(void)
{
	/* 使能时钟 */
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	/* 初始化定时器 */
	timer3.Instance = TIM3;        
	timer3.Init.Period = 65534-1;   /* 从装载值 */
	timer3.Init.Prescaler = 72-1;   /* 预分配为72，也就是每个计数值为1us */
	/* 预分配系数 */
	timer3.Init.CounterMode = TIM_COUNTERMODE_UP;   /* 计数模式 */
	
	HAL_TIM_IC_Init(&timer3);
	
	/* 初始化定时器4的通道3 */
	
	timer3_input_channl3.ICPolarity = TIM_ICPOLARITY_FALLING; /* 下升沿触发 */
	timer3_input_channl3.ICSelection = TIM_ICSELECTION_DIRECTTI; /* 映射到通道1 */
	timer3_input_channl3.ICPrescaler = TIM_ICPSC_DIV1; /* 不进行分频 */
	timer3_input_channl3.ICFilter = 0;/* 滤波器 */ 
	HAL_TIM_IC_ConfigChannel(&timer3,&timer3_input_channl3,TIM_CHANNEL_2);
	
	/* 设置优先级和使能中断 */
	HAL_NVIC_SetPriority(TIM3_IRQn,1,1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	/* 使能计数中断和捕获中断 */
	HAL_TIM_IC_Start_IT(&timer3,TIM_CHANNEL_2);
	

}
 

/**
*******************************
* @brief :初始化红外配置
* @parm  ：void
* @return：void 
*******************************
*/ 
void Parse_Init(void)
{
	GPIO_Init();
	Timer_Init();
}

#define HEAD_TIME_MAX 20000
#define HEAD_TIME_MIN 10000

#define DATA_0_TIME_MAX 1800
#define DATA_0_TIME_MIN 500

#define DATA_1_TIME_MAX 3000
#define DATA_1_TIME_MIN 1800

static uint8_t Him_buf[4];
static uint8_t him_flag = 0;
/**
*******************************
* @brief :红外协议解析函数
* @parm  ：void
* @return：void 
*******************************
*/
static void ParseIrFrame(uint32_t tickNum)
{
	static uint8_t Index = 0;
	static uint8_t head_flag = 0;
	if(tickNum < HEAD_TIME_MAX && tickNum > HEAD_TIME_MIN)
	{
		head_flag = 1;
	}
	if(!head_flag)
	{
		return;
	}
	if(tickNum > DATA_1_TIME_MIN && tickNum < DATA_1_TIME_MAX)
	{
		Him_buf[Index / 8] >>= 1;
		Him_buf[Index /  8] |= 0x80 ;
		Index++;
	}
	if(tickNum > DATA_0_TIME_MIN && tickNum < DATA_0_TIME_MAX)
	{
		Him_buf[Index / 8] >>= 1;
		Index++;
	}
	if(Index == 32)
	{
		Index = 0;
		if((Him_buf[2] & Him_buf[3]) == 0)
		{
				him_flag = 1;
		}
		
	}
	
}
/**
*******************************
* @brief :红外处理任务
* @parm  ：void
* @return：void 
*******************************
*/
void ParseTask(void)
{
	if(him_flag)
	{
		
		if(Him_buf[2] == 0x0c )
		{
			printf("trun on led\r\n");
		}
		else if(Him_buf[2] == 0x18)
		{
			printf("trun of led\r\n");
		}
		him_flag = 0;
	}
}

static uint16_t g_tim = 0;
void TIM3_IRQHandler(void)
{
	if(__HAL_TIM_GET_FLAG(&timer3,TIM_FLAG_CC2))
	{
		/* 清空标志位 */
		__HAL_TIM_CLEAR_FLAG(&timer3,TIM_FLAG_CC2);
		/* 获取时间 */
		g_tim = HAL_TIM_ReadCapturedValue(&timer3,TIM_CHANNEL_2);
		/* 清空cnt */
		__HAL_TIM_SET_COUNTER(&timer3,0);
		ParseIrFrame(g_tim);
		
	}
}


