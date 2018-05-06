#ifndef __SOUND_H__
#define __SOUND_H__

#include "supervision.h"

#define BPS			22050


void sound_init();
void sound_reset();
void sound_done();
void sound_write(uint32 Addr, uint8 data);
void sound_noise_write(uint32 Addr, uint8 data);
void sound_audio_dma(uint32 Addr, uint8 data);
void sound_exec(uint32 cycles);
void audio_turnSound(BOOL bOn);
void soundport_w(int which, int offset, int data);
void svision_noise_w(int offset, int data);
void svision_sounddma_w(int offset, int data);
void sound_decrement();
void sound_stream_update(uint8 *stream, int len);

#endif
