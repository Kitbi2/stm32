#include "InFared.h"

TIM_HandleTypeDef Tim_handler;
TIM_IC_InitTypeDef Chandl_Confing;

	uint16_t time = 0;

/**
*******************************
* @brief : 初始化输入捕获的GPIO
* @parm  ：void
* @return ：void
*******************************
*/
static void TIM_IC_GPIO_Init(void)
{
	/* 使能需要的GPIO时钟 */
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef InFared_Gpio;        
	InFared_Gpio.Mode = GPIO_MODE_INPUT;
	InFared_Gpio.Pin = GPIO_PIN_7;
	InFared_Gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	InFared_Gpio.Pull = GPIO_NOPULL;
	
	/* 初始化GPIO */
	HAL_GPIO_Init(GPIOC,&InFared_Gpio);
	
}	
/**
*******************************
* @brief : 初始化输入捕获的定时器
* @parm  ：void
* @return ：void
*******************************
*/
static void TIM_IC_Init(void)
{
	/* 使能定时器时钟 */
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	/* 进行重新映射 */
	__HAL_AFIO_REMAP_TIM3_ENABLE();
	/* 配置定时器 */
	Tim_handler.Instance = TIM3;
	Tim_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim_handler.Init.Prescaler = 72-1;
	Tim_handler.Init.Period = 65535-1;
	HAL_TIM_IC_Init(&Tim_handler);
	
	/* 定时器的通道配置 */
	Chandl_Confing.ICPolarity = TIM_ICPOLARITY_FALLING;
	Chandl_Confing.ICSelection = TIM_ICSELECTION_DIRECTTI;
	Chandl_Confing.ICFilter = 0;
	Chandl_Confing.ICPrescaler = TIM_ICPSC_DIV1; 
	
	HAL_TIM_IC_ConfigChannel(&Tim_handler,&Chandl_Confing,TIM_CHANNEL_2);
	
	/* 中断使能和设置 */
	HAL_NVIC_SetPriority(TIM3_IRQn,1,1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	/* 开始捕获 */
	HAL_TIM_IC_Start_IT(&Tim_handler,TIM_CHANNEL_2);	
	
	
}

#define IndrQUEUE_BUF_MAX 10
Queue_t InDrqueue;
uint8_t InDrQueue_Buf[IndrQUEUE_BUF_MAX ];

/**
*******************************
* @brief : 红外遥控接受初始化
* @parm  ：void
* @return ：void
*******************************
*/
void InFare_Init(void)
{
	TIM_IC_GPIO_Init();
	TIM_IC_Init();
	/* 初始化数据队列 */
	QueueInit(&InDrqueue,IndrQUEUE_BUF_MAX,InDrQueue_Buf);
}

#define HEAD_TIME_MAX 14000
#define HEAD_TIME_MIN 12000

#define DATA_ZERO_MAX 2300
#define DATA_ZERO_MIN 2100

#define DATA_ONE_MAX 1200
#define DATA_ONE_MIN 200



/**
*******************************
* @brief : 数据处理函数
* @parm  ：void
* @return ：void
*************************
*/
static void ParseIrFrame(uint16_t time)
{
	static uint8_t Head_Flag = 0;
	static uint8_t Parese_buf[32] = {0};
	static uint8_t Inedex = 0;
	
	/* 判断时间是否为头 */
	if(time > HEAD_TIME_MIN &&  time < HEAD_TIME_MAX)
	{
		Head_Flag = 1;
	}
	
	/* 没有头，并且发送过来的时间不是头直接忽略 */
	if(!Head_Flag)
	{
		return;
	}
	
	Parese_buf[Inedex / 8]  >>= 1;	
	if(time > DATA_ZERO_MIN &&  time < DATA_ZERO_MAX)
	{
		

	}
	else if(time > DATA_ONE_MIN &&  time <  DATA_ONE_MAX )
	{
		
		Parese_buf[Inedex / 8] |= 0x80;
	}
	Inedex++;
	if(Inedex == 32)
	{
		Inedex = 0;
		if(Parese_buf[0] == (uint8_t)~Parese_buf[1]  && Parese_buf[2] == (uint8_t)~Parese_buf[3] ) /* 判断数据是否合理 */
		{	
			/* 入队列中去 */
			QueuePush(&InDrqueue,Parese_buf[2]);
	
		}
	}
	
}

void InFared_Task(void)
{
	uint8_t data = 0;
	while(QueuePop(&InDrqueue,&data) == QUEUE_OK)
	{
		printf("the data is %x\r\n",data);
	}
}
void TIM3_IRQHandler(void)
{
	/* 判断是否为捕获中断 */
	if(__HAL_TIM_GET_FLAG(&Tim_handler,TIM_FLAG_CC2))
	{
		
		time = HAL_TIM_ReadCapturedValue(&Tim_handler,TIM_CHANNEL_2);		
		__HAL_TIM_SET_COUNTER(&Tim_handler,0);
		/* 清理标志位 */
		__HAL_TIM_CLEAR_FLAG(&Tim_handler,TIM_FLAG_CC2);
		/* 读取数值 */

		/* 清空计数器 */
		
		/* 数据处理操作 */
//		printf("time is %d\r\n",time);
		ParseIrFrame(time);
	}
	
}


