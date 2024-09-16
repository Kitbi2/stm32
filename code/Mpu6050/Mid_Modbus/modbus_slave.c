
#include "mb.h"
#include "mbutils.h"
#include "modbus_slave.h"
#include <stdio.h>

eMBErrorCode  eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress,
                                 USHORT usNRegs, eMBRegisterMode eMode )
{
	if(eMode == MB_REG_WRITE)
	{
			printf("the %d\r\n",usNRegs);
	}
	else if(eMode == MB_REG_READ)
	{
		printf("the %d\r\n",usNRegs);
	}
	

	
	return MB_ENOERR;
}

void Modbus_Init(void)
{
	eMBInit(MB_RTU,0x02,0,9600,MB_PAR_NONE);
	eMBEnable();	
}
