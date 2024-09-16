#include "led_driver.h"
#include "stm32f1xx_hal.h"


void GPIO_CLK_ENABLE(GPIO_TypeDef * GPIO_Pro)
{
	if(GPIO_Pro == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(GPIO_Pro == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(GPIO_Pro == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
		else if(GPIO_Pro == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
		else if(GPIO_Pro == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if(GPIO_Pro == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if(GPIO_Pro == GPIOG
		)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
}

typedef struct
{
	uint32_t GPIO_Pin;				 /* GPIO�����ſ� */
	GPIO_TypeDef* GPIO_Pro;  /* GPIO���� */
}Led_GPIO_t;


static Led_GPIO_t led_list[] = 
{
	{GPIO_PIN_0,GPIOB},
	{GPIO_PIN_1,GPIOB},
	{GPIO_PIN_5,GPIOB}
};

#define LED_NUM_MAX() (sizeof(led_list) / sizeof(led_list[0]))
	
/**
*******************************
* @brief : led��ʼ��
* @parm  ��void
* @return ��void
*******************************
*/
void Led_DrvInit(void)
{
	GPIO_InitTypeDef tmp;
	for(uint8_t i = 0;i < LED_NUM_MAX();i++)
	{
		/* ʹ��ʱ�� */
		GPIO_CLK_ENABLE(led_list[i].GPIO_Pro);
		/* ���ýṹ��,���г�ʼ�� */
		tmp.Mode = GPIO_MODE_OUTPUT_PP;
		tmp.Pin = led_list[i].GPIO_Pin;
		tmp.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(led_list[i].GPIO_Pro,&tmp);
		HAL_GPIO_WritePin(led_list[i].GPIO_Pro,led_list[i].GPIO_Pin,GPIO_PIN_SET);

	}
	 
}
/**
*******************************
* @brief : ����һ��led
* @parm  ��ledno---Ŀ��led
* @return ��void
*******************************
*/
void TurnOnLed(uint8_t ledNo)
{
	if(ledNo > 2)
		return ;
	HAL_GPIO_WritePin(led_list[ledNo].GPIO_Pro,led_list[ledNo].GPIO_Pin,GPIO_PIN_RESET);
}
/**
*******************************
* @brief : Ϩ��һ��led
* @parm  ��ledNo---Ŀ��led
* @return ��void
*******************************
*/
void TurnOffLed(uint8_t ledNo)
{
	if(ledNo > 2)
		return ;
	HAL_GPIO_WritePin(led_list[ledNo].GPIO_Pro,led_list[ledNo].GPIO_Pin,GPIO_PIN_SET);

	
}
/**
*******************************
* @brief : led�ƽ��е�ƽ��ת
* @parm  ��ledNo---Ŀ��led
* @return ��void
*******************************
*/
void ToogleLed(uint8_t ledNo)
{
	if(ledNo > 2)
		return ;
	HAL_GPIO_TogglePin(led_list[ledNo].GPIO_Pro,led_list[ledNo].GPIO_Pin);
}





