#ifndef LCD_H
#define LCD_H


#define ASSCII16    16
#define ASSCII24    24


/**
*******************************
* @brief : 初始化定时器的背光
* @parm  ：void
* @return ：void
*******************************
*/
void LCD_Init(void);

/**
*******************************
* @brief : 清屏操作
* @parm  ：void
* @return ：uint16_t ---返回读取到的数据
*******************************
*/
void LCD_Clear(uint16_t color);

/**
*******************************
* @brief : 对某个域区填充单个颜色
* @parm  ：void
* @return ：uint16_t ---返回读取到的数据
*******************************
*/
void LCD_Fill_PureColor(uint16_t x_start,uint16_t x_end,uint16_t y_start,uint16_t y_end,uint16_t* color);

/**
*******************************
* @brief : 显示图片
* @parm  ：void
* @return ：void
*******************************
*/
void LCD_Show_Pic(uint16_t x_start,uint16_t x_end,uint16_t y_start,uint16_t y_end,uint16_t *color);

/**
*******************************
* @brief : 画点
* @parm  ：void
* @return :
*******************************
*/
void LCD_Write_Poin(uint16_t x_start,uint16_t y_start,uint16_t color);
#endif

