#ifndef  NOFLASH_H
#define  NOFLASH_H

/**
*******************************
* @brief : 确定noFlash的ManufacturerID是否正确
* @parm  ：void
* @return ：void
************************* ******
*/
void NOflash_Read_ManufacturerID(void);

/**
*******************************
* @brief : 确定noFlash的ManufacturerID是否正确
* @parm  ：void
* @return ：void
************************* ******
*/
void NOflash_Read_JEDEID(void);
/**
*******************************
* @brief : 初始化NOflash
* @parm  ：void
* @return ：void
************************* ******
*/
void NOFlash_Init(void);
/**
*******************************
* @brief : 写色私邼
* @parm  煤buffer---私邼謽址
					 len---写色私邼婴讏
	         addr---写色NOflash謽址
* @return 煤void
*******************************
*/
void NOFlash_Write_Data(uint8_t * buffer,uint32_t addr,uint32_t len);
/**
*******************************
* @brief : 读取数据
* @parm  ：buffer---数据存放的地址
					 len---目标长度
	         addr---目标地址
* @return ：void
*******************************
*/
void NOFlash_Read_Data(uint8_t * buffer,uint32_t len,uint32_t addr);
/**
*******************************
* @brief : 擦除某个扇区
* @parm  ：addr --- 进行擦除的扇区
* @return ：void
*******************************
*/
void NOFlash_Sector(uint32_t addr);

void SPI_FLASH_BufferWrite(uint8_t* pBuffer,uint32_t NumByteToWrite,uint32_t WriteAddr);
void noFlash_Test(void);
#endif
