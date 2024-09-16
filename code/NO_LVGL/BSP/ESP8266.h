#ifndef ESP8266_H
#define ESP8266_H

void ESP8266_Init(void);

/**
*******************************
* @brief �� ͨ��usart3�������ݸ�esp8266
* @parm  ��buf---���ݻ�����
           len---���ݳ���
* @return ��void
*******************************
*/
void ESP8266_Write(char *buf, int len);

/**
*******************************
* @brief �� ��ȡeps8266�ķ�������
* @parm  ��	len---���ݳ���
						
* @return ��char --- �������ݵĵ�ַ
*******************************
*/
char * EPS8266_READ(uint16_t * len);

void EPS8266_Clear(void);

#endif



