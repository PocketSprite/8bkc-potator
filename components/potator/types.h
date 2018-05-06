#include <stdint.h>

#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef NULL
#undef NULL
#define NULL	0
#endif

#ifndef BYTE
#define BYTE	unsigned char
#endif

#ifndef WORD
#define WORD	unsigned short
#endif

#ifndef DWORD
#define DWORD	unsigned long
#endif

#ifndef TRUE
#define BOOL	int
#define TRUE	1
#define FALSE	0
#endif

#define BIT(n) (1 << (n))

#define byte   BYTE
//#define word   WORD

#define UINT8	uint8_t
#define UINT16	uint16_t
#define INT8	int8_t
#define INT16	int16_t
#define INT32	int32_t
#define UINT32	uint32_t
#define INT64   int64_t
#define UINT64  uint64_t

#define uint8	UINT8
#define uint16	UINT16
#define uint32	UINT32
#define uint64  UINT64
#define int8	INT8
#define int16	INT16
#define int32	INT32
#define int64   INT64

#define u8		uint8
#define u16		uint16
#define u32		uint32
#define u64		uint64
#define s8		int8
#define s16		int16
#define s32		int32
#define s64		int64


#endif
