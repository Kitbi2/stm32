#ifndef WIFI_APP_H
#define WIFI_APP_H

typedef enum
{
	WIFI_COMM_WAIT,
	WIFI_COMM_OK,
	WIFI_COMM_FAIL,
} WifiComState_t;

WifiComState_t CheckWifiModuleWork(void);

 WifiComState_t AtCmdHandle(char * cmd,char * rsp,uint32_t timeout);

#endif


