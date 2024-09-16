#ifndef CALENDAR_H
#define CALENDAR_H
typedef struct
{
	uint32_t year;
	uint16_t mouth;
	uint16_t day;
	uint16_t hour;
	uint16_t min;
	uint16_t sec;
	
}Date_t;
/**
*******************************
* @brief :初始化日历
* @parm  ：void
* @return：void 
*******************************
*/
void Calendar_Init(void);

/**
*******************************
* @brief : 获取时间
* @parm  ：void
* @return：void 
*******************************
*/
void Get_Date(Date_t * buf);

#endif


