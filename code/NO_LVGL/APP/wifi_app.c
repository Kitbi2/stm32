#include "stm32f1xx_hal.h"

#include "ESP8266.h"
#include "Systick.h"


#include <stdio.h>
#include <string.h>


typedef enum
{
	WIFI_COM_WAIT,
	WIFI_COM_OK,
	WIFI_COM_FAIL,
	
}WifiComState_t;

 WifiComState_t AtCmdHandle(char * cmd,char * rsp,uint32_t timeout)
{
	static WifiComState_t s_commState = WIFI_COM_OK;
	static uint64_t sendCmdTime;	
	char * rev = NULL;
	if(s_commState == WIFI_COM_OK)
	{
		printf("the cmd %s \r\n",cmd);
		ESP8266_Write(cmd,strlen(cmd));
		s_commState = WIFI_COM_WAIT;
		sendCmdTime = Get_Work_Time();
		
	}
	else if(s_commState == WIFI_COM_WAIT)
	{
		if(Get_Work_Time() - sendCmdTime < timeout)
		{
			uint16_t len;
			rev = EPS8266_READ(&len);
			
			printf("the rev %s\r\n",rev);
			if (strstr(rev, rsp) != NULL)
			{
				s_commState = WIFI_COM_OK;
				EPS8266_Clear();
			}
			
		}
		else
		{
			s_commState = WIFI_COM_FAIL;

		}
	}
	
	return s_commState;

}

typedef struct {
    /* Ҫ���͵�AT���� */
    char *cmd;
    /* ������Ӧ�����ݣ�Ĭ�ϴ���ƥ�䵽���ַ�����Ϊ����ִ�гɹ� */
    char *rsp;
    /* �õ�Ӧ��ĳ�ʱʱ�䣬�ﵽ��ʱʱ��Ϊִ��ʧ�ܣ���λms*/
    uint32_t timeoutMs;
} AtCmdInfo_t;

static AtCmdInfo_t g_checkModuleCmdTable[] = {
	
	   {
        .cmd = "ATE0\r\n",        // �رջ���
        .rsp = "OK",
		.timeoutMs = 1000,
    },
    
		
		{
        .cmd = "AT\r\n",        // AT
        .rsp = "OK",
		.timeoutMs = 1000,
    },
	{
        .cmd = "AT+CWMODE=3\r\n",  //wifiģʽ
        .rsp = "OK",
		.timeoutMs = 1000,	
	},
	{
        .cmd = "AT+CWJAP=\"YE8681253\",\"YJZ26333\"\r\n", //����·����
        .rsp = "OK",
		.timeoutMs = 15000,	
	},
	{
		.cmd = "AT+CIPSTART=\"TCP\",\"183.31.27.154\",1883\r\n", //���ӷ�����
		.rsp = "OK",
		.timeoutMs = 15000,	
	
	},
	{
		.cmd = "AT+CIPMODE=1\r\n",            //͸��ģʽ
	 	.rsp = "OK",
		.timeoutMs = 1000,	
	
	},	
	{
		.cmd = "AT+CIPSEND\r\n",            //��������
	 	.rsp = ">",
		.timeoutMs = 1000,	
	
	},	
	
};
typedef enum 
{
  ATE0 = 0,
	AT,
	AT_CWMODE_3,
	AT_CWJAP,
	AT_CIPSTART,
	AT_CIPMODE,
	AT_CIPSEND,
} AtCheckModuleCmdType;

WifiComState_t CheckWifiModuleWork(void)
{
	WifiComState_t commState;
	static uint8_t retryCount = 0;
	static AtCheckModuleCmdType cmdType = ATE0;	
	
	
	switch (cmdType)
	{
	
		case ATE0:
			
			commState = AtCmdHandle(g_checkModuleCmdTable[cmdType].cmd,
															g_checkModuleCmdTable[cmdType].rsp,
															g_checkModuleCmdTable[cmdType].timeoutMs);
			if(commState == WIFI_COM_OK)
			{
				retryCount = 0;
				cmdType = AT;
			}
			else if(commState == WIFI_COM_FAIL)
			{
				retryCount++;
				if(retryCount == 3)
				{
					retryCount = 0;
					return WIFI_COM_FAIL;
				}
			}
			break;
			
		case AT:
			
			commState = AtCmdHandle(g_checkModuleCmdTable[cmdType].cmd,
															g_checkModuleCmdTable[cmdType].rsp,
															g_checkModuleCmdTable[cmdType].timeoutMs);
			if(commState == WIFI_COM_OK)
			{
				retryCount = 0;
				cmdType = AT_CWMODE_3;
			}
			else if(commState == WIFI_COM_FAIL)
			{
				retryCount++;
				if(retryCount == 3)
				{
					retryCount = 0;
					return WIFI_COM_FAIL;
				}
			}
			break;	

		case AT_CWMODE_3:
			
			commState = AtCmdHandle(g_checkModuleCmdTable[cmdType].cmd,
															g_checkModuleCmdTable[cmdType].rsp,
															g_checkModuleCmdTable[cmdType].timeoutMs);
			if(commState == WIFI_COM_OK)
			{
				retryCount = 0;
				cmdType = AT_CWJAP;
			}
			else if(commState == WIFI_COM_FAIL)
			{
				retryCount++;
				if(retryCount == 3)
				{
					retryCount = 0;
					return WIFI_COM_FAIL;
				}
			}
			break;		

		case AT_CWJAP:
			
			commState = AtCmdHandle(g_checkModuleCmdTable[cmdType].cmd,
															g_checkModuleCmdTable[cmdType].rsp,
															g_checkModuleCmdTable[cmdType].timeoutMs);
			if(commState == WIFI_COM_OK)
			{
				retryCount = 0;
				cmdType = 0xff;
			}
			else if(commState == WIFI_COM_FAIL)
			{
				retryCount++;
				if(retryCount == 3)
				{
					retryCount = 0;
					return WIFI_COM_FAIL;
				}
			}
			break;		
			
			
			
		case AT_CIPSTART:  
			
			commState = AtCmdHandle(g_checkModuleCmdTable[cmdType].cmd,
															g_checkModuleCmdTable[cmdType].rsp,
															g_checkModuleCmdTable[cmdType].timeoutMs);
			if(commState == WIFI_COM_OK)
			{
				retryCount = 0;
				cmdType = 0xff;
			}
			else if(commState == WIFI_COM_FAIL)
			{
				retryCount++;
				if(retryCount == 3)
				{
					retryCount = 0;
					return WIFI_COM_FAIL;
				}
			}
			break;	
			
			
		case AT_CIPMODE:
			
			commState = AtCmdHandle(g_checkModuleCmdTable[cmdType].cmd,
															g_checkModuleCmdTable[cmdType].rsp,
															g_checkModuleCmdTable[cmdType].timeoutMs);
			if(commState == WIFI_COM_OK)
			{
				retryCount = 0;
				cmdType = AT_CIPSEND;
			}
			else if(commState == WIFI_COM_FAIL)
			{
				retryCount++;
				if(retryCount == 3)
				{
					retryCount = 0;
					return WIFI_COM_FAIL;
				}
			}
			break;

	
			
		case AT_CIPSEND:
			
		
			commState = AtCmdHandle(g_checkModuleCmdTable[cmdType].cmd,
															g_checkModuleCmdTable[cmdType].rsp,
															g_checkModuleCmdTable[cmdType].timeoutMs);
			if(commState == WIFI_COM_OK)
			{
				retryCount = 0;
				cmdType = 0xff;
			}
			else if(commState == WIFI_COM_FAIL)
			{
				retryCount++;
				if(retryCount == 3)
				{
					retryCount = 0;
					return WIFI_COM_FAIL;
				}
			}
			break;
			
		default:
			printf("aili context\r\n");

			break;
	}

	return WIFI_COM_WAIT;
}





