#ifndef __LISTENUMFILES_H__
#define __LISTENUMFILES_H__

#include <list.h>
#include <xfile.h>
#include "FileEntry.h"

typedef struct {
	PLIST	List;
	DWORD	Index;
	BYTE	SearchPattern[MAX_PATH];
} ENUM_RECORD, * PENUM_RECORD;

extern BOOL ListFindFirstFile(
				PENUM_RECORD SearchRecord,
				PLIST FileList,
				char *Path,
				PFILE_ENTRY *FileEntry);

extern BOOL ListFindNextFile(
				PENUM_RECORD SearchRecord,
				PFILE_ENTRY *FileEntry);

extern BOOL ListFindClose(
				PENUM_RECORD SearchRecord);

#endif
