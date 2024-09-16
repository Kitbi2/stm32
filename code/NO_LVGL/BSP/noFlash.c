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
#define NOFLASH_WRITEOK_CODE   0x01    /* ���λΪ1��ʱ���ʾ��æ */

#define NOFLASH_SETOR_CODE   0x20

/**
*******************************
* @brief : ʹ��NOFlash
* @parm  ��void
* @return ��void
*******************************
*/
static void NOFlash_Start(void)
{
	SPI1_CSS0_L();
}

/**
*******************************
* @brief : ʧ��NOFlash
* @parm  ��void
* @return ��void
*******************************
*/
static void NOFlash_Stop(void)
{
	SPI1_CSS0_H();
}

/**
*******************************
* @brief : ȷ��noFlash��ManufacturerID�Ƿ���ȷ
* @parm  ��void
* @return ��void
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
* @brief : ȷ��noFlash��ManufacturerID�Ƿ���ȷ
* @parm  ��void
* @return ��void
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
* @brief : ��ʼ��NOflash
* @parm  ��void
* @return ��void
************************* ******
*/
void NOFlash_Init(void)
{
	SPI1_Init();
	/* ����һ�οյ�д����� */
	NOFlash_Start();
	SPI1_WriteReadBtye(ManufacturerID_ADDR);
	NOFlash_Stop();
	DelayNums(100);
//	NOflash_Read_ManufacturerID();
//	NOflash_Read_JEDEID();


}


/**
*******************************
* @brief : ��ȡ����
* @parm  ��buffer---���ݴ�ŵĵ�ַ
					 len---Ŀ�곤��
	         addr---Ŀ���ַ
* @return ��void
*******************************
*/
void NOFlash_Read_Data(uint8_t * buffer,uint32_t len,uint32_t addr)
{
//	printf("read the len is %d the addr is %d\r\n",len,addr);

//		printf("the addr is %x\r\n",addr);
//	printf("the addr is %x\r\n",(addr & 0xff0000) >> 16);
//	printf("the addr is %x\r\n",(addr & 0xff00) >> 8);
//	printf("the addr is %x\r\n",(addr & 0xff));
	/* ʹ��NOFlash */
	NOFlash_Start();
	
	/* д���ȡ������ */
	SPI1_WriteReadBtye(NOFLASH_READDATA_CODE);
	
	/* д���ַ */
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
* @brief : ʹ��д����
* @parm  ��void
* @return ��void
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
* @brief : �ȵ�д�����
* @parm  ��void
* @return ��bool---д���Ƿ�ɹ�
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
* @brief : д������
* @parm  ��buffer---���ݵ�ַ
					 len---д�����ݳ���
	         addr---д��NOflash��ַ
* @return ��void
*******************************
*/
void NOFlash_Write_Data(uint8_t * buffer,uint32_t addr,uint32_t len)
{
//	printf("write the len is %d the addr is %d\r\n",len,addr);
	/* ʹ��noflash��д���� */
	NOFlash_WriteEnble();
	/* ʹ��noflash */
	NOFlash_Start();
	/* д��дҳ������ */
	SPI1_WriteReadBtye(NOFLASH_WRITEDATA_PAGE_CODE);
	
	/* д���ַ */
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
	/* ʧ��noflash */
	NOFlash_Stop();

	if(!NOFlash_WriteFinsh())
		printf("NOflash write error\r\n");
}



/**
*******************************
* @brief : ����ĳ������
* @parm  ��addr --- ���в���������
* @return ��void
*******************************
*/
void NOFlash_Sector(uint32_t addr)
{
/* ʹ��noflash��д���� */
	NOFlash_WriteEnble();
	/* ʹ��noflash */
	NOFlash_Start();
	/* д��дҳ������ */
	SPI1_WriteReadBtye(0x20);
	
	/* д���ַ */
	SPI1_WriteReadBtye((addr & 0xff0000) >> 16);
	SPI1_WriteReadBtye((addr & 0xff00) >> 8);
	SPI1_WriteReadBtye((addr & 0xff) );

	/* ʧ��noflash */
	NOFlash_Stop();

	if(!NOFlash_WriteFinsh())
		printf("NOflash write error\r\n");	
}

#define SPI_FLASH_PageSize 256

void SPI_FLASH_BufferWrite(uint8_t* pBuffer,uint32_t NumByteToWrite,uint32_t WriteAddr)
{
	
		printf("the len %d the addr %d\r\n",NumByteToWrite,WriteAddr);
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    /*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������
    ������AddrֵΪ0*/
    Addr = WriteAddr % SPI_FLASH_PageSize;

    /*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
    count = SPI_FLASH_PageSize - Addr;
    /*�����Ҫд��������ҳ*/
    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
    /*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    /* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
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
            /*�Ȱ�����ҳ��д��*/
            while (NumOfPage--)
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            NOFlash_Write_Data(pBuffer, WriteAddr,
                                NumOfSingle);
        }
    }
    /* ����ַ�� SPI_FLASH_PageSize ������  */
    else
    {
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            /*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
            if (NumOfSingle > count)
            {
                temp = NumOfSingle - count;
                /*��д����ǰҳ*/
                NOFlash_Write_Data(pBuffer, WriteAddr, count);

                WriteAddr +=  count;
                pBuffer += count;
                /*��дʣ�������*/
                NOFlash_Write_Data(pBuffer, WriteAddr, temp);
            }
            else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    NumByteToWrite);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            /*��ַ����������count�ֿ������������������*/
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            /* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
            NOFlash_Write_Data(pBuffer, WriteAddr, count);

            /* ���������ظ���ַ�������� */
            WriteAddr +=  count;
            pBuffer += count;
            /*������ҳ��д��*/
            while (NumOfPage--)
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            if (NumOfSingle != 0)
            {
                NOFlash_Write_Data(pBuffer, WriteAddr,
                                    NumOfSingle);
            }
        }
    }
}



