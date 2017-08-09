#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#include <sys/types.h>

#if BYTE_ORDER == BIG_ENDIAN
#define MOTOROLA_WORD(a) (a)
#define MOTOROLA_DWORD(a) (a)
#define INTEL_WORD(val) \
			((WORD) ( \
			(((WORD) (val) & (WORD) 0x00ffU) << 8) | \
			(((WORD) (val) & (WORD) 0xff00U) >> 8)))
#define INTEL_DWORD(val) \
			((DWORD) ( \
			(((DWORD) (val) & (DWORD) 0x000000ffU) << 24) | \
		    (((DWORD) (val) & (DWORD) 0x0000ff00U) <<  8) | \
		    (((DWORD) (val) & (DWORD) 0x00ff0000U) >>  8) | \
		    (((DWORD) (val) & (DWORD) 0xff000000U) >> 24)))
#elif BYTE_ORDER == LITTLE_ENDIAN
#define MOTOROLA_WORD(val) \
			((WORD) ( \
			(((WORD) (val) & (WORD) 0x00ffU) << 8) | \
			(((WORD) (val) & (WORD) 0xff00U) >> 8)))
#define MOTOROLA_DWORD(val) \
			((DWORD) ( \
			(((DWORD) (val) & (DWORD) 0x000000ffU) << 24) | \
		    (((DWORD) (val) & (DWORD) 0x0000ff00U) <<  8) | \
		    (((DWORD) (val) & (DWORD) 0x00ff0000U) >>  8) | \
		    (((DWORD) (val) & (DWORD) 0xff000000U) >> 24)))
#define INTEL_WORD(a) (a)
#define INTEL_DWORD(a) (a)
#else
#error "Endian.h: Looks like endianness is not defined."
#endif

#endif
