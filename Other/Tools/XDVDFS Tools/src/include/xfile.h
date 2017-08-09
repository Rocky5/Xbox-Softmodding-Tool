// Platform independant file I/O (what a pain !)

#ifndef __XFILE_H__
#define __XFILE_H__

#include <stdtypes.h>

#define MAX_FILENAME_LENGTH	256

#define XFILE_ATTRIBUTE_FILE		1
#define XFILE_ATTRIBUTE_DIRECTORY	2

#define XFILE_READ		1
#define XFILE_WRITE		2

#ifdef _WIN32		// Windows

#define DIRECTORY_SEPARATOR		'\\'
#define DIRECTORY_SEPARATOR_STR	"\\"
#define XFileFixSlashes			XFileConvertToBackslash

typedef HANDLE XFILE, *PXFILE;

typedef struct {
	HANDLE			SearchHandle;
	WIN32_FIND_DATA	Win32FindData;

	BYTE			Filename[MAX_FILENAME_LENGTH];
	DWORD			Attributes;
} XFILE_FIND_DATA, *PXFILE_FIND_DATA;

#else				// UNIX, Mac OS

#include <sys/types.h>
#include <dirent.h>

#define DIRECTORY_SEPARATOR		'/'
#define DIRECTORY_SEPARATOR_STR	"/"
#define XFileFixSlashes			XFileConvertToSlash

#define MAX_PATH 4096

// On UNIX systems stricmp is called strcasecmp
#define stricmp strcasecmp

typedef int XFILE, *PXFILE;

typedef struct {
	DIR		*Dir;

	BYTE	Filename[MAX_FILENAME_LENGTH];
	DWORD	Attributes;
} XFILE_FIND_DATA, *PXFILE_FIND_DATA;

#endif // _WIN32

extern BOOL XFileOpen(
				PXFILE	File,
				LPCSTR	Filename,
				DWORD	Mode);

extern BOOL XFileSetPointer(
				PXFILE	File,
				INT64	Position);

extern BOOL	XFileRead(
				PXFILE	File,
				PVOID	Buffer,
				DWORD	Size);

extern BOOL	XFileWrite(
				PXFILE	File,
				PVOID	Buffer,
				DWORD	Size);

extern BOOL XFileClose(PXFILE File);

extern VOID	XFileConvertToBackslash(LPSTR Path);

extern VOID XFileConvertToSlash(LPSTR Path);

extern INT  XFileFindDirectorySeparator(LPSTR Path, INT Position);

extern VOID XFileRemoveTrailingSlash(LPSTR Path);

extern VOID XFileCreatePath(LPSTR SPath);

extern BOOL	XFileFindFirst(PXFILE_FIND_DATA FindData, const char *Path);

extern BOOL	XFileFindNext(PXFILE_FIND_DATA FindData);

extern BOOL	XFileFindClose(PXFILE_FIND_DATA FindData);

extern BOOL	XFileGetSize(const char *Filename, PDWORD Size);

extern BOOL XFileHasTrailingSlash(LPSTR Path);

extern LPSTR XFileGetFilename(LPSTR Path);

#endif // __XFILE_H__
