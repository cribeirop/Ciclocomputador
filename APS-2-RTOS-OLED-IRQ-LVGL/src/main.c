#include <asf.h>
#include <string.h>
#include "ili9341.h"
#include "lvgl.h"
#include "touch/touch.h"
#include "arm_math.h"
#include "../ASF/thirdparty/lvgl8/examples/lv_examples.h"

#define TASK_SIMULATOR_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_SIMULATOR_STACK_PRIORITY (tskIDLE_PRIORITY)

#define RAIO 0.508/2
#define VEL_MAX_KMH  5.0f
#define VEL_MIN_KMH  0.5f
//#define RAMP

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
LV_FONT_DECLARE(bot);

#define LV_HOR_RES_MAX          (240)
#define LV_VER_RES_MAX          (320)

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
static lv_obj_t * labelBot;


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
/**
* raio 20" => 50,8 cm (diametro) => 0.508/2 = 0.254m (raio)
* w = 2 pi f (m/s)
* v [km/h] = (w*r) / 3.6 = (2 pi f r) / 3.6
* f = v / (2 pi r 3.6)
* Exemplo : 5 km / h = 1.38 m/s
*           f = 0.87Hz
*           t = 1/f => 1/0.87 = 1,149s
*/
float kmh_to_hz(float vel, float raio) {
	float f = vel / (2*PI*raio*3.6);
	return(f);
}

void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type);
volatile char flag_rtc_alarm;
volatile int flag_hour = 12, flag_minute = 46, flag_settings, flag_power = 0;

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
			flag_hour = 23;
		} else if (flag_minute < 0) {
			flag_minute = 59;
		}
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
		if (flag_hour > 23) {
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
	
	static lv_style_t sbot;
// 	lv_style_init(&sbot);
// 	lv_style_set_bg_color(&sbot, lv_palette_main(LV_PALETTE_NONE));
// 	lv_style_set_border_color(&sbot, lv_palette_main(LV_PALETTE_NONE));
// 	lv_style_set_border_width(&sbot, 5);
	lv_style_set_text_font(&sbot, &bot);

	
	static lv_style_t sclk;
	lv_style_init(&sclk);
	lv_style_set_bg_color(&sclk, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_color(&sclk, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_border_width(&sclk, 5);
	lv_style_set_text_font(&sclk, &clk);
		
	lv_obj_t * labelBtn1;
	lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn1, power_handler, LV_EVENT_ALL, NULL);
	lv_obj_align(btn1, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_obj_add_style(btn1, &style, 0);
	labelBtn1 = lv_label_create(btn1);
	lv_label_set_text(labelBtn1, LV_SYMBOL_POWER); //LV_SYMBOL_POWER
	lv_obj_center(labelBtn1);
	lv_obj_set_width(btn1, 60);
	lv_obj_set_height(btn1, 60);
		
	lv_obj_t * labelBtn4;
	lv_obj_t * btn4 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn4, down_handler, LV_EVENT_ALL, NULL);
	lv_obj_align(btn4, LV_ALIGN_BOTTOM_RIGHT, -3, 0);
	lv_obj_add_style(btn4, &style, 0);
	labelBtn4 = lv_label_create(btn4);
	lv_label_set_text(labelBtn4, LV_SYMBOL_DOWN );
	lv_obj_center(labelBtn4);
	lv_obj_set_width(btn4, 60);
	lv_obj_set_height(btn4, 60);
		
	lv_obj_t * labelBtn5;
	lv_obj_t * btn5 = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn5, up_handler, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btn5, btn4, LV_ALIGN_OUT_LEFT_TOP, -35, 0);
	lv_obj_add_style(btn5, &style, 0);
	labelBtn5 = lv_label_create(btn5);
	lv_label_set_text(labelBtn5, LV_SYMBOL_UP );
	lv_obj_center(labelBtn5);
	lv_obj_set_width(btn5, 60);
	lv_obj_set_height(btn5, 60);
	
	labelClock = lv_label_create(lv_scr_act());
	lv_obj_align(labelClock, LV_ALIGN_TOP_RIGHT,0, 0);
	lv_obj_set_style_text_font(labelClock, &dseg30new, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelClock, lv_color_white(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelClock, "%d" ":" "%d", flag_hour, flag_minute);
	
	lv_obj_t * labelRel;
	lv_obj_t * btnRel = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btnRel, event_handler_settings, LV_EVENT_ALL, NULL);
	lv_obj_align_to(btnRel, labelBtn1, LV_ALIGN_OUT_RIGHT_TOP, 18, -16); // +25 foi pra direita do power, -10, fez ele subir
	lv_obj_add_style(btnRel, &sclk, 0);
	labelRel = lv_label_create(btnRel);
	lv_label_set_text(labelRel, "a"); //LV_SYMBOL_POWER
	lv_obj_center(labelRel);
	lv_obj_set_width(btnRel, 60);
	lv_obj_set_height(btnRel, 60);
	
	labelBot = lv_label_create(lv_scr_act());
	lv_obj_align(labelBot, LV_ALIGN_TOP_LEFT, 15, 0); // +25 foi pra direita do power, -10, fez ele subir
	lv_obj_add_style(labelBot, &sbot, 0);
	lv_label_set_text(labelBot, "a"); //LV_SYMBOL_POWER
	lv_obj_set_style_text_color(labelClock, lv_color_white(), LV_STATE_DEFAULT);
	lv_obj_set_width(labelBot, 60);
	lv_obj_set_height(labelBot, 60);
	
// #if LV_USE_ARC
// 	/*Create an Arc*/
// 	lv_obj_t * arc = lv_arc_create(lv_scr_act());
// 	lv_arc_set_end_angle(arc, 200);
// 	lv_obj_set_size(arc, 65, 65);
// 	lv_obj_align(arc, LV_ALIGN_TOP_LEFT, 3, -3);
// #endif
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
static void task_velocidade(void *pvParameters) {
	rtt_init(RTT, 1);
	float velocidade_anterior;
	while(1){
		if (!pio_get(BIKE_PIO,PIO_INPUT,BIKE_IDX_MASK)){
			
			uint32_t delta_t = rtt_read_timer_value(RTT);
			rtt_init(RTT,1);
			//printf("%f",delta_t);
			float t = (float)delta_t/32768;
			float f = 1/t;
			float w = 2*PI*f;
			float velocidade_atual = w*RAIO;
			
			//K = constante opcional(ganho)
			float K = 1;
			
			float aceleracao = K*(velocidade_atual - velocidade_anterior)/delta_t;
			float velocidade_anterior = velocidade_atual;
			
			//printf("%f",t);
			printf("\n\nVelocidade(m/s): %f",velocidade_atual);
			printf("\nAceleracao(m/s): %f",aceleracao);
			printf("\nVelocidade Media(m/s): %f",2*pi*RAIO/t);
		}
		
		//vTaskDelay(1);
	}
	
}

static void task_simulador(void *pvParameters) {

	pmc_enable_periph_clk(ID_PIOC);
	pio_set_output(PIOC, PIO_PC31, 1, 0, 0);

	float vel = VEL_MAX_KMH;
	float f;
	int ramp_up = 1;

	while(1){
		pio_clear(PIOC, PIO_PC31);
		delay_ms(1);
		pio_set(PIOC, PIO_PC31);
#ifdef RAMP
		if (ramp_up) {
			printf("[SIMU] ACELERANDO: %d \n", (int) (10*vel));
			vel += 0.5;
			} else {
			printf("[SIMU] DESACELERANDO: %d \n",  (int) (10*vel));
			vel -= 0.5;
		}

		if (vel >= VEL_MAX_KMH)
		ramp_up = 0;
		else if (vel <= VEL_MIN_KMH)
		ramp_up = 1;
#endif
#ifndef RAMP
		vel = 5;
		printf("[SIMU] CONSTANTE: %d \n", (int) (10*vel));
#endif
		f = kmh_to_hz(vel, RAIO);
		int t = 965*(1.0/f); //UTILIZADO 965 como multiplicador ao invés de 1000
		//para compensar o atraso gerado pelo Escalonador do freeRTOS
		delay_ms(t);
	}
}

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
			lv_label_set_text(labelBtn1, LV_SYMBOL_POWER); //LV_SYMBOL_POWER
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

static void task_lcd(void *pvParameters) {
	int px, py;

	//lv_ex_btn_1();
	lv_termostato();
	lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);

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
	
	data->point.x = py;
	data->point.y = 320 - px;
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
	ili9341_set_orientation(ILI9341_FLIP_Y | ILI9341_SWITCH_XY);
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
	if (xTaskCreate(task_simulador, "SIMUL", TASK_SIMULATOR_STACK_SIZE, NULL, TASK_SIMULATOR_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create lcd task\r\n");
	}
	
	/* Start the scheduler. */
	vTaskStartScheduler();

	while(1){ }
}
