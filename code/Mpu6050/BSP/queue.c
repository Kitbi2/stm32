
#include "stm32f1xx_hal.h"
#include <stdio.h>

#include "queue.h"

void QueueInit(Queue_t * queue,uint32_t size,uint8_t * buf)
{
	queue->buf = buf;
	queue->size = size;
	queue->head = 0;
	queue->tail = 0;

}

QueueStat_t QueuePush(Queue_t * queue,uint8_t data)
{
	/* 判断当前的对立是否满了 */
	uint32_t index = (queue->tail + 1) % queue->size;
	if(index == queue->head)
	{
		return QUEUE_OVERLOAD;
	}
	queue->buf[queue->tail] = data;
	queue->tail = index;
	return QUEUE_OK;

}

QueueStat_t QueuePop(Queue_t * queue,uint8_t* data)
{
	/* 判断当前的队列是否空了 */
	uint32_t index = queue->head;
	if(index == queue->tail)
	{
		return QUEUE_EMTPY;
	}
	*data = queue->buf[queue->head];
	queue->head = (queue->head +1 )% queue->size;
	return QUEUE_OK;
}

uint32_t QueuePushArry(Queue_t * queue,uint8_t * pArry,uint32_t len)
{
	uint32_t i = 0;
	for( i = 0;i < len;i++)
	{
		if( QueuePush(queue,pArry[i]) == QUEUE_OVERLOAD)
		{
			break;
		}
	
	}
	return i;
}

uint32_t QueuePopArry(Queue_t * queue,uint8_t * pArry,uint32_t len)
{
	uint32_t i = 0;
	for( i = 0;i < len;i++)
	{
		if( QueuePop(queue,&pArry[i]) == QUEUE_EMTPY)
		{
			break;
		}
	
	}
	return i;
}

void QueueTest(void)
{
	uint8_t buf[100];
	Queue_t queue;
	
	QueueInit(&queue,20,buf);
	
	printf("the head %d tail %d\r\n",queue.head,queue.tail);
	for(uint8_t i = 0;i < 20;i++)
	{
		QueuePush(&queue,i);
		printf("the head %d tail %d\r\n",queue.head,queue.tail);
	}
	
	for(uint8_t i = 0;i < 10;i++)
	{
		uint8_t tmp = 0;
		QueuePop(&queue,&tmp);		
		printf("the data %d head %d tail %d\r\n",tmp,queue.head,queue.tail);
	}	
	
	for(uint8_t i = 0;i < 10;i++)
	{
		QueuePush(&queue,i);
		printf("the head %d tail %d\r\n",queue.head,queue.tail);
	}		
	
}

