#ifndef XPT2046_H
#define XPT2046_H
#include <stdbool.h>

/**
*******************************
* @brief : ��ʼ��XPT2046
* @parm  ��void
* @return ��void
*******************************
*/
void XPT2046_Init(void);


/**
*******************************
* @brief : ��ȡx����
* @parm  ��void
* @return ��void
************************* ******
*/
bool XPT2046_Read_XY(uint16_t * x,uint16_t * y);


#endif


