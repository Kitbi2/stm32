#ifndef NOFLASH_H
#define NOFLASH_H

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
* @brief : 读取数据
* @parm  ：buffer---数据存放的地址
					 len---目标长度
	         addr---目标地址
* @return ：void
*******************************
*/
void NOFlash_Read_Data(uint8_t * buffer,uint8_t len,uint32_t addr);

void noFlash_Test(void);
#endif
