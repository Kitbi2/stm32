#include "InFared.h"

TIM_HandleTypeDef Tim_handler;
TIM_IC_InitTypeDef Chandl_Confing;

	uint16_t time = 0;

/**
*******************************
* @brief : ��ʼ�����벶���GPIO
* @parm  ��void
* @return ��void
*******************************
*/
static void TIM_IC_GPIO_Init(void)
{
	/* ʹ����Ҫ��GPIOʱ�� */
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef InFared_Gpio;        
	InFared_Gpio.Mode = GPIO_MODE_INPUT;
	InFared_Gpio.Pin = GPIO_PIN_7;
	InFared_Gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	InFared_Gpio.Pull = GPIO_NOPULL;
	
	/* ��ʼ��GPIO */
	HAL_GPIO_Init(GPIOC,&InFared_Gpio);
	
}	
/**
*******************************
* @brief : ��ʼ�����벶��Ķ�ʱ��
* @parm  ��void
* @return ��void
*******************************
*/
static void TIM_IC_Init(void)
{
	/* ʹ�ܶ�ʱ��ʱ�� */
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	/* ��������ӳ�� */
	__HAL_AFIO_REMAP_TIM3_ENABLE();
	/* ���ö�ʱ�� */
	Tim_handler.Instance = TIM3;
	Tim_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim_handler.Init.Prescaler = 72-1;
	Tim_handler.Init.Period = 65535-1;
	HAL_TIM_IC_Init(&Tim_handler);
	
	/* ��ʱ����ͨ������ */
	Chandl_Confing.ICPolarity = TIM_ICPOLARITY_FALLING;
	Chandl_Confing.ICSelection = TIM_ICSELECTION_DIRECTTI;
	Chandl_Confing.ICFilter = 0;
	Chandl_Confing.ICPrescaler = TIM_ICPSC_DIV1; 
	
	HAL_TIM_IC_ConfigChannel(&Tim_handler,&Chandl_Confing,TIM_CHANNEL_2);
	
	/* �ж�ʹ�ܺ����� */
	HAL_NVIC_SetPriority(TIM3_IRQn,1,1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	/* ��ʼ���� */
	HAL_TIM_IC_Start_IT(&Tim_handler,TIM_CHANNEL_2);	
	
	
}

#define IndrQUEUE_BUF_MAX 10
Queue_t InDrqueue;
uint8_t InDrQueue_Buf[IndrQUEUE_BUF_MAX ];

/**
*******************************
* @brief : ����ң�ؽ��ܳ�ʼ��
* @parm  ��void
* @return ��void
*******************************
*/
void InFare_Init(void)
{
	TIM_IC_GPIO_Init();
	TIM_IC_Init();
	/* ��ʼ�����ݶ��� */
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
* @brief : ���ݴ�����
* @parm  ��void
* @return ��void
*************************
*/
static void ParseIrFrame(uint16_t time)
{
	static uint8_t Head_Flag = 0;
	static uint8_t Parese_buf[32] = {0};
	static uint8_t Inedex = 0;
	
	/* �ж�ʱ���Ƿ�Ϊͷ */
	if(time > HEAD_TIME_MIN &&  time < HEAD_TIME_MAX)
	{
		Head_Flag = 1;
	}
	
	/* û��ͷ�����ҷ��͹�����ʱ�䲻��ͷֱ�Ӻ��� */
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
		if(Parese_buf[0] == (uint8_t)~Parese_buf[1]  && Parese_buf[2] == (uint8_t)~Parese_buf[3] ) /* �ж������Ƿ���� */
		{	
			/* �������ȥ */
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
	/* �ж��Ƿ�Ϊ�����ж� */
	if(__HAL_TIM_GET_FLAG(&Tim_handler,TIM_FLAG_CC2))
	{
		
		time = HAL_TIM_ReadCapturedValue(&Tim_handler,TIM_CHANNEL_2);		
		__HAL_TIM_SET_COUNTER(&Tim_handler,0);
		/* �����־λ */
		__HAL_TIM_CLEAR_FLAG(&Tim_handler,TIM_FLAG_CC2);
		/* ��ȡ��ֵ */

		/* ��ռ����� */
		
		/* ���ݴ������ */
//		printf("time is %d\r\n",time);
		ParseIrFrame(time);
	}
	
}


