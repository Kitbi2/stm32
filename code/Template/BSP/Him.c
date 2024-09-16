#include "Him.h"
#include "KEY.h"
#include "Calendar.h"
#include "Temp.h"
#include "secor.h"
#include "stdio.h"
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"
#include "lv_demo_widgets.h"
#include "led_driver.h"
#include  "stm32f1xx_hal.h"
//static uint8_t Led_Status[3] = {0};
//static void event_btn_handler(lv_event_t* e)
//{

//    static lv_style_t share_style;
//    lv_style_init(&share_style);
//    lv_obj_t * event_btn = lv_event_get_target(e);
//    uint8_t * btn_data = lv_event_get_user_data(e); /* ??????? */

//    if(*btn_data == 0)
//    {
//			lv_obj_set_style_bg_color(event_btn,lv_color_make(0xff,00,00),0);
//        
//       *btn_data = 1;


//    }
//    else
//    {
//			lv_obj_set_style_bg_color(event_btn,lv_color_make(0x00,0xff,00),0);
//      *btn_data = 0;
//     

//    }


//		ToogleLed((uint8_t)(btn_data - Led_Status));

//}
//void Him_Demo(void)
//{



//    static lv_style_t share_style;
//    lv_style_init(&share_style);
//    lv_style_set_bg_color(&share_style,lv_color_make(0,0xff,00)); 
//    lv_style_set_bg_opa(&share_style,LV_OPA_100);   
//    for(uint8_t i = 0;i < 3;i++)
//    {
//        lv_obj_t * btn1 =  lv_btn_create(lv_scr_act());
//        lv_obj_set_size(btn1,100,50);   
//        lv_obj_align(btn1,LV_ALIGN_TOP_MID,0,100 * i); 
//        
//        lv_obj_add_style(btn1,&share_style,0);
//        lv_obj_add_event_cb(btn1,event_btn_handler,LV_EVENT_CLICKED,&Led_Status[i]);
//    }


//}

static lv_obj_t * temp_lable1;
static lv_obj_t * temp_lable2;
static void timer_handler(lv_timer_t * timer)
{
/* ܱȡς׈ìҢȒˤԶ */
	sec_t SecorBuf;
	if(Get_Secor_Data(&SecorBuf))
	{
		char buf1[10] = {'\0'};
		sprintf(buf1,"%3d",SecorBuf.ap.ALS);
		lv_label_set_text(temp_lable1,buf1);
		char buf2[10] = {'\0'};
		sprintf(buf2,"%3d",SecorBuf.ap.PS);
		printf("the ps is %3d\r\n",SecorBuf.ap.PS);
		lv_label_set_text(temp_lable2,buf2);
	}	
}
LV_FONT_DECLARE(lv_font_tmp)
LV_FONT_DECLARE(siyuan_24)
void Create_Tille(lv_obj_t * obj,const char * buf)
{
    lv_obj_t * label = lv_label_create(obj);

    /* 字体类型 */
    lv_obj_set_style_text_font(label,&siyuan_24,0);
    /* 字体颜色 */
    lv_label_set_recolor(label,true);

    /* 文本 */
    lv_label_set_text(label,buf);

    /* 位置 */
    lv_obj_set_style_align(label,LV_ALIGN_CENTER,0);

}
LV_IMG_DECLARE(light)
LV_IMG_DECLARE(lrda)

void CreateMainArea(lv_obj_t * parant)
{
    lv_obj_t * label = NULL;
    lv_obj_t *img_obj = NULL;
   /* 图片1 */
   img_obj = lv_img_create(parant);     /* 创建图片1 */
   lv_obj_align(img_obj,LV_ALIGN_TOP_LEFT,0,0);  /* 位置 */
   lv_img_set_src(img_obj,&light);               /* 目标图片 */

   /* label1 */
   label = lv_label_create(parant);
    /* 字体类型 */
    lv_obj_set_style_text_font(label,&siyuan_24,0);
    /* 字体颜色 */
    lv_label_set_recolor(label,true);

    /* 文本 */
    lv_label_set_text(label,"#ff0000光照：");

    /* 位置 */
    lv_obj_align(label,LV_ALIGN_TOP_MID,-20,0);

   /* 图片2 */     
   img_obj = lv_img_create(parant);     /* 创建图片1 */
   lv_obj_align(img_obj,LV_ALIGN_BOTTOM_LEFT,0,0);  /* 位置 */
   lv_img_set_src(img_obj,&lrda);               /* 目标图片 */

   /* label2 */
   label = lv_label_create(parant);
    /* 字体类型 */
    lv_obj_set_style_text_font(label,&siyuan_24,0);
    /* 字体颜色 */
    lv_label_set_recolor(label,true);

    /* 文本 */
    lv_label_set_text(label,"#ff0000 红外：");

    /* 位置 */
    lv_obj_align(label,LV_ALIGN_BOTTOM_MID,-20,0);
		
		
		temp_lable1 = lv_label_create(parant);
		lv_obj_set_style_text_font(temp_lable1,&siyuan_24,0);
		lv_obj_set_style_text_color(temp_lable1,lv_color_make(0xff,00,00),0);
		lv_label_set_text(temp_lable1,"015");
	  lv_obj_align(temp_lable1,LV_ALIGN_TOP_RIGHT,0,0);
		
		temp_lable2 = lv_label_create(parant);
		lv_obj_set_style_text_font(temp_lable2,&siyuan_24,0);
		lv_obj_set_style_text_color(temp_lable2,lv_color_make(0xff,00,00),0);
		lv_label_set_text(temp_lable2,"015");
	  lv_obj_align(temp_lable2,LV_ALIGN_BOTTOM_RIGHT,0,0);

}
LV_FONT_DECLARE(lv_font_tmp)
void Him_Demo(void)
{
    /* 创建一个 屏幕对象 */
    lv_obj_t * scren  = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scren,320,480);
    lv_obj_set_style_bg_color(scren,lv_color_make(0,0,0),0);


    lv_obj_t * tille1  = lv_obj_create(scren);
    /* 大小 */
    lv_obj_set_size(tille1,lv_pct(100),lv_pct(10));
    /* 位置 */

    /* 颜色 */
    lv_obj_set_style_bg_color(tille1,lv_color_make(0,0,0),0);
    /* 取消滚动条 */
    lv_obj_set_scroll_dir(tille1,LV_DIR_NONE);

    /* 边框长度 */
    lv_obj_set_style_border_width(tille1,2,0);

    Create_Tille(tille1,"#ff0000 光照和红外");

    static lv_point_t linePoints[4];
    linePoints[0].x = 10;
    linePoints[0].y = 50;

    linePoints[1].x = 280;
    linePoints[1].y = 50;

    lv_obj_t *line = lv_line_create(scren);
    lv_obj_set_style_line_width(line,2,0);
    lv_obj_set_style_line_color(line,lv_color_make(0x00,0xff,0xff),0);
    lv_line_set_points(line,linePoints,2);



    lv_obj_t * tille2  = lv_obj_create(scren);
    /* 大小 */
    lv_obj_set_size(tille2,lv_pct(100),lv_pct(40));
    /* 位置 */

    /* 颜色 */
    lv_obj_set_style_bg_color(tille2,lv_color_make(0,0,0),0);
    /* 取消滚动条 */
    lv_obj_set_scroll_dir(tille2,LV_DIR_NONE);

    /* 边框长度 */
    lv_obj_set_style_border_width(tille2,2,0);

    lv_obj_center(tille2);

    CreateMainArea(tille2);
		
		lv_timer_create(timer_handler,1000,NULL);
//    lv_obj_t * label = lv_label_create(lv_scr_act());
//    lv_label_set_text(label,"HELLO YJT 黎欣之是二货");
//    lv_obj_set_size(label,310,480);






}
/**
*******************************
* @brief :Եʼۯɋܺݻۥ
* @parm  úvoid
* @returnúvoid 
*******************************
*/
void Him_Init(void)
{
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	Him_Demo();
}

void Him_LCD_Task(void)
{
	lv_task_handler();
}
/**
*******************************
* @brief :٤طŚɝ
* @parm  úvoid
* @returnúvoid 
*******************************
*/
void Him_Ouput(void)
{
	Date_t DateBuf;
	/* ܱȡRTCʱݤìҢȒˤԶ */
	Get_Date(&DateBuf);
	printf("%d/%d/%d %d:%d:%d\r\n",DateBuf.year,DateBuf.mouth,DateBuf.day,
														DateBuf.hour,DateBuf.min,DateBuf.sec);
	/* ܱȡς׈ìҢȒˤԶ */
	sec_t SecorBuf;
	if(Get_Secor_Data(&SecorBuf))
	{
		printf("the tmep is %f\r\n",SecorBuf.temp);
		printf("the ALS is %x\r\n",SecorBuf.ap.ALS);
		printf("the IR is %x\r\n",SecorBuf.ap.IR);
		printf("the PS is %x\r\n",SecorBuf.ap.PS);
	}
}
