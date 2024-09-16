#include "KEY.h"
#include "led_driver.h"
#include "Systick.h"

typedef struct
{
	GPIO_TypeDef*  KEY_PRO;   /* GPIO端口 */
	uint32_t  KEY_Pin;	      /* GPIO引脚口 */
	IRQn_Type EXIT_IRQn;      /* 中断线 */
}KEY_t;

typedef enum
{
	KEY_RELEASE = 0,
	KEY_CONFIRM,
	KEY_SHORTRESS,
	KEY_LONGPRESS
}KEY_STATE;

typedef struct
{
	KEY_STATE keyStatus;
	uint64_t prvSystime;
}Key_Info_t;

KEY_t Key_List[] = 
{
	{GPIOA,GPIO_PIN_0,EXTI0_IRQn},
	{GPIOG,GPIO_PIN_15,EXTI15_10_IRQn},
	{GPIOC,GPIO_PIN_13,EXTI15_10_IRQn},
	{GPIOE,GPIO_PIN_3,EXTI3_IRQn}
};

#define KEY1        0
#define KEY2        1
#define KEY3        2
#define KEY4        3
#define KEY_MAX()   sizeof(Key_List)/sizeof(KEY_t)
	

Key_Info_t g_KeyInfo[KEY_MAX()];

void KEY_Task(void)
{
		uint8_t i = GetKayVal();
		if(i == KEY1_GET)
		{
			ToogleLed(LED_BLUE);
		}
		if(i == KEY2_GET)
		{
			ToogleLed(LED_RED);
		}
		if(i == KEY3_GET)
		{
			ToogleLed(LED_GREEN);
		}
		if(i == KEY4_GET)
		{
			ToogleLed(LED_BLUE);
		}		
}
/**
*******************************
* @brief : 按键初始化
* @parm  ：void
* @return ：void
*******************************
*/	
void KEY_Init(void)
{
	GPIO_InitTypeDef tmp;
	for(uint8_t i = 0;i < KEY_MAX();i++)
	{
		GPIO_CLK_ENABLE(Key_List[i].KEY_PRO);
		tmp.Mode = GPIO_MODE_INPUT;/* 输入模式 */
	  tmp.Pin = Key_List[i].KEY_Pin;
		tmp.Pull = GPIO_PULLUP;
		tmp.Speed = GPIO_SPEED_FREQ_LOW;
//		tmp.Mode  = GPIO_MODE_IT_FALLING;
//		tmp.Pin = Key_List[i].KEY_Pin;
//		tmp.Pull = GPIO_PULLUP;
//		tmp.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(Key_List[i].KEY_PRO,&tmp);
//		HAL_NVIC_SetPriority(Key_List[i].EXIT_IRQn,0,0);
//		HAL_NVIC_EnableIRQ(Key_List[i].EXIT_IRQn);
		
	}
	
}

/**
*******************************
* @brief : 按键扫描
* @parm  ：keynum---要进行读取的按键
* @return ：uint8_t --- 按键的状态
*******************************
*/
static uint8_t KeyScan(uint8_t keynum)
{
	GPIO_PinState ReadPin = HAL_GPIO_ReadPin(Key_List[keynum].KEY_PRO
														,Key_List[keynum].KEY_Pin);
	switch(g_KeyInfo[keynum].keyStatus)
	{
		case  KEY_RELEASE:
			if(ReadPin == GPIO_PIN_RESET)  /* 被按下 */
			{
				g_KeyInfo[keynum].keyStatus = KEY_CONFIRM; /* 开始消抖 */
				g_KeyInfo[keynum].prvSystime = Get_Work_Time(); /* 获取当前系统时间 */
			}
			break;
		case  KEY_CONFIRM:
			if(ReadPin == GPIO_PIN_RESET)  /* 被按下 */
			{
				if(Get_Work_Time() - g_KeyInfo[keynum].prvSystime > 10 )
				{
					g_KeyInfo[keynum].keyStatus = KEY_SHORTRESS; /* 短按 */
				}
				
			}
			else 
			{
				g_KeyInfo[keynum].keyStatus = KEY_RELEASE;
			}
			break;
		case  KEY_SHORTRESS :
			if(ReadPin == GPIO_PIN_RESET)  /* 被按下 */
			{
				if(Get_Work_Time() - g_KeyInfo[keynum].prvSystime > 1000 )
				{
					g_KeyInfo[keynum].keyStatus = KEY_LONGPRESS; /* 长按 */
					
				}
			}
			else
			{
				 g_KeyInfo[keynum].keyStatus = KEY_RELEASE;
				 return keynum+1;

			}
		case  KEY_LONGPRESS:
		  if(ReadPin == GPIO_PIN_SET)  /* 长按松开 */
			{
					g_KeyInfo[keynum].keyStatus = KEY_RELEASE;
					return keynum + 0x81 ;
			}
			break;
		default:
			g_KeyInfo[keynum].keyStatus = KEY_RELEASE;
			break;
			
	}
	return 0;
}

/**
*******************************
* @brief : 系统性按键扫描
* @parm  ：void
* @return ：uint8_t 按键状态
*******************************
*/
uint8_t GetKayVal(void)
{
	uint8_t ret = 0;
	for(uint8_t i = 0; i < KEY_MAX() ;i++)
	{
		ret = KeyScan(i);
		if(ret != 0)
		{
			break;
		}
	}
	return ret;
}

//void EXTI3_IRQHandler(void)
//{
//	if(__HAL_GPIO_EXTI_GET_IT(Key_List[KEY4].KEY_Pin) != RESET)
//	{
//		ToogleLed(LED_RED);
//		__HAL_GPIO_EXTI_CLEAR_IT(Key_List[KEY4].KEY_Pin);
//	}

//}	

//void EXTI0_IRQHandler(void)
//{
//	if(__HAL_GPIO_EXTI_GET_IT(Key_List[KEY1].KEY_Pin) != RESET)
//	{
//		ToogleLed(LED_GREEN);
//		__HAL_GPIO_EXTI_CLEAR_IT(Key_List[KEY1].KEY_Pin);
//	}
//}
//void EXTI15_10_IRQHandler(void)
//{
//	if(__HAL_GPIO_EXTI_GET_IT(Key_List[KEY3].KEY_Pin) != RESET)
//	{
//		ToogleLed(LED_BLUE);
//		__HAL_GPIO_EXTI_CLEAR_IT(Key_List[KEY3].KEY_Pin);
//	}
//	
//	if(__HAL_GPIO_EXTI_GET_IT(Key_List[KEY2].KEY_Pin) != RESET)
//	{
//		ToogleLed(LED_RED);
//		__HAL_GPIO_EXTI_CLEAR_IT(Key_List[KEY2].KEY_Pin);
//	}
//}
