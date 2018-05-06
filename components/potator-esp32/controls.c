
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>
#include <stdlib.h>
#include <stdio.h>
#include "controls.h"
#include "8bkc-hal.h"
#include "powerbtn_menu.h"

uint8 controls_state;

void controls_init()
{
}

void controls_done()
{
}

void controls_reset()
{
	controls_state = 0x00;
}

void controls_write(uint32 addr, uint8 data)
{
}

void controls_state_write(uint8 type, uint8 data)
{
     if(controls_state==data) 
        return; 
     else 
        controls_state = 0;
     
     if(type)
       controls_state|=data;
     else
       controls_state=data;
}

uint8 controls_read(uint32 addr)
{
	return(controls_state^0xff); 
}

//defined in main.c
extern uint16_t *oledfb;

BOOL controls_update(void)
{
	controls_state = 0;
	int keys=kchal_get_keys();

	if (keys & KC_BTN_RIGHT)	controls_state|=0x01;
	if (keys & KC_BTN_LEFT)		controls_state|=0x02;
	if (keys & KC_BTN_DOWN)		controls_state|=0x04;
	if (keys & KC_BTN_UP)		controls_state|=0x08;
	if (keys & KC_BTN_A)		controls_state|=0x10;
	if (keys & KC_BTN_B)		controls_state|=0x20;
	if (keys & KC_BTN_SELECT)	controls_state|=0x40;
	if (keys & KC_BTN_START)	controls_state|=0x80;

	if (keys & KC_BTN_POWER) {
		//Show powerbutton menu. Not customized in any sense atm. This should really have select new game,
		//reset emu, etc functionality.
		vTaskDelay(10); //hack: make sure video task is done with framebuffer
		kchal_sound_mute(1);
		int r=powerbtn_menu_show(oledfb);
		if (r==POWERBTN_MENU_EXIT) kchal_exit_to_chooser();
		if (r==POWERBTN_MENU_POWERDOWN) kchal_power_down();
		kchal_sound_mute(0);
	}

	return(TRUE);
}
