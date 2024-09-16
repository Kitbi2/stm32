/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "noFlash.h"
#include "Calendar.h"
#include <time.h>
#include "Systick.h"
#include "sdio_sdcard.h"
/* Definitions of physical drive number for each drive */
#define DEV_SD		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_NOFLASH		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
//	DSTATUS stat;
//	int result;

	switch (pdrv) {
	case DEV_SD :
			sd_init();  /* 初始化SD卡 */
				show_sdcard_info();
		// translate the reslut code here

		return RES_OK;

	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
//	DRESULT res;
//	int result;

	switch (pdrv) {
	case DEV_SD :
	
		
		sd_read_disk((uint8_t *)buff,sector,count );



		return RES_OK;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
//	DRESULT res;
//	int result;

	switch (pdrv) {
	case DEV_SD :

	sd_write_disk((uint8_t *)buff,sector,count);			

		return RES_OK;


	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
//	DRESULT res;
//	int result;

    DRESULT res;

    if (pdrv == DEV_SD)    /* SD卡 */
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;

            case 	GET_SECTOR_SIZE
							
						:
                *(DWORD *)buff = 512;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:
                *(WORD *)buff = g_sdcard_handler.SdCard.BlockSize;  /* g_sd_card_info.CardBlockSize;也可以 */
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)buff = ((long long)g_sdcard_handler.SdCard.BlockNbr * g_sdcard_handler.SdCard.BlockSize) / 512; /* g_sd_card_info.CardCapacity / 512;也可以 */
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
                break;
        }
    }

	return res;
}

DWORD get_fattime (void)
{
    time_t t;
    struct tm *stm;


    t = GetRTC_Count();
    stm = localtime(&t);

    return (DWORD)(stm->tm_year - 1980) << 25 |
           (DWORD)(stm->tm_mon + 1) << 21 |
           (DWORD)stm->tm_mday << 16 |
           (DWORD)stm->tm_hour << 11 |
           (DWORD)stm->tm_min << 5 |
           (DWORD)stm->tm_sec >> 1;
}



