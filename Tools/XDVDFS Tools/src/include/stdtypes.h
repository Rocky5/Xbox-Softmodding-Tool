#ifndef __STDTYPES_H__
#define __STDTYPES_H__

#ifdef _WIN32

// When compiling for a WIN32 target we can safely assume
// that the CPU is little endian
#define LITTLE_ENDIAN	3412
#define BYTE_ORDER		LITTLE_ENDIAN

#include <windows.h>

#else

// NULL, if necessary
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else //__cplusplus
#define NULL ((void *) 0)
#endif //__cplusplus
#endif //NULL

// The core types from which every other numeric type derives.
// These are all not so obvious.  They are the same size on all systems.
typedef char INT8, *PINT8;
typedef unsigned char UINT8, *PUINT8;
typedef short INT16, *PINT16;
typedef unsigned short UINT16, *PUINT16;
typedef int INT32, *PINT32;
typedef unsigned int UINT32, *PUINT32;
typedef long long INT64, *PINT64;
typedef unsigned long long UINT64, *PUINT64;

// The core types (Win32 style)
#define VOID void
typedef VOID *PVOID, *LPVOID;
typedef UINT8 BOOL, *PBOOL, *LPBOOL;
typedef UINT8 BYTE, *PBYTE, *LPBYTE;
typedef UINT16 WORD, *PWORD, *LPWORD;
typedef UINT32 DWORD, *PDWORD, *LPDWORD;
typedef UINT32 DWORD32, *PDWORD32, *LPDWORD32;
typedef UINT64 DWORD64, *PDWORD64, *LPDWORD64;

// Character types
typedef INT8 CHAR, *PCHAR;
typedef UINT16 WCHAR, *PWCHAR;
#ifdef UNICODE
typedef WCHAR TCHAR, *PTCHAR;
#else //UNICODE
typedef CHAR TCHAR, *PTCHAR;
#endif

// String types
#define CONST const
typedef CHAR *PSTR, *LPSTR;
typedef CONST CHAR *PCSTR, *LPCSTR;
typedef WCHAR *PWSTR, *LPWSTR;
typedef CONST WCHAR *PCWSTR, *LPCWSTR;
typedef TCHAR *PTSTR, *LPTSTR;
typedef CONST TCHAR *PCTSTR, *LPCTSTR;

// The core types (NT style)
typedef BOOL BOOLEAN, *PBOOLEAN;
typedef UINT8 UCHAR, *PUCHAR;
typedef INT16 SHORT, *PSHORT;
typedef UINT16 USHORT, *PUSHORT;
typedef INT32 INT, *PINT;
typedef UINT32 UINT, *PUINT;
typedef INT32 LONG, *PLONG;
typedef UINT32 ULONG, *PULONG;
typedef INT64 LONGLONG, *PLONGLONG;
typedef UINT64 ULONGLONG, *PULONGLONG;

// Pointer-integer types (for Win64 compatibility)
// These are all 32 bits here because this is the XBOX.
typedef INT32 INT_PTR;
typedef UINT32 UINT_PTR, DWORD_PTR, ULONG_PTR, SIZE_T;
typedef UINT32 *PUINT_PTR, *PDWORD_PTR, *PULONG_PTR, *PSIZE_T;
typedef UINT32 *LPUINT_PTR, *LPDWORD_PTR, *LPULONG_PTR, *LPSIZE_T;

// Boolean values
#define TRUE 1
#define FALSE 0

// Function call stuff
#define IN
#define OUT
#define OPTIONAL
#define FAR
#define NEAR

// GCC-specific definitions
#define PACKED __attribute__((packed))
#define CDECL __attribute__((cdecl))
#define STDCALL __attribute__((stdcall))
#define FASTCALL __attribute__((stdcall))
#define DECLSPEC_NORETURN __attribute__ ((noreturn))
#define INLINE __inline__

#define WINAPI STDCALL
#define WINAPIV CDECL
#define NTAPI STDCALL
#define APIENTRY STDCALL
#define CALLBACK STDCALL
#define APIPRIVATE STDCALL
#define NTSYSAPI extern

// Raw handle type
typedef PVOID HANDLE, *PHANDLE, *LPHANDLE;

typedef struct {
	DWORD LowDateTime;
	DWORD HighDateTime;
} FILETIME, *PFILETIME;

#define min(a,b)  ((a) < (b) ? (a) : (b))
#define max(a,b)  ((a) > (b) ? (a) : (b))

#endif // _WIN32

#endif // __STDTYPES_H__

