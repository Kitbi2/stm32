#ifndef AT24C02_H
#define AT24C02_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>
void at24c02_test(void);

/**
*******************************
* @brief : ��ȡACK�ź�
* @parm  ��addr---Ҫ�����ȡ��eeprom�ĵ�ַ
           pbuffer---���ݴ�ŵĵ�ַ
					 num---��ȡ�����ݸ���
* @return : bool --- �Ƿ��ȡ�ɹ�
*******************************
**/

bool EEPROM_Read(uint8_t addr,uint8_t * pbuffer,uint8_t num);

/**
*******************************
* @brief : ��ȡACK�ź�
* @parm  ��addr---Ҫ����д���eeprom�ĵ�ַ
           pbuffer---���ݴ�ŵĵ�ַ
					 num---д������ݸ���
* @return : bool --- �Ƿ�д��ɹ�
*******************************
**/

bool EEPROM_Write(uint8_t addr,uint8_t * pbuffer,uint8_t num);

void at24c02_test(void);
#endif


