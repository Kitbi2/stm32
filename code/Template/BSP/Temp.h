#ifndef ADC_H
#define ADC_H

/**
*******************************
* @brief : 获取温度
* @parm  ：void
* @return ：float ---温度的编码值
*******************************
*/
float Get_Temp(void);
/**
*******************************
* @brief : NTR
* @parm  ：void
* @return ：void
*******************************
*/
void NTR_Init(void);
/**
*******************************
* @brief : 触发NTR进行转化
* @parm  ：void
* @return ：uint16_t ---电压的编码值
*******************************
*/
void NTR_Driver(void);

#endif



