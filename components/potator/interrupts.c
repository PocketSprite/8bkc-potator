#include "M6502/M6502.h"
#include "interrupts.h"
#include <stdlib.h>
#include <stdio.h>

void interrupts_init()
{
	//fprintf(log_get(), "interrupts init\n");
}

void interrupts_reset()
{
	//fprintf(log_get(), "interrupts reset\n");
}

void interrupts_done()
{
	//fprintf(log_get(), "interrupts done\n");
}

void interrupts_write(uint32 Addr, uint8 data)
{
}

uint8 interrupts_read(uint32 Addr)
{
	return(0xff);
}

void interrupts_exec(uint32 cycles)
{
}

void interrupts_nmi()
{
	Int6502(supervision_get6502regs(), INT_NMI);
}

void interrupts_irq()
{
	Int6502(supervision_get6502regs(), INT_IRQ);
}
