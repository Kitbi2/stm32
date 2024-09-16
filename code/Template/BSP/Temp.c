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
	float tempe_val; /* 温度数据 */

}NTR_t;

NTR_t NTR;

/**
*******************************
* @brief : 初始化对应的gpio
* @parm  ：void
* @return ：void
*******************************
*/
static void NTR_GPIO_Init(void)
{
	
	/* 初始化时钟 */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	/* 配置gpio */
	NTR.NTR_GPIO.GPIO_BASE = GPIOC;
	NTR.NTR_GPIO.GPIO_struct.Mode = GPIO_MODE_AF_INPUT; //模拟输入
	NTR.NTR_GPIO.GPIO_struct.Pin = GPIO_PIN_10;         //GPIO10
	NTR.NTR_GPIO.GPIO_struct.Pull = GPIO_NOPULL;        //浮空
	NTR.NTR_GPIO.GPIO_struct.Speed = GPIO_SPEED_FREQ_HIGH; //高速

	HAL_GPIO_Init(NTR.NTR_GPIO.GPIO_BASE,&NTR.NTR_GPIO.GPIO_struct);
}

///**
//*******************************
//* @brief : 初始化ADC 软件触发
//* @parm  ：void
//* @return ：void
//*******************************
//*/
//static void ADC_Init_BySoftWork(void)
//{
//	/* 配置adc的时钟 */
//	RCC_PeriphCLKInitTypeDef Per = {0};
//	
//	/* 目标参数 */
//	Per.PeriphClockSelection = RCC_PERIPHCLK_ADC; // 目标是时钟
//	Per.AdcClockSelection = RCC_ADCPCLK2_DIV6; // adc的预分频
//	HAL_RCCEx_PeriphCLKConfig(&Per);
//	
//	
//	/* 配置ADC */
//	__HAL_RCC_ADC1_CLK_ENABLE();/* 使能ADC1的时钟 */
//	ADC_Handler.Instance = ADC1; 	/* adc基地址 */
//	ADC_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;/* 左对齐 */
//	ADC_Handler.Init.ScanConvMode = ADC_SCAN_DISABLE;/* 非扫描模式 */
//	ADC_Handler.Init.ContinuousConvMode =DISABLE;/* 单次转化 */
//	ADC_Handler.Init.NbrOfConversion = 1;/* 通道数目 */
//	/* 忽略间断模式 */
//	ADC_Handler.Init.DiscontinuousConvMode = DISABLE;
//	ADC_Handler.Init.NbrOfDiscConversion  = 0;
//	
//	ADC_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;/* 触发源外部触发  */

//	HAL_ADC_Init(&ADC_Handler);
//	
//	/* 配置ADC的通道 */
//	
//	ADC_ChannelConfTypeDef conf;
//	conf.Channel = 10;
//	conf.Rank = 1;
//	conf.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
//	HAL_ADC_ConfigChannel(&ADC_Handler,&conf);
//}

/**
*******************************
* @brief : 配置用于触发ADC的定时器3
* @parm  ：void
* @return ：void
*******************************
*/
static void NTR_TIM_Init(void)
{
	/* 使能时钟 */
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	/* 初始化定时器 */
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Instance = TIM3;       

	/* 定时器的频率 72000000 / 720 / 1000 = 100hz 也就是1ms中断100次*/
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.Period = 1000-1;   /* 从装载值 */
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.Prescaler = 720-1;   /* 预分配为72，也就是每个计数值为1us */
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	/* 72000khz / (50 * 72) = 20khz */
	/* 预分配系数 */
	
	NTR.NTR_TIME.NTR_ADC_Timer_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;   /* 计数模式 */
	HAL_TIM_Base_Init(&NTR.NTR_TIME.NTR_ADC_Timer_Handler);	
	
	
	NTR.NTR_TIME.Master.MasterOutputTrigger = TIM_TRGO_UPDATE;
	NTR.NTR_TIME.Master.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&NTR.NTR_TIME.NTR_ADC_Timer_Handler,&NTR.NTR_TIME.Master);
	
	HAL_TIM_Base_Start_IT(&NTR.NTR_TIME.NTR_ADC_Timer_Handler);
	
	
	
}


/**
*******************************
* @brief : 配置用于adc的dma
* @parm  ：void
* @return ：void
*******************************
*/
static void NTR_DMA_Init(void)
{
	/* 使能时钟 */
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* 选择初始化的通道 */
	NTR.NTR_ADC_DMA_Handler.Instance = DMA1_Channel1;
	/* 选择模式 外设to内存 */
	NTR.NTR_ADC_DMA_Handler.Init.Direction = DMA_PERIPH_TO_MEMORY;
	/* 外设非增量模式 */
	NTR.NTR_ADC_DMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;
	/* 存储器增量模式 */
	NTR.NTR_ADC_DMA_Handler.Init.MemInc = DMA_MINC_ENABLE;
	/* 外设位宽 */
	NTR.NTR_ADC_DMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	/* 内存位宽 */
	NTR.NTR_ADC_DMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	/* 最高优先级 */
	NTR.NTR_ADC_DMA_Handler.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	/* 模式正常 */
	NTR.NTR_ADC_DMA_Handler.Init.Mode = DMA_NORMAL;
	
	/* 初始化dma */
	HAL_DMA_Init(&NTR.NTR_ADC_DMA_Handler);	
	
	/* 进行adc句柄和dma句柄的绑定 */
	__HAL_LINKDMA(&NTR.NTR_ADC_Handler,DMA_Handle,NTR.NTR_ADC_DMA_Handler);
	/* 使能dma的全传输中断 */
	__HAL_DMA_ENABLE_IT(&NTR.NTR_ADC_DMA_Handler,DMA_IT_TC);
	/* 配置dma的中断组和优先级 */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn,1,1);	
	/* 使能dma中断 */
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
*******************************
* @brief : 初始化ADC定时器触发
* @parm  ：void
* @return ：void
*******************************
*/
static void NTR_ADC_Init(void)
{
	/* 配置adc的时钟 */
	RCC_PeriphCLKInitTypeDef Per = {0};
	
	/* 目标参数 */
	Per.PeriphClockSelection = RCC_PERIPHCLK_ADC; // 目标是时钟
	Per.AdcClockSelection = RCC_ADCPCLK2_DIV6; // adc的预分频
	HAL_RCCEx_PeriphCLKConfig(&Per);
	
	
	/* 配置ADC */
	__HAL_RCC_ADC1_CLK_ENABLE();/* 使能ADC1的时钟 */
	NTR.NTR_ADC_Handler.Instance = ADC1; 	/* adc基地址 */
	NTR.NTR_ADC_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;/* 右对齐 */
	NTR.NTR_ADC_Handler.Init.ScanConvMode = ADC_SCAN_DISABLE;/* 非扫描模式 */
	NTR.NTR_ADC_Handler.Init.ContinuousConvMode = DISABLE;/* 多次转化 */
	NTR.NTR_ADC_Handler.Init.NbrOfConversion = 1;/* 通道数目 */
	/* 忽略间断模式 */
	NTR.NTR_ADC_Handler.Init.DiscontinuousConvMode = DISABLE;
	NTR.NTR_ADC_Handler.Init.NbrOfDiscConversion  = 0;
	
	NTR.NTR_ADC_Handler.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;/* 定时器触发  */
	
	HAL_ADC_Init(&NTR.NTR_ADC_Handler);
	ADC1->CR2 |= (1 << 20);
	/* 配置ADC的通道 */	
	ADC_ChannelConfTypeDef conf;
	conf.Channel = 10;      /* 通道10工作 */
	conf.Rank = 1;          /* 序列一 */
	conf.SamplingTime = ADC_SAMPLETIME_7CYCLES_5; /* 采样周期 */
	HAL_ADC_ConfigChannel(&NTR.NTR_ADC_Handler,&conf);	
	
	
	/* 配置dma的中断组和优先级 */
	HAL_NVIC_SetPriority(ADC1_2_IRQn,1,1);	
	/* 使能dma中断 */
	HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
	
	
  HAL_ADC_Start_DMA(&NTR.NTR_ADC_Handler,NTR.adc_buf,ADC_BUF_MAX);
	

}
/**
*******************************
* @brief : adc外设
* @parm  ：void
* @return ：void
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
* @brief : dma1中断服务函数
* @parm  ：void

* @return ：void
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
* @brief : 获取adc的一次采样
* @parm  ：void
* @return ：uint16_t ---电压的编码值
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
* @brief : 触发NTR进行转化
* @parm  ：void
* @return ：uint16_t ---电压的编码值
*******************************
*/
void NTR_Driver(void)
{
	
	if(NTR.adc_flag == 1)
	{
		uint32_t Really_Average = 0;
		uint32_t Average_Add = 0;
		uint32_t adc_data[ADC_BUF_MAX];
		memcpy(adc_data,NTR.adc_buf,ADC_BUF_MAX * sizeof(uint32_t));  /* 把数据从缓冲区拷贝走人 */	
		/*
			adc进行平均滤波
			100个数据每10个进行一次平均滤波，
		
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
		/* 换算成温度 */
		HAL_ADC_Start_DMA(&NTR.NTR_ADC_Handler,NTR.adc_buf,ADC_BUF_MAX);
	
	}

}

/**
*******************************
* @brief : 获取温度
* @parm  ：void
* @return ：float ---温度的编码值
*******************************
*/
float Get_Temp(void)
{
	return NTR.tempe_val;
}

