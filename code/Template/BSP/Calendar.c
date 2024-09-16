#include  "stm32f1xx_hal.h"
#include "stdio.h"
#include "Calendar.h"
#include <time.h>


RTC_HandleTypeDef Rtc_handler; 
uint8_t rtc_task = 0;



/**
*******************************
* @brief : 获取RTC的CNT
* @parm  ：void
* @return：uint32_t ---RTC的CNT数值
*******************************
*/
static uint32_t GetRTC_Count(void)
{
	
		uint32_t ret = 0;
		RTC->CRL |= 1 << 4;             /* 进入配置模式 */
	
		/* 读取数据 */
		ret = RTC->CNTH;
		ret <<= 16;
		ret |= RTC->CNTL;         
		RTC->CRL &= ~(1 << 4);          /* 退出配置模式 */

		while (!(RTC->CRL & (1 << 5))); /* 等待操作完成 */

		return ret;
}
/**
*******************************
* @brief : 写入RTC的CNT
* @parm  ：val---目标数值
* @return ：void
*******************************
*/
static void SetRTC_Count(uint32_t val)
{
		RTC->CRL |= 1 << 4;             /* 进入配置模式 */
	
		/* 写入数据数据 */
		RTC->CNTH = (val >> 16);
		RTC->CNTL = (val & 0xffff);    
	
		RTC->CRL &= ~(1 << 4);          /* 退出配置模式 */

		while (!(RTC->CRL & (1 << 5))); /* 等待操作完成 */
}

/**
*******************************
* @brief :初始化LSE
* @parm  ：void
* @return：void 
*******************************
*/
static void LSE_Init(void)
{
	
	/* 使能BWK和PKR的时钟 */
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	
	/* 使能LSE */
	RCC_OscInitTypeDef LSEInit;
	LSEInit.OscillatorType = RCC_OSCILLATORTYPE_LSE;/* 目标配置的目标时钟源为LSE */
	LSEInit.LSEState = RCC_LSE_ON; /* 开启LSE */
	LSEInit.PLL.PLLState = RCC_PLL_NONE; /* 忽略pll */
	

	HAL_RCC_OscConfig(&LSEInit); /* 配置LSE */	
}
/**
*******************************
* @brief :初始化RTC
* @parm  ：void
* @return：void 
*******************************
*/
static void RTC_Init(void)
{
	/* 使能BWK和PKR的时钟 */
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	/* 配置LSE作为RTC的时钟 */
	RCC_PeriphCLKInitTypeDef RTCSock;
	RTCSock.PeriphClockSelection = RCC_PERIPHCLK_RTC; /* 要进行选择时钟源的外设 */
	RTCSock.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; /* 配置RTC的时钟源为LSE */

	
	/* 配置RTC */

	Rtc_handler.Instance = RTC;    
	Rtc_handler.Init.AsynchPrediv = 32767;
	/* 判断是否为第一次配置 */
	uint32_t ret = HAL_RTCEx_BKUPRead(&Rtc_handler,RTC_BKP_DR1);/* 读取备份数据 */
	
	
	if(ret != 0x5050) /* 如果ret!=0x5050就是非第一次 */
	{
		
		/* 使能LSE */
		LSE_Init();
		
		__HAL_RCC_RTC_ENABLE(); /* 使能RTC时钟 */
		HAL_RCCEx_PeriphCLKConfig(&RTCSock); /* 配置RTC的时钟源 */	
		HAL_RTC_Init(&Rtc_handler);	         /* 配置RTC */
		
		SetRTC_Count(1709730113);
		
		/* 对备份写入0x5052 */
		HAL_RTCEx_BKUPWrite(&Rtc_handler,RTC_BKP_DR1,0x5050);
		
	
	}		
	/* 开启预分频中断 */	 
	__HAL_RTC_ALARM_ENABLE_IT(&Rtc_handler,RTC_IT_SEC);

	/* 设置RTC中断优先级和组,使能中断 */
	HAL_NVIC_SetPriority(RTC_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(RTC_IRQn);	
	
}
/**
*******************************
* @brief :初始化日历
* @parm  ：void
* @return：void 
*******************************
*/
void Calendar_Init(void)
{
	RTC_Init();
}

void RTC_IRQHandler(void)
{
	if (__HAL_RTC_ALARM_GET_FLAG(&Rtc_handler,RTC_FLAG_SEC) != RESET)
	{
		
		__HAL_RTC_ALARM_CLEAR_FLAG(&Rtc_handler, RTC_FLAG_SEC); 

		rtc_task = 1;
	}
}
/**
*******************************
* @brief : 获取时间
* @parm  ：void
* @return：void 
*******************************
*/
void Get_Date(Date_t * buf)
{
	time_t timestamp = GetRTC_Count();
	struct tm *date = localtime(&timestamp);
	buf->year = date->tm_year+1900;
	buf->mouth = date->tm_mon + 1;
	buf->day = date->tm_mday;
	buf->hour = date->tm_hour + 8;
	buf->min = date->tm_min;
	buf->sec = date->tm_sec;
	
}




