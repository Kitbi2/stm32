#ifndef  NOFLASH_H
#define  NOFLASH_H

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
* @brief : дɫ˽ߝ
* @parm  úbuffer---˽ߝַ֘
					 len---дɫ˽ߝӤ׈
	         addr---дɫNOflashַ֘
* @return úvoid
*******************************
*/
void NOFlash_Write_Data(uint8_t * buffer,uint32_t addr,uint32_t len);
/**
*******************************
* @brief : ��ȡ����
* @parm  ��buffer---���ݴ�ŵĵ�ַ
					 len---Ŀ�곤��
	         addr---Ŀ���ַ
* @return ��void
*******************************
*/
void NOFlash_Read_Data(uint8_t * buffer,uint32_t len,uint32_t addr);
/**
*******************************
* @brief : ����ĳ������
* @parm  ��addr --- ���в���������
* @return ��void
*******************************
*/
void NOFlash_Sector(uint32_t addr);

void SPI_FLASH_BufferWrite(uint8_t* pBuffer,uint32_t NumByteToWrite,uint32_t WriteAddr);
void noFlash_Test(void);
#endif
