
#include <xfile.h>

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <direct.h>

// WIN32 version of XFileOpen
BOOL XFileOpen(
		PXFILE	File,
		LPCSTR	Filename,
		DWORD	Mode)
{
	if (Mode == XFILE_READ)
	{
		*File = CreateFile(
					Filename,
					GENERIC_READ,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,
					NULL);
	} else if (Mode == XFILE_WRITE)
	{
		*File = CreateFile(
					Filename,
					GENERIC_WRITE,
					FILE_SHARE_READ,
					NULL,
					OPEN_ALWAYS,
					FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,
					NULL);
	}
	else
		return FALSE;  // Bad parameter

	if (*File == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;
}

// WIN32 version of XFileSetPointer
BOOL XFileSetPointer(
		PXFILE	File,
		INT64	Position)
{
	LARGE_INTEGER	Displacement;

	Displacement.QuadPart = Position;

	if ((SetFilePointer(*File,
			(LONG)Displacement.LowPart,
			&Displacement.HighPart,
			FILE_BEGIN) == 0xFFFFFFFF)&&
			(GetLastError() != NO_ERROR))
	{
		return FALSE;
	}

	return TRUE;
}

// WIN32 version of XFileRead
BOOL XFileRead(
		PXFILE	File,
		PVOID	Buffer,
		DWORD	Size)
{
	DWORD			BytesRead;

	if (ReadFile(
			*File,
			Buffer,
			Size,
			&BytesRead,
			NULL))
		return TRUE;

	return FALSE;
}

// WIN32 version of XFileWrite
BOOL XFileWrite(
		PXFILE	File,
		PVOID	Buffer,
		DWORD	Size)
{
	DWORD			BytesRead;

	if (WriteFile(
			*File,
			Buffer,
			Size,
			&BytesRead,
			NULL))
		return TRUE;

	return FALSE;
}

// WIN32 version of XFileClose
BOOL XFileClose(PXFILE File)
{
	SetEndOfFile(*File);	// Has no effect on files opened for reading

	CloseHandle(*File);

	return TRUE;
}

// Internal function
static VOID XFileFillFindData(PXFILE_FIND_DATA FindData, PWIN32_FIND_DATA Win32FindData)
{
	strncpy(FindData->Filename, Win32FindData->cFileName, MAX_FILENAME_LENGTH - 1);
	FindData->Filename[MAX_FILENAME_LENGTH - 1] = 0;

	if (Win32FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		FindData->Attributes = XFILE_ATTRIBUTE_DIRECTORY;
	else
		FindData->Attributes = XFILE_ATTRIBUTE_FILE;
}

// WIN32 version of XFileFindFirst
BOOL XFileFindFirst(PXFILE_FIND_DATA FindData, const char *Path)
{
	BYTE	CompletePath[MAX_PATH];

	strcpy(CompletePath, Path);
	strcat(CompletePath, "\\*.*");

	FindData->SearchHandle = FindFirstFile(CompletePath, &FindData->Win32FindData);

	if (FindData->SearchHandle != INVALID_HANDLE_VALUE)
	{
		XFileFillFindData(FindData, &FindData->Win32FindData);

		if (!strcmp(FindData->Filename, "."))
		{
			XFileFindNext(FindData);	// Skip ".."

			if (!XFileFindNext(FindData))
			{
				XFileFindClose(FindData);
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}

// WIN32 version of XFileFindNext
BOOL XFileFindNext(PXFILE_FIND_DATA FindData)
{
	if (FindNextFile(FindData->SearchHandle, &FindData->Win32FindData))
	{
		XFileFillFindData(FindData, &FindData->Win32FindData);
		return TRUE;
	}

	return FALSE;
}

// WIN32 version of XFileFindClose
BOOL XFileFindClose(PXFILE_FIND_DATA FindData)
{
	if (FindData->SearchHandle != INVALID_HANDLE_VALUE)
		FindClose(FindData->SearchHandle);

	return TRUE;
}

// WIN32 version of XFileGetSize
BOOL XFileGetSize(const char *Filename, PDWORD Size)
{
	HANDLE	File;

	File = CreateFile(
				Filename,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
	
	if (File == INVALID_HANDLE_VALUE)
		return FALSE;

	*Size = GetFileSize(File, NULL);

	CloseHandle(File);

	return TRUE;
}

#else

#ifdef linux
	#define __USE_LARGEFILE64
	#define _LARGEFILE64_SOURCE
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef linux	// Linux
	#define READFLAGS				O_RDONLY | O_LARGEFILE
	#define WRITEFLAGS				O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE

	#define lseek					lseek64

	typedef __off64_t				XFILE_OFFSET;

#else	// Other unixes (MacOS, FreeBSD, ...)

	#define READFLAGS				O_RDONLY
	#define WRITEFLAGS				O_WRONLY | O_CREAT | O_TRUNC

	typedef	off_t					XFILE_OFFSET;

#endif

// UNIX version of XFileOpen
BOOL XFileOpen(
		PXFILE	File,
		LPCSTR	Filename,
		DWORD	Mode)
{
	if (Mode == XFILE_READ)
		*File = open(Filename, READFLAGS, 0);
	else if (Mode == XFILE_WRITE)
		*File = open(Filename, WRITEFLAGS, 0644);
	else
		return FALSE;  // Bad parameter

	if (*File == -1)
		return FALSE;

	return TRUE;
}

// UNIX version of XFileSetPointer
BOOL XFileSetPointer(
		PXFILE	File,
		INT64	Position)
{
	XFILE_OFFSET Offset;

	Offset = Position;

	if (lseek(*File, Offset, SEEK_SET) == -1)
		return FALSE;

	return TRUE;
}

// UNIX version of XFileRead
BOOL XFileRead(
		PXFILE	File,
		PVOID	Buffer,
		DWORD	Size)
{
	if (read(*File, Buffer, Size) != Size)
		return FALSE;

	return TRUE;
}

// UNIX version of XFileWrite
BOOL XFileWrite(
		PXFILE	File,
		PVOID	Buffer,
		DWORD	Size)
{
	if (write(*File, Buffer, Size) != Size)
		return FALSE;

	return TRUE;
}

// UNIX version of XFileClose
BOOL XFileClose(PXFILE File)
{
	close(*File);

	return TRUE;
}

// UNIX version of XFileFindFirst
BOOL XFileFindFirst(PXFILE_FIND_DATA FindData, const char *Path)
{
	FindData->Dir = opendir(Path);

	if (FindData->Dir)
	{
		if (XFileFindNext(FindData))
			return TRUE;
		else
		{
			XFileFindClose(FindData);
			return FALSE;
		}
	}

	return FALSE;
}

// UNIX version of XFileFindNext
BOOL XFileFindNext(PXFILE_FIND_DATA FindData)
{
	struct dirent *Entry;

IgnoreEntries:
	Entry = readdir(FindData->Dir);

	if (Entry)
	{
		if (!strcmp(Entry->d_name, ".") || !strcmp(Entry->d_name, ".."))
			goto IgnoreEntries;

#ifdef _DARWIN_
		if (!strcmp(Entry->d_name, ".DS_Store")
                        || !strcmp(Entry->d_name, "Icon\r")
                        || !strcmp(Entry->d_name, ".localized"))
			goto IgnoreEntries;
#endif

		strncpy(FindData->Filename, Entry->d_name, MAX_FILENAME_LENGTH - 1);
		FindData->Filename[MAX_FILENAME_LENGTH - 1] = 0;

		switch(Entry->d_type)
		{
		case	DT_DIR:
			FindData->Attributes = XFILE_ATTRIBUTE_DIRECTORY;
			break;
		case	DT_REG:
			FindData->Attributes = XFILE_ATTRIBUTE_FILE;
			break;
		default:
			FindData->Attributes = 0;
		}

		return TRUE;
	}

	return FALSE;
}

// UNIX version of XFileFindClose
BOOL XFileFindClose(PXFILE_FIND_DATA FindData)
{
	if (FindData->Dir)
	{
		closedir(FindData->Dir);
		return TRUE;
	}

	return FALSE;
}

// UNIX version of XFileGetSize
BOOL XFileGetSize(const char *Filename, PDWORD Size)
{
	struct stat	StatData;

	if (stat(Filename, &StatData))
		return FALSE;

	*Size = StatData.st_size;

	return TRUE;
}

#endif // _WIN32


VOID XFileConvertToBackslash(LPSTR Path)
{
	while(*Path)
	{
		if (*Path == '/')
			*Path = '\\';

		Path++;
	}
}

VOID XFileConvertToSlash(LPSTR Path)
{
	while(*Path)
	{
		if (*Path == '\\')
			*Path = '/';

		Path++;
	}
}

LPSTR XFileGetFilename(LPSTR Path)
{
	int	Position;

	Position = strlen(Path) - 1;

	if (Position <= 0)
		return Path;

	while( (Position > 0)&&( Path[Position] != DIRECTORY_SEPARATOR) )
		Position--;

	if (Path[Position] == DIRECTORY_SEPARATOR)
		Position++;

	return &Path[Position];
}

INT XFileFindDirectorySeparator(LPSTR Path, INT Position)
{
	if (Position < 0)
		return -1;

	if (Position >= (INT)strlen(Path))
		return -1;

	while(Path[Position])
	{
		if (Path[Position] == DIRECTORY_SEPARATOR)
			break;

		Position++;
	}

	if (!Path[Position])
		Position = -1;

	return Position;
}

VOID XFileRemoveTrailingSlash(LPSTR Path)
{
	int	i = strlen(Path) - 1;

	if (i >= 0)
	{
		if (Path[i] == DIRECTORY_SEPARATOR)
			Path[i] = 0;
	}
}

BOOL XFileHasTrailingSlash(LPSTR Path)
{
	int	i = strlen(Path) - 1;

	if (Path[i] == DIRECTORY_SEPARATOR)
		return TRUE;

	return FALSE;
}

static INT XFileFindDirectorySlash(LPSTR Path, INT Position)
{
	if (Position < 0)
		return -1;

	if (Position >= (INT)strlen(Path))
		return -1;

	while(Path[Position])
	{
		if (Path[Position] == '/')
			break;

		Position++;
	}

	if (!Path[Position])
		Position = -1;

	return Position;
}

VOID XFileCreatePath(LPSTR SPath)
{
	int		position = 0;
	char	*path;

	path = (char *)malloc(strlen(SPath) + 1);

	strcpy(path, SPath);

	XFileRemoveTrailingSlash(path);
	XFileConvertToSlash(path);

	while(( position = XFileFindDirectorySlash(path, position) ) != -1)
	{
		if ((!position)||((position == 2)&&(path[1] == ':')))
		{
			position++;
			continue;
		}

		path[position] = 0;

#ifdef _WIN32
		mkdir(path);
#else
		mkdir(path, 0755);
#endif
		
		path[position] = '/';
		position++;
	}

#ifdef _WIN32
		mkdir(path);
#else
		mkdir(path, 0755);
#endif

	free(path);
}

