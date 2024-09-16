#ifndef AT24C02_H
#define AT24C02_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>
void at24c02_test(void);

/**
*******************************
* @brief : 读取ACK信号
* @parm  ：addr---要进入读取的eeprom的地址
           pbuffer---数据存放的地址
					 num---读取的数据个数
* @return : bool --- 是否读取成功
*******************************
**/

bool EEPROM_Read(uint8_t addr,uint8_t * pbuffer,uint8_t num);

/**
*******************************
* @brief : 读取ACK信号
* @parm  ：addr---要进入写入的eeprom的地址
           pbuffer---数据存放的地址
					 num---写入的数据个数
* @return : bool --- 是否写入成功
*******************************
**/

bool EEPROM_Write(uint8_t addr,uint8_t * pbuffer,uint8_t num);

void at24c02_test(void);
#endif


