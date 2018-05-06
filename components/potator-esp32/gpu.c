#include "gpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static uint8 gpu_regs[4];

void gpu_init(void){
}

void gpu_done(void){
}


void gpu_reset(void) {
	memset(gpu_regs, 0, 4);
}

void gpu_write(uint32 addr, uint8 data) {
	gpu_regs[(addr&0x03)] = data;
}

uint8 gpu_read(uint32 addr) {
	return(gpu_regs[(addr&0x03)]);
}

void gpu_render_scanline(uint32 scanline, int16 *backbuffer) {
	uint8 *vram_line = &(memorymap_getUpperRamPointer())[(gpu_regs[2] >> 2) + (scanline*0x30)];
	memcpy(backbuffer, vram_line, 160/4);
}

