#ifndef __BUILDDIRECTORYTREE_H__
#define __BUILDDIRECTORYTREE_H__

#include <stdtypes.h>
#include <list.h>

#include "FileEntry.h"

typedef struct {
	WORD	LeftOffset;
	WORD	RightOffset;
	DWORD	StartSector;
	DWORD	TotalSize;
	BYTE	Attributes;
	BYTE	FilenameLength;
	BYTE	Filename[];
} XDVDFS_ENTRY, *PXDVDFS_ENTRY;

typedef struct _DIRENTRY_INFO {
	struct _DIRENTRY_INFO	*Next;
	DWORD					Left;
	DWORD					Right;
	DWORD					Offset;
	DWORD					Flag;
	PFILE_ENTRY				Entry;
} DIRENTRY_INFO, *PDIRENTRY_INFO;

extern void BuildDirectoryTree(
		PLIST FileList,
		PFILE_ENTRY Directory,
		PDIRENTRY_INFO *OutDirentryInfo,
		DWORD *OutNbEntries,
		PDIRENTRY_INFO *OutHead);

#endif
