#include "timer.h"
#include <stdlib.h>
#include <stdio.h>

static uint8 timer_regs[2];
static int32 timer_cycles;
static BOOL   timer_activated;

void p_timer_init()
{
	//fprintf(log_get(), "timer: init\n");
}

void p_timer_done()
{
	//fprintf(log_get(), "timer: done\n");
}

void p_timer_reset()
{
	//fprintf(log_get(), "timer: reset\n");
	timer_regs[0]=0x00;
	timer_regs[1]=0x00;
	timer_regs[2]=0x00;

	timer_cycles = 0;
	timer_activated = FALSE;
}

void p_timer_write(uint32 addr, uint8 data)
{
	//iprintf("timer: writing 0x%.2x at 0x%.4x\n", data, addr);
	timer_regs[addr&0x01] = data;

	if (addr&0x01)
	{
		timer_cycles = ((uint32)data)*256;
		timer_activated = TRUE;
		timer_regs[0]&=0xfe;
	}
}

uint8 p_timer_read(uint32 addr)
{
	return(timer_regs[addr&0x01]);
}

void p_timer_exec(uint32 cycles)
{
	if (timer_activated)
	{
		timer_cycles-=cycles;
		
		if (timer_cycles<0)
		{
			timer_regs[0]|=0x01;
//			fprintf(log_get(), "timer: irq\n");
			interrupts_irq();
			timer_activated=FALSE;
		}
	}
}
