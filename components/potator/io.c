#include "io.h"
#include <stdlib.h>
#include <stdio.h>

static uint8 io_data;

void io_init(void)
{
	//fprintf(log_get(), "io: init\n");
}

void io_reset(void)
{
	//fprintf(log_get(), "io: reset\n");
	io_data = 0x00;
}

void io_done(void)
{
	//fprintf(log_get(), "io: done\n");
}

void io_write(uint32 addr, uint8 data)
{
	io_data = data;		
}

uint8 io_read(uint32 addr)
{
	return(io_data);
}
