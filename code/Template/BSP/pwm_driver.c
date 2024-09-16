
//#include "stm32f1xx_hal.h"
//	TIM_HandleTypeDef  htim3;
//TIM_OC_InitTypeDef htim3_pwm_channel3;



//static void Timer_Init(uint16_t psc,uint16_t arr,uint16_t Duty)
//{
//	/* 使能定时器3 */
//	__HAL_RCC_TIM3_CLK_ENABLE();

//	
//	/* 配置定时器3 */

//	
//	htim3.Instance = TIM3;
//	htim3.Init.Prescaler = psc - 1;
//	htim3.Init.Period  = arr - 1;
//	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//	
//	
//	HAL_TIM_PWM_Init(&htim3);
//	
//	/* 配置定时器3的channel通道3 */
//	
//	htim3_pwm_channel3.OCMode = TIM_OCMODE_PWM1;
//	htim3_pwm_channel3.Pulse = Duty;
//	htim3_pwm_channel3.OCPolarity = TIM_OCPOLARITY_LOW;
//	HAL_TIM_PWM_ConfigChannel(&htim3,&htim3_pwm_channel3,TIM_CHANNEL_3);
//	
//	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);


//}

//static void Pwm_GPIO_Init(void)
//{
//	/* 使能时钟 */
//	__HAL_RCC_AFIO_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();

//	/* 初始化IO */
//	GPIO_InitTypeDef GPIO_struct;
//	
//	GPIO_struct.Mode = GPIO_MODE_AF_PP; /* 复用推挽输出 */
//	GPIO_struct.Pin = GPIO_PIN_0;       /* 0 */
//	GPIO_struct.Pull = GPIO_PULLUP;     /* 上拉 */
//	GPIO_struct.Speed = GPIO_SPEED_FREQ_LOW; /* 低速 */
//	
//	HAL_GPIO_Init(GPIOB,&GPIO_struct);
//}
//void Pwm_Driver(uint16_t psc,uint16_t arr,uint16_t Duty )
//{
//	Pwm_GPIO_Init();
//	Timer_Init(psc,arr,Duty);
//	
//}
