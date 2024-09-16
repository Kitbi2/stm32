#ifndef QUEUE_H
#define QUEUE_H
typedef struct
{
	uint32_t size;
	uint8_t * buf;
	uint32_t head;
	uint32_t tail;

}Queue_t;
typedef enum
{
	QUEUE_OK = 0,
	QUEUE_ERRO,
	QUEUE_EMTPY,
	QUEUE_OVERLOAD
}QueueStat_t;

void QueueInit(Queue_t * queue,uint32_t size,uint8_t * buf);
QueueStat_t QueuePush(Queue_t * queue,uint8_t data);
QueueStat_t QueuePop(Queue_t * queue,uint8_t* data);
uint32_t QueuePushArry(Queue_t * queue,uint8_t * pArry,uint32_t len);
uint32_t QueuePopArry(Queue_t * queue,uint8_t * pArry,uint32_t len);


void QueueTest(void);

#endif


