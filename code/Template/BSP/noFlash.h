#ifndef NOFLASH_H
#define NOFLASH_H

/**
*******************************
* @brief : ȷ��noFlash��ManufacturerID�Ƿ���ȷ
* @parm  ��void
* @return ��void
************************* ******
*/
void NOflash_Read_ManufacturerID(void);

/**
*******************************
* @brief : ȷ��noFlash��ManufacturerID�Ƿ���ȷ
* @parm  ��void
* @return ��void
************************* ******
*/
void NOflash_Read_JEDEID(void);
/**
*******************************
* @brief : ��ʼ��NOflash
* @parm  ��void
* @return ��void
************************* ******
*/
void NOFlash_Init(void);

/**
*******************************
* @brief : ��ȡ����
* @parm  ��buffer---���ݴ�ŵĵ�ַ
					 len---Ŀ�곤��
	         addr---Ŀ���ַ
* @return ��void
*******************************
*/
void NOFlash_Read_Data(uint8_t * buffer,uint8_t len,uint32_t addr);

void noFlash_Test(void);
#endif
