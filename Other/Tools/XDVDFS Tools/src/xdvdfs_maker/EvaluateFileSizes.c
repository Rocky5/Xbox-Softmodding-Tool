#include <stdlib.h>
#include <memory.h>

#include <list.h>
#include <xfile.h>

#include "FileEntry.h"
#include "BuildDirectoryTree.h"
#include "ListEnumFiles.h"
#include "Tools.h"

BOOL EvaluateFileSizes(PLIST FileList, char *RootDir)
{
	PFILE_ENTRY	FileEntry;
	DWORD		i;

	for(i = 0;i < FileList->Count;i++)
	{
		FileEntry = (PFILE_ENTRY)FileList->Pointers[i];

		if (FileEntry->Attributes == XFILE_ATTRIBUTE_FILE)
		{
			PBYTE	CompleteName;
			DWORD	Size;

			CompleteName = (PBYTE)malloc(strlen(FileEntry->Filename) + strlen(RootDir) + 1);
			strcpy(CompleteName, RootDir);
			strcat(CompleteName, FileEntry->Filename);

			if (!XFileGetSize(CompleteName, &Size))
			{
				printf("%s: Can't open file ! Aborting...\n", CompleteName);
				free(CompleteName);
				return FALSE;
			}

			FileEntry->SizeBytes = Size;
			FileEntry->SizeSectors = MakeMultipleOf(Size, 2048) / 2048;

			free(CompleteName);
		}
		else
		{
			PDIRENTRY_INFO	DirentryInfo;
			PDIRENTRY_INFO	Head;
			PDIRENTRY_INFO	Element;
			LPSTR			Filename;
			DWORD			EntrySize;
			DWORD			TotalSize = 0;
			DWORD			NbEntries = 0;

			BuildDirectoryTree(FileList, FileEntry, &DirentryInfo, &NbEntries, &Head);
			if (NbEntries)
			{
				Element = Head;
				while(Element)
				{
					Filename = XFileGetFilename(Element->Entry->Filename);
					EntrySize = MakeMultipleOf(14 + strlen(Filename), 4);

					if ((TotalSize > 0)&&
						((MakeMultipleOf(TotalSize, 2048) - TotalSize) < EntrySize))
						TotalSize = MakeMultipleOf(TotalSize, 2048);

					TotalSize += EntrySize;
					Element = Element->Next;
				}

				FileEntry->SizeBytes = TotalSize;
				FileEntry->SizeSectors = MakeMultipleOf(TotalSize, 2048) / 2048;

				free(DirentryInfo);
			}
			else
			{
				FileEntry->SizeBytes = 0;
				FileEntry->SizeSectors = 0;
			}
		}
	}

	return TRUE;
}
