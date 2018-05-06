#ifndef __SUPERVISION_H__
#define __SUPERVISION_H__

#include "types.h"
#include "memory.h"
#include "io.h"
#include "gpu.h"
#include "timer.h"
#include "controls.h"
#include "memorymap.h"
#include "interrupts.h"
#include "M6502.h"

extern void supervision_init(void);
extern void supervision_reset(void);
extern void supervision_reset_handler(void);
extern void supervision_done(void);
extern void supervision_exec(int16 *backbuffer, BOOL bRender);
extern BOOL supervision_load(uint8 *rom, uint32 romSize);
extern BOOL supervision_update_input(void);
extern M6502 *supervision_get6502regs(void);
extern void supervision_turnSound(BOOL bOn);

extern int	sv_loadState(char *statepath, int id);
extern int	sv_saveState(char *statepath, int id);

#endif
