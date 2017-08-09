#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>
#include <xfile.h>
#include <md5.h>

#define BUFFER_SIZE	1048576

BYTE	TransferBuffer[BUFFER_SIZE];

static BOOL EnumerateDirectory(
				PLIST FileList,
				char *BasePath,
				char *RelativePath)
{
	XFILE_FIND_DATA	FindData;
	PBYTE			FileEntry;
	PBYTE			NewPath;

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
		FileEntry = (PBYTE)malloc(strlen(RelativePath) + strlen(FindData.Filename) + 2);
		strcpy(FileEntry, RelativePath);
		if (!XFileHasTrailingSlash(RelativePath))
			strcat(FileEntry, DIRECTORY_SEPARATOR_STR);
		strcat(FileEntry, FindData.Filename);

		if (FindData.Attributes == XFILE_ATTRIBUTE_DIRECTORY)
		{
			EnumerateDirectory(FileList, BasePath, FileEntry);
			free(FileEntry);
		}
		else
			ListAdd(FileList, (PVOID)FileEntry);

	} while(XFileFindNext(&FindData));

	XFileFindClose(&FindData);

	return TRUE;
}

BOOL GetListFromDirectory(
		PLIST FileList, 
		char *RootDirectory)
{
	if (!EnumerateDirectory(FileList, RootDirectory, DIRECTORY_SEPARATOR_STR))
		return FALSE;

	return TRUE;
} 

void DisplayGauge(DWORD Progress, DWORD Total)
{
	DWORD	Percent, i;

	Percent = (DWORD)( (float)Progress * 100 / Total );

	if (Percent > 100)
		Percent = 100;

	printf("\r[");

	for(i=0;i < 100;i += 2)
	{
		if (i <= Percent)
			printf("*");
		else
			printf(".");
	}

	printf("] %d%%", Percent);

#ifndef _WIN32
	fflush(stdout);
#endif
}

void ClearGauge(void)
{
	printf("\r                                                         \r");
}

DWORD MakeMultipleOf(DWORD Value, DWORD Multiple)
{
	if (Value % Multiple)
		Value += Multiple - (Value % Multiple);

	return Value;
}

void Usage(void)
{
	fprintf(stderr, "create_md5sum [Options] [Root Directory]"
					"\n\nOptions:\n"
					"-h\n   Displays this text.\n"
					"-o [Filename]\n   Will output the md5sums to the specified file.\n"
					"\n\n");
} 

int main(int argc, char *argv[])
{
	XFILE				InFile;
	XFILE				OutFile;
	struct	MD5Context	Context;
	BYTE				MD5Digest[16];
	LIST				FileList;
	DWORD				FileSize;
	DWORD				Remainder;
	BYTE				FilenameSize;
	DWORD				i;
	PBYTE				RootPath = NULL;
	PBYTE				OutFilename = "md5sums";

	// Check for sufficient number of arguments
	if (argc < 2)
	{
		Usage();
		return 0;
	}
	
	// Parse command line options
	for(i = 1;i < (DWORD)argc;i++)
	{
		if (argv[i][0] == '-')
		{
			if (!stricmp(argv[i], "-h"))
			{
				Usage();
				return 0;
			}
			else if (!stricmp(argv[i], "-o"))
			{
				i++;
				OutFilename = argv[i];
			}
			else
				printf("%s: Unknown option.\n", argv[i]);
		}
		else
		{
			if (!RootPath)
				RootPath = argv[i];
			else
				printf("%s: Ignoring extra parameter.\n", argv[i]);
		}
	}
 
	// Check if we were given enough information to do our work
	if (!RootPath)
	{
		printf("You MUST specify a root directory.\n");
		return -1;
	}
 
	XFileRemoveTrailingSlash(RootPath);
 
	ListCreate(&FileList, 1024, 1024);

	GetListFromDirectory(&FileList, RootPath);

	if (!XFileOpen(&OutFile, OutFilename, XFILE_WRITE))
	{
		printf("%s: Can't open / create file.\n", OutFilename);
		ListDestroy(&FileList, TRUE);
		return -1;
	}

	for(i=0;i<FileList.Count;i++)
	{
		strcpy(TransferBuffer, RootPath);
		strcat(TransferBuffer, DIRECTORY_SEPARATOR_STR);
		strcat(TransferBuffer, FileList.Pointers[i]);

		if (!XFileGetSize(TransferBuffer, &FileSize))
		{
			printf("%s: Can't get file size.\n", TransferBuffer);
			ListDestroy(&FileList, TRUE);
			XFileClose(&OutFile);
			return -1;
		}

		if (!XFileOpen(&InFile, TransferBuffer, XFILE_READ))
		{
			printf("%s: Can't open file, aborting...\n", TransferBuffer);
			ListDestroy(&FileList, TRUE);
			XFileClose(&OutFile);
			return -1;
		}
 
		puts(FileList.Pointers[i]);

		MD5Init(&Context);
		memset(MD5Digest, 0, sizeof(MD5Digest));

		Remainder = FileSize;
		while(Remainder > 0)
		{
			#ifdef _WIN32
			// Under Windows we must make sure each read operation is for a size that is a 
			// multiple of a sector (see FILE_FLAG_NO_BUFFERING)
			XFileRead(&InFile, TransferBuffer, MakeMultipleOf(min(Remainder, sizeof(TransferBuffer)), 2048));
			#else
			XFileRead(&InFile, TransferBuffer, min(Remainder, sizeof(TransferBuffer)));
			#endif
			MD5Update(&Context, TransferBuffer, min(Remainder, sizeof(TransferBuffer)));

			Remainder -= min(Remainder, sizeof(TransferBuffer));
			DisplayGauge(FileSize - Remainder, FileSize);
		}
 		
		MD5Final(MD5Digest, &Context);

		FilenameSize = strlen(FileList.Pointers[i]);
		XFileConvertToBackslash(FileList.Pointers[i]);
		XFileWrite(&OutFile, MD5Digest, 16);
		XFileWrite(&OutFile, &FilenameSize, 1);
		XFileWrite(&OutFile, FileList.Pointers[i], FilenameSize);

		XFileClose(&InFile);
		ClearGauge();
	}
	
	XFileClose(&OutFile);
	ListDestroy(&FileList, TRUE);

	puts("Done.\n");
	return 0;
}
