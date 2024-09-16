#include "stm32f1xx_hal.h"
#include "led_driver.h"
#include "queue.h"
#include "stdio.h"
#include "USBtoComApp.h"
#define Data_FREMES_HEAD0_TARG   0  /* ֡ͷ0 */
#define Data_FREMES_HEAD1_TARG   1  /* ֡ͷ1 */
#define Data_FREMES_LEN_TARG     2  /* ���ݳ��� */
#define Data_FREMES_CODE_TARG    3  /* ������ */


#define Data_FREMES_HEAD0    0x11
#define Data_FREMES_HEAD1    0x55


#define Data_FREMES_LEDCtlr  0x01   
#define Data_FREMES_LED_STATUS_TARG    4  /* ״̬*/
#define Data_FREMES_LED_NUM_TARG    5  /* ���*/
#define QUEUE_BUF_MAX 77
#define Data_Frames_Len_MAX 3
Queue_t queue;
uint8_t Queue_Buf[QUEUE_BUF_MAX ];

uint8_t flag = 0;
void USBtoComApp(uint8_t *data,uint32_t len)
{

	QueuePushArry(&queue,data,len);
	
}

void USBtoComTask(void)
{
			
			uint8_t c = '\0';
	if(QueuePop(&queue,&c) == QUEUE_OK)
	{
		printf("the %c\r\n",c);
	}
			
//		uint8_t data_frames[Data_Frames_Len_MAX + 3];
//		while(QueuePop(&queue,&data_frames[Data_FREMES_HEAD0_TARG]) == QUEUE_OK)
//		{
//			
//			/* �жϰ�ͷ1 */
//			if(data_frames[Data_FREMES_HEAD0_TARG] != Data_FREMES_HEAD0)
//			{
//				continue;
//			}
//			if(QueuePop(&queue,&data_frames[Data_FREMES_HEAD1_TARG]) != QUEUE_OK)
//			{
//				continue;
//			}
//			
//			/* �жϰ�ͷ2 */
//			if(data_frames[Data_FREMES_HEAD1_TARG] != Data_FREMES_HEAD1)
//			{
//				continue;
//			}
//			if(QueuePop(&queue,&data_frames[Data_FREMES_LEN_TARG]) != QUEUE_OK)
//			{
//				continue;
//			}		
//			
//			/* �ж϶�ȡ���ݵĳ��� */
//			if(data_frames[Data_FREMES_LEN_TARG] > Data_Frames_Len_MAX )
//			{
//				continue;
//			}
//			
//			if(QueuePopArry(&queue,data_frames+3,data_frames[Data_FREMES_LEN_TARG]) != data_frames[Data_FREMES_LEN_TARG])
//			{
//				continue;
//			}
//			
//			if( data_frames[Data_FREMES_CODE_TARG] == Data_FREMES_LEDCtlr)
//			{
//				data_frames[Data_FREMES_LED_STATUS_TARG] == 0? TurnOffLed(data_frames[Data_FREMES_LED_NUM_TARG]) :
//										TurnOnLed(data_frames[Data_FREMES_LED_NUM_TARG]);
//			}
//			

//			break;
//			
//		}

}
void USBtoComAppInit(void)
{
	QueueInit(&queue,QUEUE_BUF_MAX,Queue_Buf);
}


