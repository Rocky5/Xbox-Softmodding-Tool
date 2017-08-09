#ifndef __BUFFERED_IO_H__
#define __BUFFERED_IO_H__

#include <stdtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// Determines how many sectors are buffered in each instance of CDIO_READ
#define DISK_BUFFER		64

typedef struct {
	DWORD	SectorList[DISK_BUFFER];		// Ring buffer for buffered disk i/o
	DWORD	LockList[DISK_BUFFER];			// Lock for each buffered sector
	BYTE	DiskBuffer[2048 * DISK_BUFFER];	// Storage room for buffered sectors
	DWORD	WriteIndex;						// Write pointer

	// Pointer to arbitrary data passed at init
	// (usually a file or device handle)
	PVOID	Data;

	BOOL	(*Sectors)(						// Routine to get sectors
				PVOID	Data,				//  Pointer to arbitrary data
				PVOID	Buffer,				//  Buffer to fill
				DWORD	StartSector,		//  Start sector
				DWORD	ReadSize);			//  Number of sectors to read

} CDIO_READ, *PCDIO_READ;

// Get a sector from buffer and lock it
extern PBYTE GetSectorBuffered(
				PCDIO_READ This,
				DWORD SectorNumber);

// Release a locked buffer
extern void ReleaseBufferedSector(
				PCDIO_READ This,
				DWORD SectorNumber);

#ifdef __cplusplus
}
#endif

#endif // __BUFFERED_IO_H__
