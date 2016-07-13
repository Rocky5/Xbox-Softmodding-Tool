#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdtypes.h>
#include <xfile.h>
#include <xdvdfs.h>

#ifndef MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif

#define TRANSFER_BUFFER_SIZE	1 * 1048576

// On UNIX systems stricmp is called strcasecmp
#ifndef _WIN32
#define stricmp strcasecmp
#endif

#define DISPLAY if (!Quiet) fprintf

BYTE	TransferBuffer[TRANSFER_BUFFER_SIZE];

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

VOID DisplayGauge(DWORD Progress, DWORD Total)
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

VOID ClearGauge(VOID)
{
	printf("\r                                                         \r");
}

VOID ExtractFile(
		PXDVDFS_SESSION Session,
		PSEARCH_RECORD SearchRecord,
		LPSTR OutFileName)
{
	FILE_RECORD	InputFile;
	XFILE		OutputFile;
	DWORD		Size;

	if (XDVDFS_OpenFileEx(Session, SearchRecord, &InputFile) != XDVDFS_NO_ERROR)
	{
		printf("Can't open input file.\n");
		return;
	}

	if (!XFileOpen(&OutputFile, OutFileName, XFILE_WRITE))
	{
		printf("Can't open output file.\n");
		return;
	}

	Size = InputFile.FileSize;

	while( Size )
	{
		XDVDFS_FileRead(Session, &InputFile, TransferBuffer,
			MIN(Size, TRANSFER_BUFFER_SIZE));
		XFileWrite(&OutputFile, TransferBuffer,
			MIN(Size, TRANSFER_BUFFER_SIZE));

		Size -= MIN(Size, TRANSFER_BUFFER_SIZE);

		DisplayGauge( InputFile.FileSize - Size, InputFile.FileSize );
	}

	XFileClose(&OutputFile);
	XDVDFS_FileClose(Session, &InputFile);

	ClearGauge();
}

BOOL ExtractFiles(
		PXDVDFS_SESSION Session,
		LPSTR ExtractPath,
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
		FullPath = (LPSTR)malloc(strlen(ExtractPath) + strlen(RelativePath) + 1);
		strcpy(FullPath, ExtractPath);
		strcat(FullPath, RelativePath);

		XFileCreatePath(FullPath);

		free(FullPath);
	}

	ReturnCode = XDVDFS_EnumFiles(Session, &SearchRecord);
	while(ReturnCode == XDVDFS_NO_ERROR)
	{
		if (!(SearchRecord.Attributes & XDVDFS_ATTRIBUTE_DIRECTORY))
		{
			FullPath = (LPSTR)malloc(strlen(ExtractPath) + strlen(RelativePath)
								+ strlen(SearchRecord.Filename) + 2);
			strcpy(FullPath, ExtractPath);
			strcat(FullPath, RelativePath);
			strcat(FullPath, DIRECTORY_SEPARATOR_STR);
			strcat(FullPath, SearchRecord.Filename);

			printf("%s"DIRECTORY_SEPARATOR_STR"%s\n",
						RelativePath, 
						SearchRecord.Filename);

			ExtractFile(Session, &SearchRecord, FullPath);

			free(FullPath);
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

			ExtractFiles(Session, ExtractPath, StartPath, NewPath);

			free(NewPath);
		}

		ReturnCode = XDVDFS_EnumFiles(Session, &SearchRecord);
	}

	return TRUE;
}

void Usage(void)
{
	fprintf(stderr, "xdvdfs_extract [Options] [Image File]"
					" [-o OutputPath]\n\nOptions:\n"
					"-h\n   Displays this text\n-quiet\n   Reduce verbosity\n"
					"-partial <StartPath>\n   Select start path for extraction.\n\n"
					"Default output path is '.'\n"
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
	BOOL			Quiet = FALSE;
	int				InputFilesStart;
	int				InputFilesEnd = 0;
	int				OutputFile;
	int				i;
	char			*InputFileName;
	char			*OutputPath;
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
		OutputPath = ".";
		InputFilesEnd = argc;
	}
	else
		OutputPath = argv[OutputFile];

	XFileFixSlashes(OutputPath);
	XFileRemoveTrailingSlash(OutputPath);

	InputFilesStart = -1;
	for(i = 1;i < InputFilesEnd;i++)
	{
		if (!stricmp(argv[i], "-h"))
		{
			Usage();
			return 0;
		}
		else if (!stricmp(argv[i], "-quiet"))
		{
			Quiet = TRUE;
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

	DISPLAY(stderr, "Extracting files...\n");
	i = ExtractFiles(Session, OutputPath, StartPath, "");

	XDVDFS_UnMount(Session);
	free(Session);

	XFileClose(&File);

	if (!i)
		printf("Path %s not found.\n", StartPath);
	else
		printf("Done.\n");

	return 0;
}

