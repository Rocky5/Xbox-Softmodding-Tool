#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <stdio.h>
#include <stdtypes.h>
#include <xfile.h>

extern INT32 FileListAlphaSort(CONST PVOID sa, CONST PVOID sb);

extern void DisplayGauge(DWORD Progress, DWORD Total);

extern void ClearGauge(void);

extern DWORD MakeMultipleOf(DWORD Value, DWORD Multiple);

extern void WriteZeroedSectors(
					PXFILE File, 
					DWORD Number, 
					PBYTE TransferBuffer, 
					DWORD TransferBufferSize,
					BOOL DisplayProgress);

#endif




