#ifndef __LIST_H__
#define __LIST_H__

#include <stdtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	DWORD	Capacity;
	DWORD	Count;
	DWORD	Step;
	PVOID	*Pointers;
} LIST, *PLIST;

extern BOOL ListCreate(
				PLIST List,
				DWORD Step,
				DWORD PreAlloc);

extern BOOL ListAdd(
				PLIST List,
				PVOID Data);

extern BOOL ListSort(
				PLIST List,
				int (*Compare)(CONST PVOID, CONST PVOID));

extern BOOL ListDestroy(
				PLIST List,
				BOOL Cleanup);

#ifdef __cplusplus
}
#endif

#endif // __LIST_H__
