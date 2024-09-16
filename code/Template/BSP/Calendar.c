#include  "stm32f1xx_hal.h"
#include "stdio.h"
#include "Calendar.h"
#include <time.h>


RTC_HandleTypeDef Rtc_handler; 
uint8_t rtc_task = 0;



/**
*******************************
* @brief : ��ȡRTC��CNT
* @parm  ��void
* @return��uint32_t ---RTC��CNT��ֵ
*******************************
*/
static uint32_t GetRTC_Count(void)
{
	
		uint32_t ret = 0;
		RTC->CRL |= 1 << 4;             /* ��������ģʽ */
	
		/* ��ȡ���� */
		ret = RTC->CNTH;
		ret <<= 16;
		ret |= RTC->CNTL;         
		RTC->CRL &= ~(1 << 4);          /* �˳�����ģʽ */

		while (!(RTC->CRL & (1 << 5))); /* �ȴ�������� */

		return ret;
}
/**
*******************************
* @brief : д��RTC��CNT
* @parm  ��val---Ŀ����ֵ
* @return ��void
*******************************
*/
static void SetRTC_Count(uint32_t val)
{
		RTC->CRL |= 1 << 4;             /* ��������ģʽ */
	
		/* д���������� */
		RTC->CNTH = (val >> 16);
		RTC->CNTL = (val & 0xffff);    
	
		RTC->CRL &= ~(1 << 4);          /* �˳�����ģʽ */

		while (!(RTC->CRL & (1 << 5))); /* �ȴ�������� */
}

/**
*******************************
* @brief :��ʼ��LSE
* @parm  ��void
* @return��void 
*******************************
*/
static void LSE_Init(void)
{
	
	/* ʹ��BWK��PKR��ʱ�� */
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	
	/* ʹ��LSE */
	RCC_OscInitTypeDef LSEInit;
	LSEInit.OscillatorType = RCC_OSCILLATORTYPE_LSE;/* Ŀ�����õ�Ŀ��ʱ��ԴΪLSE */
	LSEInit.LSEState = RCC_LSE_ON; /* ����LSE */
	LSEInit.PLL.PLLState = RCC_PLL_NONE; /* ����pll */
	

	HAL_RCC_OscConfig(&LSEInit); /* ����LSE */	
}
/**
*******************************
* @brief :��ʼ��RTC
* @parm  ��void
* @return��void 
*******************************
*/
static void RTC_Init(void)
{
	/* ʹ��BWK��PKR��ʱ�� */
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	/* ����LSE��ΪRTC��ʱ�� */
	RCC_PeriphCLKInitTypeDef RTCSock;
	RTCSock.PeriphClockSelection = RCC_PERIPHCLK_RTC; /* Ҫ����ѡ��ʱ��Դ������ */
	RTCSock.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; /* ����RTC��ʱ��ԴΪLSE */

	
	/* ����RTC */

	Rtc_handler.Instance = RTC;    
	Rtc_handler.Init.AsynchPrediv = 32767;
	/* �ж��Ƿ�Ϊ��һ������ */
	uint32_t ret = HAL_RTCEx_BKUPRead(&Rtc_handler,RTC_BKP_DR1);/* ��ȡ�������� */
	
	
	if(ret != 0x5050) /* ���ret!=0x5050���Ƿǵ�һ�� */
	{
		
		/* ʹ��LSE */
		LSE_Init();
		
		__HAL_RCC_RTC_ENABLE(); /* ʹ��RTCʱ�� */
		HAL_RCCEx_PeriphCLKConfig(&RTCSock); /* ����RTC��ʱ��Դ */	
		HAL_RTC_Init(&Rtc_handler);	         /* ����RTC */
		
		SetRTC_Count(1709730113);
		
		/* �Ա���д��0x5052 */
		HAL_RTCEx_BKUPWrite(&Rtc_handler,RTC_BKP_DR1,0x5050);
		
	
	}		
	/* ����Ԥ��Ƶ�ж� */	 
	__HAL_RTC_ALARM_ENABLE_IT(&Rtc_handler,RTC_IT_SEC);

	/* ����RTC�ж����ȼ�����,ʹ���ж� */
	HAL_NVIC_SetPriority(RTC_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(RTC_IRQn);	
	
}
/**
*******************************
* @brief :��ʼ������
* @parm  ��void
* @return��void 
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
* @brief : ��ȡʱ��
* @parm  ��void
* @return��void 
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




