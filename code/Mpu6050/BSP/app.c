#include "ff.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include "noFlash.h"
#include "Systick.h"
#define FATFS_STR "0:"
#define FILE_PATH "0:tmp"
static FATFS g_fs;
static FIL g_file;
static bool g_FSstatus;

void FatfsInit(void)
{
	FRESULT res;
	uint8_t workbuff[FF_MAX_SS];
	res = f_mount(&g_fs,FATFS_STR,1);
	
	if(res == FR_NO_FILESYSTEM)
	{
		
		res = f_mkfs(FATFS_STR,0,workbuff,FF_MAX_SS);
						

		if(res == FR_OK)
		{
			printf("%d the err code\r\n",res);
			f_mount(NULL,FATFS_STR,1);
			res = f_mount(&g_fs,FATFS_STR,1);


		}
	}
	if(res != FR_OK)
	{
		printf("%d the err code\r\n",res);
		g_FSstatus = false;
		return;
	}
	g_FSstatus = true;
	
}

	uint8_t write_buf[4096] = {0};
	uint8_t read_buf[4096] = {0};
void FatTask(void)
{
//	printf("ok\r\n");

//	
//	for(uint16_t i = 0; i < 4096;i++)
//		write_buf[i] =0xcc;
//	
//	
//	NOFlash_Sector(0 << 12);


// SPI_FLASH_BufferWrite(write_buf,4096,0);


//	
//	NOFlash_Read_Data(read_buf,4096,0);
//	for(uint16_t i = 0; i < 4096;i++)
//			printf("the read_buf[%d] is %d\r\n",i,read_buf[i]);


	if(!g_FSstatus)
	{
		printf("file sys gg\r\n");
		return;
	}
	FRESULT res;
	res = f_open(&g_file,FILE_PATH,FA_OPEN_APPEND | FA_WRITE | FA_READ);
	if(res != FR_OK)
	{
		printf("f_open error\r\n");
		return;
	}
	
	char buf[] = "hello yjt";
	char read_buf[20] = {'\0'};
	printf("the ret %d\r\n",f_printf(&g_file,"%s",buf));
	f_close(&g_file);
	res = f_open(&g_file,FILE_PATH,FA_OPEN_EXISTING | FA_READ);
	
//	f_read(&g_file,read_buf,9,&bot);
	
	if(f_gets(read_buf,9,&g_file) != NULL)
	{
	
		printf("the read is %s\r\n",read_buf);

	}
	
	
	
}
