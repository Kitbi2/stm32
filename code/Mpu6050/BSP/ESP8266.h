#ifndef ESP8266_H
#define ESP8266_H

void ESP8266_Init(void);

/**
*******************************
* @brief ： 通过usart3发送数据给esp8266
* @parm  ：buf---数据缓冲区
           len---数据长度
* @return ：void
*******************************
*/
void ESP8266_Write(char *buf, int len);

/**
*******************************
* @brief ： 获取eps8266的返回数据
* @parm  ：	len---数据长度
						
* @return ：char --- 返回数据的地址
*******************************
*/
char * EPS8266_READ(uint16_t * len);

void EPS8266_Clear(void);

#endif



