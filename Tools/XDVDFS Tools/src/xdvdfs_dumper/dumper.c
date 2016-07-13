#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdtypes.h>
#include <xfile.h>
#include <xdvdfs.h>
#include <list.h>

// On UNIX systems stricmp is called strcasecmp
#ifndef _WIN32
#define stricmp strcasecmp
#endif

#define DISPLAY if (!Quiet) fprintf

DWORD	FileCount = 0;
DWORD	FolderCount = 0;

typedef struct {
	DWORD	StartSector;
	DWORD   FileSize;
	BYTE	Filename[];

} FILE_ENTRY, *PFILE_ENTRY;

INT32 FileCompare(CONST PVOID sa, CONST PVOID sb)
{
	PFILE_ENTRY a, b;

	a = *((PFILE_ENTRY *)sa);
	b = *((PFILE_ENTRY *)sb);

	return ( ((int)a->StartSector) - ((int)b->StartSector));
}

BOOL	ReadImage(
	PVOID Data,
	PVOID Buffer,
	DWORD StartSector,
	DWORD Size)
{
	PXFILE	File = (PXFILE)Data;

	if (!XFileSetPointer(File, ((INT64)StartSector) * 2048))
		return FALSE;

	if (!XFileRead(File, Buffer, Size * 2048))
		return FALSE;

	return TRUE;
}

BOOL ListEnumerateFiles(
		PXDVDFS_SESSION Session,
		PLIST FileList,
		LPSTR StartPath,
		LPSTR RelativePath)
{
	SEARCH_RECORD	SearchRecord;
	DWORD			ReturnCode;
	LPSTR			FullPath;

	FullPath = (LPSTR)malloc(strlen(StartPath) + strlen(RelativePath) + 1);
	strcpy(FullPath, StartPath);
	strcat(FullPath, RelativePath);

	ReturnCode = XDVDFS_OpenFolder(Session, FullPath, &SearchRecord);
	
	free(FullPath);
		
	if (ReturnCode != XDVDFS_NO_ERROR)
		return FALSE;
	else
	{
		PFILE_ENTRY	FileEntry;

		if (strlen(RelativePath))
		{
			//CloneXB - FileEntry = (PFILE_ENTRY)malloc(strlen(RelativePath) + 3 + 4);
			FileEntry = (PFILE_ENTRY)malloc(strlen(RelativePath) + 3 + 4 + 4);

			FileEntry->StartSector = SearchRecord.StartSector;
			//CloneXB
			FileEntry->FileSize = 0;
			//FileEntry->FileSize = SearchRecord.FileSize;
			//CloneXB
			strcpy(FileEntry->Filename, "<");
			strcat(FileEntry->Filename, RelativePath);
			strcat(FileEntry->Filename, ">");
		}
		else
		{
			//CloneXB - FileEntry = (PFILE_ENTRY)malloc(4 + 4);
			FileEntry = (PFILE_ENTRY)malloc(4 + 4 + 4);

			FileEntry->StartSector = SearchRecord.StartSector;
			//CloneXB
			//FileEntry->FileSize = SearchRecord.FileSize;
			FileEntry->FileSize = 0;
			//CloneXB
			strcpy(FileEntry->Filename, "<");
			strcat(FileEntry->Filename, DIRECTORY_SEPARATOR_STR);
			strcat(FileEntry->Filename, ">");
		}

		ListAdd(FileList, (PVOID)FileEntry);

		FolderCount++;
	}

	ReturnCode = XDVDFS_EnumFiles(Session, &SearchRecord);
	while(ReturnCode == XDVDFS_NO_ERROR)
	{
		if (!(SearchRecord.Attributes & XDVDFS_ATTRIBUTE_DIRECTORY))
		{
			PFILE_ENTRY	FileEntry;

			//CloneXB - FileEntry = (PFILE_ENTRY)malloc(strlen(RelativePath) + strlen(SearchRecord.Filename) + 2 + 4);
			FileEntry = (PFILE_ENTRY)malloc(strlen(RelativePath) + strlen(SearchRecord.Filename) + 2 + 4 + 4);

			FileEntry->StartSector = SearchRecord.FileStartSector;
			//CloneXB
			FileEntry->FileSize = SearchRecord.FileSize;
			//CloneXB
			strcpy(FileEntry->Filename, RelativePath);
			strcat(FileEntry->Filename, DIRECTORY_SEPARATOR_STR);
			strcat(FileEntry->Filename, SearchRecord.Filename);

			ListAdd(FileList, (PVOID)FileEntry);

			FileCount++;
		}

		ReturnCode = XDVDFS_EnumFiles(Session, &SearchRecord);
	}

	// 'Rewind' the searchrecord
	SearchRecord.Position = 0;

	ReturnCode = XDVDFS_EnumFiles(Session, &SearchRecord);
	while(ReturnCode == XDVDFS_NO_ERROR)
	{
		if (SearchRecord.Attributes & XDVDFS_ATTRIBUTE_DIRECTORY)
		{
			char	*NewPath;

			NewPath = (char *)malloc(strlen(RelativePath) +
				strlen(SearchRecord.Filename) + 2);

			strcpy(NewPath, RelativePath);
			strcat(NewPath, DIRECTORY_SEPARATOR_STR);
			strcat(NewPath, SearchRecord.Filename);

			ListEnumerateFiles(Session, FileList, StartPath, NewPath);

			free(NewPath);
		}

		ReturnCode = XDVDFS_EnumFiles(Session, &SearchRecord);
	}

	return TRUE;
}

void Usage(void)
{
	fprintf(stderr, "XDVDFS_Dumper v2.0 (by CloneXB)\n\n"
					"xdvdfs_dumper [Options] [Image File]"
					" [-o OutputFile]\n\nOptions:\n"
					"-h\n   Displays this text\n-quiet\n   Reduce verbosity\n"
					"-newmethod\n   Produces new layout format which includes sector information\n"
					"-partial <StartPath>\n   Select start path for dump.\n\n"
					"Default output file is 'filelist.txt'\n"
#ifdef _WIN32
					"[Image File] can be a device such as \\\\.\\I:\n");
#else
					"[Image File] can be a device such as /dev/dvd\n");
#endif
}

int main(int argc, char *argv[])
{
	PXDVDFS_SESSION	Session;
	XFILE			File;
	LIST			FileList;
	BOOL			Quiet = FALSE;
	BOOL			Dump = FALSE;
	BOOL			NewMethod = FALSE;
	int				InputFilesStart;
	int				InputFilesEnd = 0;
	int				OutputFile;
	int				i;
	char			*InputFileName;
	char			*OutputFileName;
	char			*StartPath = "";

	OutputFile = -1;
	for(i = 1;i < argc;i++)
	{
		if (!stricmp(argv[i], "-o"))
		{
			OutputFile = i + 1;
			InputFilesEnd = i;
			break;
		}
	}

	if ((OutputFile == -1)||(OutputFile > argc))
	{
		OutputFileName = "filelist.txt";
		InputFilesEnd = argc;
	}
	else
		OutputFileName = argv[OutputFile];

	InputFilesStart = -1;
	for(i = 1;i < InputFilesEnd;i++)
	{
		if (!stricmp(argv[i], "-h"))
		{
			Usage();
			return 0;
		}
		else if (!stricmp(argv[i], "-dump"))
		{
			Dump = TRUE;
		}
		else if (!stricmp(argv[i], "-quiet"))
		{
			Quiet = TRUE;
		}
		else if (!stricmp(argv[i], "-newmethod"))
		{
			NewMethod = TRUE;
		}
		else if (!stricmp(argv[i], "-partial"))
		{
			StartPath = argv[i + 1];

			XFileFixSlashes(StartPath);
			XFileRemoveTrailingSlash(StartPath);

			i++;
		}
		else
		{
			InputFilesStart = i;
			break;
		}
	}

	if (InputFilesStart == -1)
	{
		Usage();
		return -1;
	}

	if ((InputFilesEnd - InputFilesStart) > 1)
	{
		fprintf(stderr, "Can't specify more than one input file.\n");
		return -1;
	}

	InputFileName = argv[InputFilesStart];

	DISPLAY(stderr, "Opening input file / device...\n");

	if (!XFileOpen(&File, InputFileName, XFILE_READ))
	{
		fprintf(stderr, "Can't open input file !\n");
		return -1;
	}

	DISPLAY(stderr, "Mounting filesystem...\n");

	Session = (PXDVDFS_SESSION)malloc(sizeof(XDVDFS_SESSION));
	Session->StartSector = 0;
	if (!XDVDFS_Mount(Session, &ReadImage, &File))
	{
		fprintf(stderr, "Coundn't mount XDVDFS filesystem !\n");
		free(Session);
		XFileClose(&File);
		return -1;
	}

	DISPLAY(stderr, "Exploring disk...\n");

	ListCreate(&FileList, 16384, 16384);

	i = ListEnumerateFiles(Session, &FileList, StartPath, "");

	if (Dump)
	{
		XFILE	OutFile;
		XFileOpen(&OutFile, "dump.bin", XFILE_WRITE);
		XFileWrite(&OutFile, Session->Read.DiskBuffer,
			sizeof(Session->Read.DiskBuffer));
		XFileClose(&OutFile);
	}

	XDVDFS_UnMount(Session);
	free(Session);

	XFileClose(&File);

	if (!i)
	{
		printf("Path %s not found.\n", StartPath);
		goto cleanup_exit;
	}

	DISPLAY(stderr, "Found %d files and %d folders.\n", FileCount, FolderCount);
	DISPLAY(stderr, "Sorting...\n");

	ListSort(&FileList, FileCompare);

	DISPLAY(stderr, "Saving info as %s...\n", OutputFileName);

	if (NewMethod)
	{
		// CloneXB's new format of layout file include sector positions and other stuff
		if (!XFileOpen(&File, OutputFileName, XFILE_WRITE))
			fprintf(stderr, "Cant open output file !\n");
		else
		{
			char szBuf[400 + 1];
			char FileOrDir = '\0';
			char FileName[256 + 1];

			// write out header comments
			// as we are reading from dvd-r there is only ever 1 layer - layer 0;
			strcpy(szBuf, "# DISC TITLE: (Unavailable)\n# VIDEO MODE: (Unavailable)\n# PUBLISHER : (Unavailable)\n# EXTRA INFO: None\n# DUMP UTIL : XDVDFS_Dumper v2.0\n#\n# <<<<< LAYER 0 BEGINS HERE >>>>>\n#\n");
			XFileWrite(&File, szBuf, (DWORD)strlen(szBuf));

			for(i = 0;i < (int)FileList.Count; i++)
			{	
				// see if its a file or directory
				if (((PFILE_ENTRY)FileList.Pointers[i])->Filename[0] == '<')
				{
					FileOrDir = 'D';
					strncpy(FileName, &((PFILE_ENTRY)FileList.Pointers[i])->Filename[1], (strlen(((PFILE_ENTRY)FileList.Pointers[i])->Filename) - 2));
					FileName[(strlen(((PFILE_ENTRY)FileList.Pointers[i])->Filename) - 2)] = '\0';
				}
				else
				{
					strcpy(FileName, ((PFILE_ENTRY)FileList.Pointers[i])->Filename);
					FileOrDir = 'F';
				}
				
				sprintf(szBuf, "%c,%s,%d,%d\n", FileOrDir, FileName,
											((PFILE_ENTRY)FileList.Pointers[i])->FileSize,
											((PFILE_ENTRY)FileList.Pointers[i])->StartSector);

				XFileWrite(&File, szBuf, (DWORD)strlen(szBuf));
			}

			XFileClose(&File);
		}
	}
	else
	{
		if (!XFileOpen(&File, OutputFileName, XFILE_WRITE))
			fprintf(stderr, "Cant open output file !\n");
		else
		{
			// we will concatenate all data and write everything in one go.
			DWORD	SizeTotal = 0;
			void	*ConcatBuffer;
			char	*ConcatPtr;

			for(i = 0;i < (int)FileList.Count; i++)
				SizeTotal += strlen(((PFILE_ENTRY)FileList.Pointers[i])->Filename) + 1;

			ConcatBuffer = malloc(SizeTotal);
			ConcatPtr = (char *)ConcatBuffer;

			for(i = 0;i < (int)FileList.Count; i++)
			{	
				memcpy(
					ConcatPtr,
					((PFILE_ENTRY)FileList.Pointers[i])->Filename,
					strlen(((PFILE_ENTRY)FileList.Pointers[i])->Filename));
				ConcatPtr += strlen(((PFILE_ENTRY)FileList.Pointers[i])->Filename);
				*ConcatPtr++ = '\n';
			}

			XFileWrite(&File, ConcatBuffer, SizeTotal);

			XFileClose(&File);

			free(ConcatBuffer);
		}
	}

	DISPLAY(stderr, "Done.\n");

cleanup_exit:
	ListDestroy(&FileList, TRUE);

	return 0;
}

