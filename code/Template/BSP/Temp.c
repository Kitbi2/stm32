#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#define ADC_BUF_MAX 100
typedef struct
{
	GPIO_TypeDef * GPIO_BASE;
	GPIO_InitTypeDef GPIO_struct;  

}NTR_GPIO_t;
typedef struct
{
	TIM_HandleTypeDef 	NTR_ADC_Timer_Handler;
	TIM_MasterConfigTypeDef  Master;
}NTR_TIME_T;

typedef struct
{
	NTR_GPIO_t          NTR_GPIO;
	NTR_TIME_T          NTR_TIME;
	ADC_HandleTypeDef	 	NTR_ADC_Handler;
	DMA_HandleTypeDef  	NTR_ADC_DMA_Handler;
	uint32_t adc_buf[ADC_BUF_MAX];
	uint8_t adc_flag;
	float tempe_val; /* �¶����� */

}NTR_t;

NTR_t NTR;

/**
*******************************
* @brief : ��ʼ����Ӧ��gpio
* @parm  ��void
* @return ��void
*******************************
*/
static void NTR_GPIO_Init(void)
{
	
	/* ��ʼ��ʱ�� */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	/* ����gpio */
	NTR.NTR_GPIO.GPIO_BASE = GPIOC;
	NTR.NTR_GPIO.GPIO_struct.Mode = GPIO_MODE_AF_INPUT; //ģ������
	NTR.NTR_GPIO.GPIO_struct.Pin = GPIO_PIN_10;         //GPIO10
	NTR.NTR_GPIO.GPIO_struct.Pull = GPIO_NOPULL;        //����
	NTR.NTR_GPIO.GPIO_struct.Speed = GPIO_SPEED_FREQ_HIGH; //����

	HAL_GPIO_Init(NTR.NTR_GPIO.GPIO_BASE,&NTR.NTR_GPIO.GPIO_struct);
}

///**
//*******************************
//* @brief : ��ʼ��ADC �������
//* @parm  ��void
//* @return ��void
//*******************************
//*/
//static void ADC_Init_BySoftWork(void)
//{
//	/* ����adc��ʱ�� */
//	RCC_PeriphCLKInitTypeDef Per = {0};
//	
//	/* Ŀ����� */
//	Per.PeriphClockSelection = RCC_PERIPHCLK_ADC; // Ŀ����ʱ��
//	Per.AdcClockSelection = RCC_ADCPCLK2_DIV6; // adc��Ԥ��Ƶ
//	HAL_RCCEx_PeriphCLKConfig(&Per);
//	
//	
//	/* ����ADC */
//	__HAL_RCC_ADC1_CLK_ENABLE();/* ʹ��ADC1��ʱ�� */
//	ADC_Handler.Instance = ADC1; 	/* adc����ַ */
//	ADC_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;/* ����� */
//	ADC_Handler.Init.ScanConvMode = ADC_SCAN_DISABLE;/* ��ɨ��ģʽ */
//	ADC_Handler.Init.ContinuousConvMode =DISABLE;/* ����ת�� */
//	ADC_Handler.Init.NbrOfConversion = 1;/* ͨ����Ŀ */
//	/* ���Լ��ģʽ */
//	ADC_Handler.Init.DiscontinuousConvMode = DISABLE;
//	ADC_Handler.Init.NbrOfDiscConversion  = 0;
//	
//	ADC_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;/* ����Դ�ⲿ����  */

//	HAL_ADC_Init(&ADC_Handler);
//	
//	/* ����ADC��ͨ�� */
//	
//	ADC_ChannelConfTypeDef conf;
//	conf.Channel = 10;
//	conf.Rank = 1;
//	conf.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
//	HAL_ADC_ConfigChannel(&ADC_Handler,&conf);
//}

/**
*******************************
* @brief : �������ڴ���ADC�Ķ�ʱ��3
* @parm  ��void
* @return ��void
*******************************
*/
static void NTR_TIM_Init(void)
{
	/* ʹ��ʱ�� */
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	/* ��ʼ����ʱ�� */
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Instance = TIM3;       

	/* ��ʱ����Ƶ�� 72000000 / 720 / 1000 = 100hz Ҳ����1ms�ж�100��*/
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.Period = 1000-1;   /* ��װ��ֵ */
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.Prescaler = 720-1;   /* Ԥ����Ϊ72��Ҳ����ÿ������ֵΪ1us */
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	/* 72000khz / (50 * 72) = 20khz */
	/* Ԥ����ϵ�� */
	
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;   /* ����ģʽ */
	HAL_TIM_Base_Init(&NTR.NTR_TIME.NTR_ADC_Timer_Handler);	
	
	
	NTR.NTR_TIME.Master.MasterOutputTrigger = TIM_TRGO_UPDATE;
	NTR.NTR_TIME.Master.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&NTR.NTR_TIME.NTR_ADC_Timer_Handler,&NTR.NTR_TIME.Master);
	
	HAL_TIM_Base_Start_IT(&NTR.NTR_TIME.NTR_ADC_Timer_Handler);
	
	
	
}


/**
*******************************
* @brief : ��������adc��dma
* @parm  ��void
* @return ��void
*******************************
*/
static void NTR_DMA_Init(void)
{
	/* ʹ��ʱ�� */
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* ѡ���ʼ����ͨ�� */
	NTR.NTR_ADC_DMA_Handler.Instance = DMA1_Channel1;
	/* ѡ��ģʽ ����to�ڴ� */
	NTR.NTR_ADC_DMA_Handler.Init.Direction = DMA_PERIPH_TO_MEMORY;
	/* ���������ģʽ */
	NTR.NTR_ADC_DMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;
	/* �洢������ģʽ */
	NTR.NTR_ADC_DMA_Handler.Init.MemInc = DMA_MINC_ENABLE;
	/* ����λ�� */
	NTR.NTR_ADC_DMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	/* �ڴ�λ�� */
	NTR.NTR_ADC_DMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	/* ������ȼ� */
	NTR.NTR_ADC_DMA_Handler.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	/* ģʽ���� */
	NTR.NTR_ADC_DMA_Handler.Init.Mode = DMA_NORMAL;
	
	/* ��ʼ��dma */
	HAL_DMA_Init(&NTR.NTR_ADC_DMA_Handler);	
	
	/* ����adc�����dma����İ� */
	__HAL_LINKDMA(&NTR.NTR_ADC_Handler,DMA_Handle,NTR.NTR_ADC_DMA_Handler);
	/* ʹ��dma��ȫ�����ж� */
	__HAL_DMA_ENABLE_IT(&NTR.NTR_ADC_DMA_Handler,DMA_IT_TC);
	/* ����dma���ж�������ȼ� */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn,1,1);	
	/* ʹ��dma�ж� */
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
*******************************
* @brief : ��ʼ��ADC��ʱ������
* @parm  ��void
* @return ��void
*******************************
*/
static void NTR_ADC_Init(void)
{
	/* ����adc��ʱ�� */
	RCC_PeriphCLKInitTypeDef Per = {0};
	
	/* Ŀ����� */
	Per.PeriphClockSelection = RCC_PERIPHCLK_ADC; // Ŀ����ʱ��
	Per.AdcClockSelection = RCC_ADCPCLK2_DIV6; // adc��Ԥ��Ƶ
	HAL_RCCEx_PeriphCLKConfig(&Per);
	
	
	/* ����ADC */
	__HAL_RCC_ADC1_CLK_ENABLE();/* ʹ��ADC1��ʱ�� */
	NTR.NTR_ADC_Handler.Instance = ADC1; 	/* adc����ַ */
	NTR.NTR_ADC_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;/* �Ҷ��� */
	NTR.NTR_ADC_Handler.Init.ScanConvMode = ADC_SCAN_DISABLE;/* ��ɨ��ģʽ */
	NTR.NTR_ADC_Handler.Init.ContinuousConvMode = DISABLE;/* ���ת�� */
	NTR.NTR_ADC_Handler.Init.NbrOfConversion = 1;/* ͨ����Ŀ */
	/* ���Լ��ģʽ */
	NTR.NTR_ADC_Handler.Init.DiscontinuousConvMode = DISABLE;
	NTR.NTR_ADC_Handler.Init.NbrOfDiscConversion  = 0;
	
	NTR.NTR_ADC_Handler.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;/* ��ʱ������  */
	
	HAL_ADC_Init(&NTR.NTR_ADC_Handler);
	ADC1->CR2 |= (1 << 20);
	/* ����ADC��ͨ�� */	
	ADC_ChannelConfTypeDef conf;
	conf.Channel = 10;      /* ͨ��10���� */
	conf.Rank = 1;          /* ����һ */
	conf.SamplingTime = ADC_SAMPLETIME_7CYCLES_5; /* �������� */
	HAL_ADC_ConfigChannel(&NTR.NTR_ADC_Handler,&conf);	
	
	
	/* ����dma���ж�������ȼ� */
	HAL_NVIC_SetPriority(ADC1_2_IRQn,1,1);	
	/* ʹ��dma�ж� */
	HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
	
	
  HAL_ADC_Start_DMA(&NTR.NTR_ADC_Handler,NTR.adc_buf,ADC_BUF_MAX);
	

}
/**
*******************************
* @brief : adc����
* @parm  ��void
* @return ��void
*******************************
*/
void NTR_Init(void)
{
	NTR_GPIO_Init();
	NTR_DMA_Init();
	NTR_ADC_Init();
	NTR_TIM_Init();
}
/**
*******************************
* @brief : dma1�жϷ�����
* @parm  ��void

* @return ��void
*******************************
*/
void DMA1_Channel1_IRQHandler(void)
{
	if(__HAL_DMA_GET_FLAG(&NTR.NTR_ADC_Handler,DMA_FLAG_TC1))
	{
		HAL_ADC_Stop_DMA(&NTR.NTR_ADC_Handler);
		NTR.adc_flag = 1;
	}
}	
/**
*******************************
* @brief : ��ȡadc��һ�β���
* @parm  ��void
* @return ��uint16_t ---��ѹ�ı���ֵ
*******************************
*/

uint16_t Get_ADC_Val(void)
{
	uint16_t ret = 0;
	
	HAL_ADC_Start(&NTR.NTR_ADC_Handler);
	HAL_ADC_PollForConversion(&NTR.NTR_ADC_Handler,10);
	
	ret = HAL_ADC_GetValue(&NTR.NTR_ADC_Handler);
	return ret;
}

/**
*******************************
* @brief : ����NTR����ת��
* @parm  ��void
* @return ��uint16_t ---��ѹ�ı���ֵ
*******************************
*/
void NTR_Driver(void)
{
	
	if(NTR.adc_flag == 1)
	{
		uint32_t Really_Average = 0;
		uint32_t Average_Add = 0;
		uint32_t adc_data[ADC_BUF_MAX];
		memcpy(adc_data,NTR.adc_buf,ADC_BUF_MAX * sizeof(uint32_t));  /* �����ݴӻ������������� */	
		/*
			adc����ƽ���˲�
			100������ÿ10������һ��ƽ���˲���
		
		*/
		for(uint8_t i = 0;i < ADC_BUF_MAX;i++)
		{
			
			if(i % 10 == 0 && i != 0)
			{
				Really_Average += Average_Add / 10;
				Average_Add = 0;
			}
			Average_Add += adc_data[i];
			
		}	
		Really_Average = Average_Add / 10;
		NTR.tempe_val = Really_Average  * 3.3 / 4069;
		/* ������¶� */
		HAL_ADC_Start_DMA(&NTR.NTR_ADC_Handler,NTR.adc_buf,ADC_BUF_MAX);
	
	}

}

/**
*******************************
* @brief : ��ȡ�¶�
* @parm  ��void
* @return ��float ---�¶ȵı���ֵ
*******************************
*/
float Get_Temp(void)
{
	return NTR.tempe_val;
}

