#ifndef __BUILDDIRECTORYTABLE_H__
#define __BUILDDIRECTORYTABLE_H__

#include <list.h>
#include "FileEntry.h"

extern void BuildDirectoryTable(
				PLIST FileList, 
				PFILE_ENTRY Directory, 
				PBYTE Buffer);

#endif
