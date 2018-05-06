#include <stdlib.h>
#include <stdio.h>
#include "controls.h"
#include "8bkc-hal.h"

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

	return(TRUE);
}
