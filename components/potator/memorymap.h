#ifndef __MEMORYMAP_H__
#define __MEMORYMAP_H__

#include "supervision.h"

void memorymap_init();
void memorymap_done();
void memorymap_reset();
uint8  memorymap_registers_read(uint32 Addr);
void memorymap_registers_write(uint32 Addr, uint8 Value);
void memorymap_load(uint8 *rom, uint32 size);

uint8 *memorymap_getUpperRamPointer(void);
uint8 *memorymap_getLowerRamPointer(void);
uint8 *memorymap_getUpperRomBank(void);
uint8 *memorymap_getLowerRomBank(void);
uint8 *memorymap_getRegisters(void);
uint8 *memorymap_getRomPointer(void);

extern uint8	*memorymap_programRom;
extern uint8	*memorymap_lowerRam;
extern uint8	*memorymap_upperRam;
extern uint8	*memorymap_lowerRomBank;
extern uint8	*memorymap_upperRomBank;
extern uint8	*memorymap_regs;

void Wr6502(register uint16_t Addr,register byte Value);

//Inlined and static for MOAR SPEED!
inline static byte Rd6502(register uint16_t Addr)
{	
	switch (Addr>>12)
	{
	case 0x0:
	case 0x1:	return(memorymap_lowerRam[Addr]);
	case 0x2:
	case 0x3:	return(memorymap_registers_read(Addr));
	case 0x4:
	case 0x5:	return(memorymap_upperRam[Addr&0x1fff]);
	case 0x6:
	case 0x7:	return(memorymap_programRom[Addr&0x1fff]);
	case 0x8:
	case 0x9:
	case 0xa:
	case 0xb: 	return(memorymap_lowerRomBank[Addr&0x3fff]);
	case 0xc:
	case 0xd:
	case 0xe:
	case 0xf: 	return(memorymap_upperRomBank[Addr&0x3fff]);
	}
	return(0xff);
}


#endif

