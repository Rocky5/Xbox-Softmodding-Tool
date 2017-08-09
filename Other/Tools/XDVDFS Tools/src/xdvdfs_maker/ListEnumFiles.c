#include <memory.h>
#include <xfile.h>

#include "ListEnumFiles.h"

/*typedef struct {
	PLIST	List;
	DWORD	Index;
	BYTE	SearchPattern[MAX_PATH];
} ENUM_RECORD, * PENUM_RECORD;*/

BOOL ListFindFirstFile(
		PENUM_RECORD SearchRecord, 
		PLIST FileList, 
		char *Path, 
		PFILE_ENTRY *FileEntry)
{
	SearchRecord->List = FileList;
	SearchRecord->Index = 0;
	strcpy(SearchRecord->SearchPattern, Path);
	if (!XFileHasTrailingSlash(Path))
		strcat(SearchRecord->SearchPattern, DIRECTORY_SEPARATOR_STR);

	return ListFindNextFile(SearchRecord, FileEntry);
}

BOOL ListFindNextFile(
		PENUM_RECORD SearchRecord,
		PFILE_ENTRY *FileEntry)
{
	DWORD	i;

	for(i = SearchRecord->Index;i < SearchRecord->List->Count;i++)
	{
		*FileEntry = (PFILE_ENTRY)SearchRecord->List->Pointers[i];

		if ((strlen((*FileEntry)->Filename) > strlen(SearchRecord->SearchPattern))&&
			(!strncmp(SearchRecord->SearchPattern, (*FileEntry)->Filename,
				strlen(SearchRecord->SearchPattern)))&&
				(XFileFindDirectorySeparator((*FileEntry)->Filename,
				strlen(SearchRecord->SearchPattern) + 1) == -1))
		{
			SearchRecord->Index = i + 1;
			return TRUE;
		}
	}

	*FileEntry = NULL;
	return FALSE;
}

BOOL ListFindClose(
		PENUM_RECORD SearchRecord)
{
	SearchRecord->Index = 0;
	SearchRecord->List = NULL;

	return TRUE;
}
