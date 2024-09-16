#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "noFlash.h"
#include "delay.h"
#include <stdbool.h>
#include "SPI.h"
#define ManufacturerID_ADDR 0xAB



#define NOFLASH_READDATA_CODE 0x03

#define NOFLASH_WRITEDATA_ENABLE_CODE 0x06



#define NOFLASH_WRITEDATA_PAGE_CODE 0x02

#define NOFLASH_WRITESTATUSREG_ADDR 0x05
#define NOFLASH_WRITEOK_CODE   0x01    /* 最低位为1的时候表示在忙 */

#define NOFLASH_SETOR_CODE   0x20

/**
*******************************
* @brief : 使能NOFlash
* @parm  ：void
* @return ：void
*******************************
*/
static void NOFlash_Start(void)
{
	SPI1_CSS0_L();
}

/**
*******************************
* @brief : 失能NOFlash
* @parm  ：void
* @return ：void
*******************************
*/
static void NOFlash_Stop(void)
{
	SPI1_CSS0_H();
}

/**
*******************************
* @brief : 确定noFlash的ManufacturerID是否正确
* @parm  ：void
* @return ：void
************************* ******
*/
void NOflash_Read_ManufacturerID(void)
{
	uint8_t ManufacturerID = 0;
	NOFlash_Start();
	SPI1_WriteReadBtye(ManufacturerID_ADDR);
	SPI1_WriteReadBtye(0xff);
	SPI1_WriteReadBtye(0xff);
	SPI1_WriteReadBtye(0xff);
	ManufacturerID = SPI1_WriteReadBtye(0xff);
	printf("the ManufacturerID is %x\r\n",ManufacturerID);
	NOFlash_Stop();

}
/**
*******************************
* @brief : 确定noFlash的ManufacturerID是否正确
* @parm  ：void
* @return ：void
************************* ******
*/
void NOflash_Read_JEDEID(void)
{
	uint8_t buf[3] = {0};
	NOFlash_Start();
	SPI1_WriteReadBtye(0x9f);
	buf[0] = SPI1_WriteReadBtye(0xff);
	buf[1] = SPI1_WriteReadBtye(0xff);
	buf[2] = SPI1_WriteReadBtye(0xff);
	
	printf("the ManufacturerID is %x\r\n",(buf[0] << 16) | (buf[1] << 8 )| (buf[2]));
	NOFlash_Stop();
	
}
/**
*******************************
* @brief : 初始化NOflash
* @parm  ：void
* @return ：void
************************* ******
*/
void NOFlash_Init(void)
{
	SPI1_Init();
	/* 进行一次空的写入操作 */
	NOFlash_Start();
	SPI1_WriteReadBtye(ManufacturerID_ADDR);
	NOFlash_Stop();
	DelayNums(100);
//	NOflash_Read_ManufacturerID();
//	NOflash_Read_JEDEID();


}


/**
*******************************
* @brief : 读取数据
* @parm  ：buffer---数据存放的地址
					 len---目标长度
	         addr---目标地址
* @return ：void
*******************************
*/
void NOFlash_Read_Data(uint8_t * buffer,uint32_t len,uint32_t addr)
{
//	printf("read the len is %d the addr is %d\r\n",len,addr);

//		printf("the addr is %x\r\n",addr);
//	printf("the addr is %x\r\n",(addr & 0xff0000) >> 16);
//	printf("the addr is %x\r\n",(addr & 0xff00) >> 8);
//	printf("the addr is %x\r\n",(addr & 0xff));
	/* 使能NOFlash */
	NOFlash_Start();
	
	/* 写入读取操作码 */
	SPI1_WriteReadBtye(NOFLASH_READDATA_CODE);
	
	/* 写入地址 */
	SPI1_WriteReadBtye((addr & 0xff0000) >> 16);
	SPI1_WriteReadBtye((addr & 0xff00) >> 8);
	SPI1_WriteReadBtye((addr & 0xff) );
	
	while(len--)
	{
		*buffer = SPI1_WriteReadBtye(0xff);
		buffer++;
	}
}
/**
*******************************
* @brief : 使能写操作
* @parm  ：void
* @return ：void
*******************************
*/
static void NOFlash_WriteEnble(void)
{
	NOFlash_Start();
	
	SPI1_WriteReadBtye(NOFLASH_WRITEDATA_ENABLE_CODE);
	
	NOFlash_Stop();
	
}
/**
*******************************
* @brief : 等等写入完成
* @parm  ：void
* @return ：bool---写入是否成功
*******************************
*/
static bool NOFlash_WriteFinsh(void)
{

	uint8_t ret = 0;
	uint32_t time = 0;
	NOFlash_Start();

	SPI1_WriteReadBtye(NOFLASH_WRITESTATUSREG_ADDR);
	do
	{
		ret = SPI1_WriteReadBtye(0xff);
	}
	while(ret & 0x01);
	
	
	if(time >=  65535)
	{
		NOFlash_Stop();
		return false;
	}
	NOFlash_Stop();
	return true;
	
}
/**
*******************************
* @brief : 写入数据
* @parm  ：buffer---数据地址
					 len---写入数据长度
	         addr---写入NOflash地址
* @return ：void
*******************************
*/
void NOFlash_Write_Data(uint8_t * buffer,uint32_t addr,uint32_t len)
{
//	printf("write the len is %d the addr is %d\r\n",len,addr);
	/* 使能noflash的写操作 */
	NOFlash_WriteEnble();
	/* 使能noflash */
	NOFlash_Start();
	/* 写入写页操作码 */
	SPI1_WriteReadBtye(NOFLASH_WRITEDATA_PAGE_CODE);
	
	/* 写入地址 */
//	printf("the addr is %x\r\n",addr);
//	printf("the addr is %x\r\n",(addr & 0xff0000) >> 16);
//	printf("the addr is %x\r\n",(addr & 0xff00) >> 8);
//	printf("the addr is %x\r\n",(addr & 0xff));
	
	SPI1_WriteReadBtye((addr & 0xff0000) >> 16);
	SPI1_WriteReadBtye((addr & 0xff00) >> 8);
	SPI1_WriteReadBtye((addr & 0xff) );
	
	while(len--)
	{
		SPI1_WriteReadBtye(*buffer);
		buffer++;
	}
	/* 失能noflash */
	NOFlash_Stop();

	if(!NOFlash_WriteFinsh())
		printf("NOflash write error\r\n");
}



/**
*******************************
* @brief : 擦除某个扇区
* @parm  ：addr --- 进行擦除的扇区
* @return ：void
*******************************
*/
void NOFlash_Sector(uint32_t addr)
{
/* 使能noflash的写操作 */
	NOFlash_WriteEnble();
	/* 使能noflash */
	NOFlash_Start();
	/* 写入写页操作码 */
	SPI1_WriteReadBtye(0x20);
	
	/* 写入地址 */
	SPI1_WriteReadBtye((addr & 0xff0000) >> 16);
	SPI1_WriteReadBtye((addr & 0xff00) >> 8);
	SPI1_WriteReadBtye((addr & 0xff) );

	/* 失能noflash */
	NOFlash_Stop();

	if(!NOFlash_WriteFinsh())
		printf("NOflash write error\r\n");	
}

#define SPI_FLASH_PageSize 256

void SPI_FLASH_BufferWrite(uint8_t* pBuffer,uint32_t NumByteToWrite,uint32_t WriteAddr)
{
	
		printf("the len %d the addr %d\r\n",NumByteToWrite,WriteAddr);
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，
    运算结果Addr值为0*/
    Addr = WriteAddr % SPI_FLASH_PageSize;

    /*差count个数据值，刚好可以对齐到页地址*/
    count = SPI_FLASH_PageSize - Addr;
    /*计算出要写多少整数页*/
    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
    /*mod运算求余，计算出剩余不满一页的字节数*/
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
    if (Addr == 0)
    {
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            NOFlash_Write_Data(pBuffer, WriteAddr,
                                NumByteToWrite);
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            /*先把整数页都写了*/
            while (NumOfPage--)
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            NOFlash_Write_Data(pBuffer, WriteAddr,
                                NumOfSingle);
        }
    }
    /* 若地址与 SPI_FLASH_PageSize 不对齐  */
    else
    {
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            /*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
            if (NumOfSingle > count)
            {
                temp = NumOfSingle - count;
                /*先写满当前页*/
                NOFlash_Write_Data(pBuffer, WriteAddr, count);

                WriteAddr +=  count;
                pBuffer += count;
                /*再写剩余的数据*/
                NOFlash_Write_Data(pBuffer, WriteAddr, temp);
            }
            else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    NumByteToWrite);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            /*地址不对齐多出的count分开处理，不加入这个运算*/
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            /* 先写完count个数据，为的是让下一次要写的地址对齐 */
            NOFlash_Write_Data(pBuffer, WriteAddr, count);

            /* 接下来就重复地址对齐的情况 */
            WriteAddr +=  count;
            pBuffer += count;
            /*把整数页都写了*/
            while (NumOfPage--)
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            if (NumOfSingle != 0)
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    NumOfSingle);
            }
        }
    }
}



