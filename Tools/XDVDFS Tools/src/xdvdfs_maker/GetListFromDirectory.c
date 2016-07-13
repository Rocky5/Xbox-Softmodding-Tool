#include <stdlib.h>
#include <memory.h>
#include <xfile.h>
#include <list.h>

#include "FileEntry.h"
#include "ListEnumFiles.h"
#include "Tools.h"

static BOOL EnumerateDirectory(
				PLIST FileList,
				char *BasePath,
				char *RelativePath)
{
	PFILE_ENTRY		FileEntry;
	XFILE_FIND_DATA	FindData;
	PBYTE			NewPath;

	FileEntry = (PFILE_ENTRY)malloc(strlen(RelativePath) + 16 + 1);
	FileEntry->Attributes = XFILE_ATTRIBUTE_DIRECTORY;
	FileEntry->SizeBytes = 0;
	FileEntry->SizeSectors = 0;
	FileEntry->StartSector = 0;
	strcpy(FileEntry->Filename, RelativePath);
	ListAdd(FileList, (PVOID)FileEntry); 

	NewPath = (PBYTE)malloc(strlen(BasePath) + strlen(RelativePath) + 2);
	strcpy(NewPath, BasePath);
	strcat(NewPath, DIRECTORY_SEPARATOR_STR);
	strcat(NewPath, RelativePath);
	XFileRemoveTrailingSlash(NewPath);

	if (!XFileFindFirst(&FindData, NewPath))
	{
		free(NewPath);
		return FALSE;
	}

	free(NewPath);

	do
	{
		FileEntry = (PFILE_ENTRY)malloc(strlen(RelativePath) + strlen(FindData.Filename) + 16 + 2);
		FileEntry->Attributes = FindData.Attributes;
		FileEntry->SizeBytes = 0;
		FileEntry->SizeSectors = 0;
		FileEntry->StartSector = 0;
		strcpy(FileEntry->Filename, RelativePath);
		if (!XFileHasTrailingSlash(RelativePath))
			strcat(FileEntry->Filename, DIRECTORY_SEPARATOR_STR);
		strcat(FileEntry->Filename, FindData.Filename);

		if (FindData.Attributes == XFILE_ATTRIBUTE_DIRECTORY)
		{
			EnumerateDirectory(FileList, BasePath, FileEntry->Filename);
			free(FileEntry);
		}
		else
			ListAdd(FileList, (PVOID)FileEntry);

	} while(XFileFindNext(&FindData));

	XFileFindClose(&FindData);

	return TRUE;
}

static void CreateOrderListFromAlphaList(
		PLIST OrderList, 
		PLIST AlphaList, 
		PFILE_ENTRY FileEntry)
{
	ENUM_RECORD	EnumRecord;
	
	ListAdd(OrderList, (PVOID)FileEntry);

	if (ListFindFirstFile(&EnumRecord, AlphaList, FileEntry->Filename, &FileEntry))
	{
		do
		{
			if (FileEntry->Attributes == XFILE_ATTRIBUTE_FILE)
				ListAdd(OrderList, (PVOID)FileEntry);
		} while(ListFindNextFile(&EnumRecord, &FileEntry));

		EnumRecord.Index = 0;
		ListFindNextFile(&EnumRecord, &FileEntry);

		do
		{
			if (FileEntry->Attributes == XFILE_ATTRIBUTE_DIRECTORY)
				CreateOrderListFromAlphaList(OrderList, AlphaList, FileEntry);
		} while(ListFindNextFile(&EnumRecord, &FileEntry));
	}

	ListFindClose(&EnumRecord);
}

BOOL GetListFromDirectory(
		PLIST FileAlphaList, 
		PLIST FileOrderList, 
		char *RootDirectory)
{
	if (!EnumerateDirectory(FileAlphaList, RootDirectory, DIRECTORY_SEPARATOR_STR))
		return FALSE;

	ListSort(FileAlphaList, FileListAlphaSort);

	CreateOrderListFromAlphaList(
		FileOrderList,
		FileAlphaList,
		(PFILE_ENTRY)FileAlphaList->Pointers[0]);

	return TRUE;
}
