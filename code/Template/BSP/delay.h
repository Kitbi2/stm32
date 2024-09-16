#ifndef DELAY_H
#define DELAY_H
/**
*******************************
* @brief : 初始化基于DWT的delay
* @parm  ：void
* @return ：void
*******************************
*/
void Delay_Init(void);
/**
*******************************
* @brief : 微秒延时
* @parm  ：nus---要延时的秒数
* @return ：void
*******************************
*/
void DelayNus(uint32_t nUs);
/**
*******************************
* @brief : 毫秒延时
* @parm  ：nms---要延时的秒数
* @return ：void
*******************************
*/
void DelayNums(uint32_t nMs);

#endif

