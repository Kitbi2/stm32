#ifndef LCD_H
#define LCD_H


#define ASSCII16    16
#define ASSCII24    24


/**
*******************************
* @brief : ��ʼ����ʱ���ı���
* @parm  ��void
* @return ��void
*******************************
*/
void LCD_Init(void);

/**
*******************************
* @brief : ��������
* @parm  ��void
* @return ��uint16_t ---���ض�ȡ��������
*******************************
*/
void LCD_Clear(uint16_t color);

/**
*******************************
* @brief : ��ĳ��������䵥����ɫ
* @parm  ��void
* @return ��uint16_t ---���ض�ȡ��������
*******************************
*/
void LCD_Fill_PureColor(uint16_t x_start,uint16_t x_end,uint16_t y_start,uint16_t y_end,uint16_t* color);

/**
*******************************
* @brief : ��ʾͼƬ
* @parm  ��void
* @return ��void
*******************************
*/
void LCD_Show_Pic(uint16_t x_start,uint16_t x_end,uint16_t y_start,uint16_t y_end,uint16_t *color);

/**
*******************************
* @brief : ����
* @parm  ��void
* @return :
*******************************
*/
void LCD_Write_Poin(uint16_t x_start,uint16_t y_start,uint16_t color);
#endif

