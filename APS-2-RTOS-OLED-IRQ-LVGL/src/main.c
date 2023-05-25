#include <asf.h>
#include <string.h>
#include "ili9341.h"
#include "lvgl.h"
#include "touch/touch.h"
/************************************************************************/
/* LCD / LVGL                                                           */
/************************************************************************/
LV_FONT_DECLARE(dseg70);
LV_FONT_DECLARE(dsegC);
LV_FONT_DECLARE(dseg50);
LV_FONT_DECLARE(dseg40);
LV_FONT_DECLARE(dseg20);
LV_FONT_DECLARE(dseg30new);
LV_FONT_DECLARE(dsegreg10);
LV_FONT_DECLARE(clock);
LV_FONT_DECLARE(clk);
LV_FONT_DECLARE(cha);

#define LV_HOR_RES_MAX          (320)
#define LV_VER_RES_MAX          (240)

/*A static or global variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;
/*Static or global buffer(s). The second buffer is optional*/
static lv_color_t buf_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];
static lv_disp_drv_t disp_drv;          /*A variable to hold the drivers. Must be static or global.*/
static lv_indev_drv_t indev_drv;

#define TASK_LCD_STACK_SIZE                (1024*6/sizeof(portSTACK_TYPE))
#define TASK_LCD_STACK_PRIORITY            (tskIDLE_PRIORITY)
#define TASK_RTC_STACK_SIZE                (1024*6/sizeof(portSTACK_TYPE))
#define TASK_RTC_STACK_PRIORITY            (tskIDLE_PRIORITY)

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,  signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);
void lv_termostato(void);

static  lv_obj_t * labelBtn1;
static  lv_obj_t * labelBtn2;
static lv_obj_t * labelBtn3;
static lv_obj_t * labelBtn4;
static lv_obj_t * labelBtn5;
static lv_obj_t * labelBtn6;
static lv_obj_t * labelBtnAA;
lv_obj_t * labelFloor;
lv_obj_t * labelFfloor;
lv_obj_t * labelSetValue;
lv_obj_t * labelClock;
lv_obj_t * labelTemp;
lv_obj_t * labelTTemp;
static lv_obj_t * labelHome;
static lv_obj_t * labelCelsius;
static lv_obj_t * labelRel;
static lv_obj_t * labelConf;
lv_obj_t * labelSet;
lv_obj_t * labelTmp;
lv_obj_t * labelTmp;


typedef struct  {
	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint32_t week;
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
} calendar;

SemaphoreHandle_t xSemaphoreRTC;

void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type);
volatile char flag_rtc_alarm;
volatile int flag_hour = 53, flag_minute = 46, flag_settings, flag_power = 0;

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName) {
	printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	for (;;) {	}
}
extern void vApplicationIdleHook(void) { }
extern void vApplicationTickHook(void) { }
extern void vApplicationMallocFailedHook(void) {
	configASSERT( ( volatile void * ) NULL );
}

/************************************************************************/
/* lvgl                                                                 */
/************************************************************************/

static void event_handler(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);

	if(code == LV_EVENT_CLICKED) {
		LV_LOG_USER("Clicked");
	}
	else if(code == LV_EVENT_VALUE_CHANGED) {
		LV_LOG_USER("Toggled");
	}
}

static void power_handler(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);

	if(code == LV_EVENT_CLICKED && flag_power == 0) {
		flag_power = 1;
		printf("%d", flag_power);
	}
	else if(code == LV_EVENT_CLICKED && flag_power == 1) {
		flag_power = 2;
		printf("%d", flag_power);
	}
	else if(code == LV_EVENT_CLICKED && flag_power == 2) {
		flag_power = 0;
		printf("%d", flag_power);
	}
	else if(code == LV_EVENT_VALUE_CHANGED) {
		LV_LOG_USER("Toggled");
	}
}

static void event_handler_m(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);

	if(code == LV_EVENT_CLICKED) {
		LV_LOG_USER("Clicked");
	}
	else if(code == LV_EVENT_VALUE_CHANGED) {
		LV_LOG_USER("Toggled");
	}
}

static void event_handler_settings(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);

	if(code == LV_EVENT_CLICKED && flag_settings == 0) {
		flag_settings = 1;
	}
	else if(code == LV_EVENT_CLICKED && flag_settings == 1) {
		flag_settings = 2;
	}
	else if(code == LV_EVENT_CLICKED && flag_settings == 2) {
		flag_settings = 0;
	}
	else if(code == LV_EVENT_VALUE_CHANGED) {
		LV_LOG_USER("Toggled");
	}
}

static void down_handler(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	char *c;
	int temp;
	if (code == LV_EVENT_CLICKED && flag_settings > 0) {
		if(code == LV_EVENT_CLICKED && flag_settings == 1) {
			flag_hour -= 1;
		}
		else if(code == LV_EVENT_CLICKED && flag_settings == 2) {
			flag_minute -= 1;
		}
		if (flag_hour < 0) {
			flag_hour = 59;
		} else if (flag_minute < 0) {
			flag_minute = 59;
		}
// 		if (flag_minute < 10 && flag_hour < 10) {
// 			lv_label_set_text_fmt(labelClock, "0%d" ":" "0%d", flag_hour, flag_minute);
// 			} else if (flag_minute < 10) {
// 			lv_label_set_text_fmt(labelClock, "%d" ":" "0%d", flag_hour, flag_minute);
// 			} else if (flag_hour < 10) {
// 			lv_label_set_text_fmt(labelClock, "0%d" ":" "%d", flag_hour, flag_minute);
// 			} else {
// 			lv_label_set_text_fmt(labelClock, "%d" ":" "%d", flag_hour, flag_minute);
// 		}
		lv_label_set_text_fmt(labelClock, "%02d" ":" "%02d", flag_hour, flag_minute);
	}
	else if(code == LV_EVENT_CLICKED) {
		c = lv_label_get_text(labelSetValue);
		temp = atoi(c);
		lv_label_set_text_fmt(labelSetValue, "%02d", temp - 1);
	}
}

static void up_handler(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	char *c;
	int temp;
	if (code == LV_EVENT_CLICKED && flag_settings > 0) {
		if(code == LV_EVENT_CLICKED && flag_settings == 1) {
			flag_hour += 1;
		}
		else if(code == LV_EVENT_CLICKED && flag_settings == 2) {
			flag_minute += 1;
		}
		if (flag_hour > 59) {
			flag_hour = 0;
		} else if (flag_minute > 59) {
			flag_minute = 0;
		}
		lv_label_set_text_fmt(labelClock, "%02d" ":" "%02d", flag_hour, flag_minute);
	}
	else if(code == LV_EVENT_CLICKED) {
		c = lv_label_get_text(labelSetValue);
		temp = atoi(c);
		lv_label_set_text_fmt(labelSetValue, "%02d", temp + 1);
	}
}

void lv_termostato(void) {	
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_width(&style, 5);
	
	static lv_style_t sclk;
	lv_style_init(&sclk);
	lv_style_set_bg_color(&sclk, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_color(&sclk, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_width(&sclk, 5);
	lv_style_set_text_font(&sclk, &clk);
	
	static lv_style_t scha;
	lv_style_init(&scha);
	lv_style_set_bg_color(&scha, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_color(&scha, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_width(&scha, 5);
	lv_style_set_text_font(&scha, &cha);
		
	lv_obj_t * labelBtn1;
	lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn1, power_handler, LV_EVENT_ALL, NULL);
	lv_obj_align(btn1, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_obj_add_style(btn1, &style, 0);
	labelBtn1 = lv_label_create(btn1);
	lv_label_set_text(labelBtn1, " [ " LV_SYMBOL_POWER); //LV_SYMBOL_POWER
	lv_obj_center(labelBtn1);
	lv_obj_set_width(btn1, 60);
	lv_obj_set_height(btn1, 60);
		
	lv_obj_t * labelBtn2;
	lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn2, event_handler_m, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btn2, btn1, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
	lv_obj_add_style(btn2, &style, 0);
	labelBtn2 = lv_label_create(btn2);
	lv_label_set_text(labelBtn2, "|  M | ");
	lv_obj_center(labelBtn2);
	lv_obj_set_width(btn2, 60);
	lv_obj_set_height(btn2, 60);
	
	lv_obj_t * labelBtn3;
	lv_obj_t * btn3 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn3, event_handler_settings, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btn3, btn2, LV_ALIGN_OUT_RIGHT_TOP, 0, 18);
	lv_obj_add_style(btn3, &sclk, 0);
	labelBtn3 = lv_label_create(btn3);
	lv_label_set_text(labelBtn3, "a");
	lv_obj_center(labelBtn3);
	lv_obj_set_width(btn3, 30);
	lv_obj_set_height(btn3, 30);
		
	lv_obj_t * labelBtnAA;
	lv_obj_t * btnAA = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btnAA, event_handler_settings, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btnAA, labelBtn3, LV_ALIGN_OUT_RIGHT_TOP, -1, -23);
	lv_obj_add_style(btnAA, &style, 0);
	labelBtnAA = lv_label_create(btnAA);
	lv_label_set_text(labelBtnAA, " ] ");
	lv_obj_center(labelBtnAA);
	lv_obj_set_width(btnAA, 60);
	lv_obj_set_height(btnAA, 60);
		
	lv_obj_t * labelBtn4;
	lv_obj_t * btn4 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn4, down_handler, LV_EVENT_ALL, NULL);
	lv_obj_align(btn4, LV_ALIGN_BOTTOM_RIGHT, -3, 0);
	lv_obj_add_style(btn4, &style, 0);
	labelBtn4 = lv_label_create(btn4);
	lv_label_set_text(labelBtn4, "  |  " LV_SYMBOL_DOWN " ]");
	lv_obj_center(labelBtn4);
	lv_obj_set_width(btn4, 60);
	lv_obj_set_height(btn4, 60);
		
	lv_obj_t * labelBtn5;
	lv_obj_t * btn5 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn5, up_handler, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btn5, btn4, LV_ALIGN_OUT_LEFT_TOP, -35, 0);
	lv_obj_add_style(btn5, &style, 0);
	labelBtn5 = lv_label_create(btn5);
	lv_label_set_text(labelBtn5, "[  " LV_SYMBOL_UP );
	lv_obj_center(labelBtn5);
	lv_obj_set_width(btn5, 60);
	lv_obj_set_height(btn5, 60);
		
	labelFloor = lv_label_create(lv_scr_act());
	lv_obj_align(labelFloor, LV_ALIGN_LEFT_MID, 45, -35);
	lv_obj_set_style_text_font(labelFloor, &dseg70, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelFloor, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelFloor, "%02d", 23);
		
	labelFfloor = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelFfloor, labelFloor, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	lv_obj_set_style_text_font(labelFfloor, &dseg30new, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelFfloor, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelFfloor, ".%d", 4);
		
	labelSetValue = lv_label_create(lv_scr_act());
	lv_obj_align(labelSetValue, LV_ALIGN_RIGHT_MID, -25, -25);
	lv_obj_set_style_text_font(labelSetValue, &dseg40, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelSetValue, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelSetValue, "%02d", 23);
	
	labelClock = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelClock, labelSetValue, LV_ALIGN_OUT_TOP_MID, -25, -30);
	lv_obj_set_style_text_font(labelClock, &dseg30new, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelClock, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelClock, "%d" ":" "%d", flag_hour, flag_minute);
		
	labelTemp = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelTemp, labelFloor, LV_ALIGN_OUT_LEFT_MID, 10, 0);
	lv_obj_set_style_text_font(labelTemp, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelTemp, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelTemp, "FLOOR\nTEMP");
		
	labelTTemp = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelTTemp, labelFloor, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
	lv_obj_set_style_text_font(labelTTemp, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelTTemp, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelTTemp, "MON");
		
	labelCelsius = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelCelsius, labelSetValue, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
	lv_obj_set_style_text_font(labelCelsius, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelCelsius, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelCelsius, "°C");
		
	lv_obj_t * labelHome;
	lv_obj_t * btnHome = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btnHome, event_handler, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btnHome, btn3, LV_ALIGN_OUT_RIGHT_TOP, 23, -40);
	lv_obj_add_style(btnHome, &style, 0);
	labelHome = lv_label_create(btnHome);
	lv_label_set_text(labelHome, LV_SYMBOL_HOME);
	lv_obj_center(labelHome);
	lv_obj_set_width(btnHome, 30);
	lv_obj_set_height(btnHome, 30);
		
	lv_obj_t * labelRel;
	lv_obj_t * btnRel = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btnRel, event_handler, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btnRel, labelSetValue, LV_ALIGN_OUT_BOTTOM_LEFT, -15, 0);
	lv_obj_add_style(btnRel, &sclk, 0);
	labelRel = lv_label_create(btnRel);
	lv_label_set_text(labelRel, "a"); //LV_SYMBOL_POWER
	lv_obj_center(labelRel);
	lv_obj_set_width(btnRel, 60);
	lv_obj_set_height(btnRel, 60);
		
	lv_obj_t * labelCha;
	lv_obj_t * btnCha = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btnCha, event_handler, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btnCha, labelRel, LV_ALIGN_OUT_RIGHT_TOP, 0, -20);
	lv_obj_add_style(btnCha, &scha, 0);
	labelCha = lv_label_create(btnCha);
	lv_label_set_text(labelCha, "a"); //LV_SYMBOL_POWER
	lv_obj_center(labelCha);
	lv_obj_set_width(btnCha, 60);
	lv_obj_set_height(btnCha, 60);
		
	lv_obj_t * labelConf;
	lv_obj_t * btnConf = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btnConf, event_handler, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btnConf, labelSetValue, LV_ALIGN_OUT_LEFT_MID, -10, -25);
	lv_obj_add_style(btnConf, &style, 0);
	labelConf = lv_label_create(btnConf);
	lv_label_set_text(labelConf, LV_SYMBOL_SETTINGS); //LV_SYMBOL_POWER
	lv_obj_center(labelConf);
	lv_obj_set_width(btnConf, 30);
	lv_obj_set_height(btnConf, 30);
		
	labelSet = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelSet, labelConf, LV_ALIGN_OUT_BOTTOM_MID, 12, 0);
	lv_obj_set_style_text_font(labelSet, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelSet, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelSet, "MON");

}

void lv_ex_btn_1(void) {
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_width(&style, 5);
	
	lv_obj_t * labelBtn1;

	lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
	lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

	labelBtn1 = lv_label_create(btn1);
	lv_label_set_text(labelBtn1, "Teste");
	lv_obj_center(labelBtn1);
}

/************************************************************************/
/* TASKS                                                                */
/************************************************************************/

static void task_rtc(void *pvParameters) {
	calendar rtc_initial = {2023, 4, 22, 12, 16, 17, 46};
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_ALREN);

	uint32_t current_hour, current_min, current_sec;
	uint32_t current_year, current_month, current_day, current_week;
	rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);
	rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
	int seconds = 0;
	uint32_t last_print_time = current_sec;

	while (1) {
		if (flag_power == 1) {
			static lv_style_t style;
			lv_style_init(&style);
			lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_NONE));
			lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_NONE));
			lv_style_set_border_width(&style, 5);
			
			lv_obj_clean(lv_scr_act());
			
			lv_obj_t * labelBtn1;
			lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
			lv_obj_add_event_cb(btn1, power_handler, LV_EVENT_ALL, NULL);
			lv_obj_align(btn1, LV_ALIGN_BOTTOM_LEFT, 0, 0);
			lv_obj_add_style(btn1, &style, 0);
			labelBtn1 = lv_label_create(btn1);
			lv_label_set_text(labelBtn1, " [ " LV_SYMBOL_POWER); //LV_SYMBOL_POWER
			lv_obj_center(labelBtn1);
			lv_obj_set_width(btn1, 60);
			lv_obj_set_height(btn1, 60);
		}
		else if(flag_power == 2) {
			lv_termostato();
			flag_power = 0;
		}
		rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
		rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 1);

		while (current_sec - last_print_time < 1) {
			rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
		}
		
		seconds += 1;
		if (seconds > 59) {
			flag_minute += 1;
			if (flag_minute > 59) {
				flag_hour += 1;
				if (flag_hour > 59) {
					flag_hour = 0;
				}
				flag_minute = 0;
			}
			seconds = 0;		
			//lv_label_set_text_fmt(labelClock, "%02d" ":" "%02d", flag_hour, flag_minute);	
		}
		if (seconds % 2 == 0 || seconds == 0) {
			lv_label_set_text_fmt(labelClock, "%02d" ":" "%02d", flag_hour, flag_minute);
		}
		else {
			lv_label_set_text_fmt(labelClock, "%02d" " " "%02d", flag_hour, flag_minute);
		}
		//lv_label_set_text_fmt(labelClock, "%02d" ":" "%02d", flag_hour, flag_minute);
		//printf("oi\n");
		
		last_print_time = current_sec;		
	//	}
	}
}

//TASK RTC
/*static void task_rtc(void *pvParameters) {
	calendar rtc_initial = {2023, 4, 22, 12, 16, 17, 46};
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_ALREN);

	uint32_t current_hour, current_min, current_sec;
	uint32_t current_year, current_month, current_day, current_week;
	
	rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);
	rtc_get_time(RTC, &current_hour, &current_min, &current_sec);

	// Set the initial alarm time
	rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
	rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 1);

	 while (1) {
		 // Wait for the semaphore to be given
		 xSemaphoreTake(xSemaphoreRTC, portMAX_DELAY);

		 rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);
		 rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
		
		printf("tchau\n");
		rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 1);
// 		// Delay for 2 seconds
		vTaskDelay(pdMS_TO_TICKS(2000));

		// Get current time
		rtc_get_time(RTC, &current_hour, &current_min, &current_sec);

		// Update RTC alarm to trigger after 2 seconds from the new current second
		rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
		rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 2);
	}*/

//TASK RTC
/*static void task_rtc(void *pvParameters) {
	calendar rtc_initial = {2023, 4, 22, 12, 16, 17, 46};
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_SECEN);

	uint32_t current_hour, current_min, current_sec;
	uint32_t current_year, current_month, current_day, current_week;
	rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);
	rtc_get_time(RTC, &current_hour, &current_min, &current_sec);

	uint32_t last_print_time = current_sec;

	while (1) {
		// Update RTC alarm to trigger after 2 seconds from the current second
		rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
		rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 2);

		printf("oi\n");
		
		// Wait for the semaphore to be given
		xSemaphoreTake(xSemaphoreRTC, portMAX_DELAY);

		printf("tchau\n");

		// Delay for 2 seconds
		vTaskDelay(pdMS_TO_TICKS(2000));

		// Get current time
		rtc_get_time(RTC, &current_hour, &current_min, &current_sec);

		// Update RTC alarm to trigger after 2 seconds from the new current second
		rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
		rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 2);
	}*/

static void task_lcd(void *pvParameters) {
	int px, py;

	//lv_ex_btn_1();
	lv_termostato();
	
	for (;;)  {
		lv_tick_inc(50);
		lv_task_handler();
		vTaskDelay(50);
	}
}

/************************************************************************/
/* configs                                                              */
/************************************************************************/

static void configure_lcd(void) {
	/**LCD pin configure on SPI*/
	pio_configure_pin(LCD_SPI_MISO_PIO, LCD_SPI_MISO_FLAGS);  //
	pio_configure_pin(LCD_SPI_MOSI_PIO, LCD_SPI_MOSI_FLAGS);
	pio_configure_pin(LCD_SPI_SPCK_PIO, LCD_SPI_SPCK_FLAGS);
	pio_configure_pin(LCD_SPI_NPCS_PIO, LCD_SPI_NPCS_FLAGS);
	pio_configure_pin(LCD_SPI_RESET_PIO, LCD_SPI_RESET_FLAGS);
	pio_configure_pin(LCD_SPI_CDS_PIO, LCD_SPI_CDS_FLAGS);
	
	ili9341_init();
	ili9341_backlight_on();
}

static void configure_console(void) {
	const usart_serial_options_t uart_serial_options = {
		.baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT,
	};

	/* Configure console UART. */
	stdio_serial_init(CONSOLE_UART, &uart_serial_options);

	/* Specify that stdout should not be buffered. */
	setbuf(stdout, NULL);
}

void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type) {
	/* Configura o PMC */
	pmc_enable_periph_clk(ID_RTC);

	/* Default RTC configuration, 24-hour mode */
	rtc_set_hour_mode(rtc, 0);

	/* Configura data e hora manualmente */
	rtc_set_date(rtc, t.year, t.month, t.day, t.week);
	rtc_set_time(rtc, t.hour, t.minute, t.second);

	/* Configure RTC interrupts */
	NVIC_DisableIRQ(id_rtc);
	NVIC_ClearPendingIRQ(id_rtc);
	NVIC_SetPriority(id_rtc, 4);
	NVIC_EnableIRQ(id_rtc);

	/* Ativa interrupcao via alarme */
	rtc_enable_interrupt(rtc,  irq_type);
}

void RTC_Handler(void) {
	uint32_t ul_status = rtc_get_status(RTC);
	
	/* seccond tick */
	if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {
		// o código para irq de segundo vem aqui
	}
	
	/* Time or date alarm */
	if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
		// o código para irq de alame vem aqui
// 		flag_rtc_alarm = 1;
// 		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
// 		xSemaphoreGiveFromISR(xSemaphoreRTC, &xHigherPriorityTaskWoken);
		rtc_clear_status(RTC, RTC_SCCR_ALRCLR);

        // Give the semaphore from the interrupt context
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(xSemaphoreRTC, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

	rtc_clear_status(RTC, RTC_SCCR_SECCLR);
	rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
	rtc_clear_status(RTC, RTC_SCCR_ACKCLR);
	rtc_clear_status(RTC, RTC_SCCR_TIMCLR);
	rtc_clear_status(RTC, RTC_SCCR_CALCLR);
	rtc_clear_status(RTC, RTC_SCCR_TDERRCLR);
}

/************************************************************************/
/* port lvgl                                                            */
/************************************************************************/

void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
	ili9341_set_top_left_limit(area->x1, area->y1);   ili9341_set_bottom_right_limit(area->x2, area->y2);
	ili9341_copy_pixels_to_screen(color_p,  (area->x2 + 1 - area->x1) * (area->y2 + 1 - area->y1));
	
	/* IMPORTANT!!!
	* Inform the graphics library that you are ready with the flushing*/
	lv_disp_flush_ready(disp_drv);
}

void my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data) {
	int px, py, pressed;
	
	if (readPoint(&px, &py))
		data->state = LV_INDEV_STATE_PRESSED;
	else
		data->state = LV_INDEV_STATE_RELEASED; 
	
	data->point.x = px;
	data->point.y = py;
}

void configure_lvgl(void) {
	lv_init();
	lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);
	
	lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
	disp_drv.draw_buf = &disp_buf;          /*Set an initialized buffer*/
	disp_drv.flush_cb = my_flush_cb;        /*Set a flush callback to draw to the display*/
	disp_drv.hor_res = LV_HOR_RES_MAX;      /*Set the horizontal resolution in pixels*/
	disp_drv.ver_res = LV_VER_RES_MAX;      /*Set the vertical resolution in pixels*/

	lv_disp_t * disp;
	disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
	
	/* Init input on LVGL */
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = my_input_read;
	lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
}

/************************************************************************/
/* main                                                                 */
/************************************************************************/
int main(void) {
	/* board and sys init */
	WDT->WDT_MR = WDT_MR_WDDIS;   
	board_init();
	sysclk_init();
	configure_console();

	/* LCd, touch and lvgl init*/
	configure_lcd();
	configure_touch();
	configure_lvgl();
	
	xSemaphoreRTC = xSemaphoreCreateBinary();
	if (xSemaphoreRTC == NULL) {
		printf("falha em criar o semaforo \n");
	}
	/* Create task to control oled */
	if (xTaskCreate(task_lcd, "LCD", TASK_LCD_STACK_SIZE, NULL, TASK_LCD_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create lcd task\r\n");
	}
	if (xTaskCreate(task_rtc, "LCD", TASK_RTC_STACK_SIZE, NULL, TASK_RTC_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create rtc task\r\n");
	}
	
	/* Start the scheduler. */
	vTaskStartScheduler();

	while(1){ }
}
