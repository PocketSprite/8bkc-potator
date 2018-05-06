#ifndef __PTIMER_H__
#define __PTIMER_H__

#include "supervision.h"

void ptimer_init();
void ptimer_done();
void ptimer_reset();
void ptimer_write(uint32 addr, uint8 data);
uint8 ptimer_read(uint32 addr);
void ptimer_exec(uint32 cycles);

#endif
