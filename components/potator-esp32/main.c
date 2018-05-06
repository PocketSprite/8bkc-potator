#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_partition.h"
#include "8bkc-hal.h"
#include "appfs.h"
#include "powerbtn_menu.h"
#include "8bkc-ugui.h"
#include "ugui.h"
#include "8bkcgui-widgets.h"
#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>

#include "sound.h"
#include "memorymap.h"
#include "supervision.h"
#include "controls.h"
#include "types.h"



esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

static void debug_screen() {
	kcugui_cls();
	UG_FontSelect(&FONT_6X8);
	UG_SetForecolor(C_WHITE);
	UG_PutString(0, 0, "INFO");
	UG_SetForecolor(C_YELLOW);
	UG_PutString(0, 16, "Potator");
	UG_PutString(0, 24, "Gitrev");
	UG_SetForecolor(C_WHITE);
	UG_PutString(0, 32, GITREV);
	UG_SetForecolor(C_YELLOW);
	UG_PutString(0, 40, "Compiled");
	UG_SetForecolor(C_WHITE);
	UG_PutString(0, 48, COMPILEDATE);
	kcugui_flush();

	while (kchal_get_keys()&KC_BTN_SELECT) vTaskDelay(100/portTICK_RATE_MS);
	while (!(kchal_get_keys()&KC_BTN_SELECT)) vTaskDelay(100/portTICK_RATE_MS);
}


static int fccallback(int button, void **glob, char **desc, void *usrptr) {
	if (button & KC_BTN_POWER) {
		int r=powerbtn_menu_show(kcugui_get_fb());
		//No need to save state or anything as we're not in a game.
		if (r==POWERBTN_MENU_EXIT) kchal_exit_to_chooser();
		if (r==POWERBTN_MENU_POWERDOWN) kchal_power_down();
	}
	if (button & KC_BTN_SELECT) debug_screen();
	return 0;
}


BOOL paused = FALSE;

static inline int getpixel(uint16_t *src, int x, int y) {
	uint8_t *srcb=(uint8_t*)src;
	return 3-((srcb[(y*40)+(x/4)]>>(x&3))&3);
}


static inline uint16_t gray(int g, int x) {
	uint16_t r;
	if (x&1) {
		r=((g>>3)<<5)+((g>>3)<<0);
	} else {
		r=((g>>3)<<11)+((g>>3)<<5);
	}
	return r;
}


static void draw(uint16_t *src, uint16_t *dst) {
	//We need to compress 160*160 to 80*64.
	//That means a compression factor of 2 by 2.5.

	/* Blit screen */
	int sy=0;
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 80; x++) {
			int pxa=0, pxb=0;
			if ((y&1)==0) {
				pxa+=getpixel(src, x*2, sy)*2;
				pxa+=getpixel(src, x*2, sy+1)*2;
				pxa+=getpixel(src, x*2, sy+2);
				pxb+=getpixel(src, x*2+1, sy)*2;
				pxb+=getpixel(src, x*2+1, sy+1)*2;
				pxb+=getpixel(src, x*2+1, sy+2);
			} else {
				pxa+=getpixel(src, x*2, sy);
				pxa+=getpixel(src, x*2, sy+1)*2;
				pxa+=getpixel(src, x*2, sy+2)*2;
				pxb+=getpixel(src, x*2+1, sy);
				pxb+=getpixel(src, x*2+1, sy+1)*2;
				pxb+=getpixel(src, x*2+1, sy+2)*2;
			}
			uint16_t g=gray(pxa*17, 0)+gray(pxb*17, 1);
			*dst++=(g<<8)|(g>>8);
		}
		if (y&1) sy+=3; else sy+=2;
	}
}

QueueHandle_t vidQueue;

uint16_t *oledfb;

void videoTask(void *arg) {
	oledfb=malloc(80*64*2);
	uint16_t *screenbuffer;
	while(1) {
		xQueueReceive(vidQueue, &screenbuffer, portMAX_DELAY);
		draw(screenbuffer, oledfb);
		kchal_send_fb(oledfb);
	}
}

#define FPS 60

int app_main() {
	int i;
	kchal_init();
	nvs_flash_init();

	kcugui_init();
	int rom_fd=kcugui_filechooser("*.sv", "Select ROM", fccallback, NULL, 0);
	kcugui_deinit();

	int sz=0;
	uint8_t *romdata;
	spi_flash_mmap_handle_t hrom=NULL;
	appfsEntryInfo(rom_fd, NULL, &sz);
	esp_err_t err=appfsMmap(rom_fd, 0, sz, (const void**)&romdata, SPI_FLASH_MMAP_DATA, &hrom);
	if (err!=ESP_OK) {
		printf("Error: mmap for rom failed\n");
	}
	
	uint16_t *screenbuffer[2];
	screenbuffer[0]=malloc(160*160/4);
	screenbuffer[1]=malloc(160*160/4);

	vidQueue=xQueueCreate(1, sizeof(uint16_t*));
	xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 5, NULL, 1);

	supervision_init(); //Init the emulator	
	supervision_load((u8*)romdata, (uint32)sz);

	uint8_t sndbuf[(BPS/FPS)*2]={0};
	uint8_t sndbuf_hw[BPS/FPS];

	kchal_sound_start(BPS, 2048);
	int bufno=0;
	while(1) {
		while(!paused) {
			controls_update();
			supervision_exec((int16*)screenbuffer[bufno],1);
			xQueueSend(vidQueue, &screenbuffer[bufno], 0);
			for (int i=0; i<(BPS/60)*2; i++) sndbuf[i]=128;
			sound_stream_update(sndbuf, (BPS/FPS)*2);
			for (int i=0; i<(BPS/60); i++) {
				int s=sndbuf[i*2]+sndbuf[i*2+1]-256;
				s=s+128;
				if (s<0) s=0;
				if (s>255) s=255;
//				printf("%x\n", s);
				sndbuf_hw[i]=s;
			}
			kchal_sound_push(sndbuf_hw, (BPS/FPS));
			bufno=bufno?0:1;
			controls_reset();
		}
	}

	kchal_exit_to_chooser();
	return 0;
}

