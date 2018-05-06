#ifndef __TIMER_H__
#define __TIMER_H__

#include "supervision.h"

void p_timer_init();
void p_timer_done();
void p_timer_reset();
void p_timer_write(uint32 addr, uint8 data);
uint8 p_timer_read(uint32 addr);
void p_timer_exec(uint32 cycles);

#endif
