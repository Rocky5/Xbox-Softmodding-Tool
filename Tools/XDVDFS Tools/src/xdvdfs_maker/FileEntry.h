#ifndef __FILEENTRY_H__
#define __FILEENTRY_H__

#include <stdtypes.h>

typedef struct {
	DWORD	StartSector;
	DWORD	SizeSectors;
	DWORD	SizeBytes;
	DWORD	Attributes;
	BYTE	Filename[];
} FILE_ENTRY, *PFILE_ENTRY;

#endif
