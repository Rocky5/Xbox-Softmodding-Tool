#include "CalcFilePlacement.h"
#include "FileEntry.h"
#include "Tools.h"

#define DVD_START_SECTOR	264
#define DVD_CAPACITY		2290000
//#define DVD_CAPACITY		4580000 // uncomment for very primitive dual layer support

BOOL CalcFilePlacement(PLIST FileList, BOOL	Padding, PDWORD ImageTotalSize, DWORD OEIBuffer)
{
	PFILE_ENTRY	FileEntry;
	DWORD		CurrentSector;
	DWORD		TotalDataSize = 0;
	DWORD		i;
	DWORD       dwGapToLeaveNearEdge = 0;

	// calculate what the MB specified in OEIBuffer is in actual sectors
	if (OEIBuffer > 0)
	{
		dwGapToLeaveNearEdge = MakeMultipleOf(((OEIBuffer * 1024) * 1024), 2048) / 2048;
	}

	for(i = 0;i < FileList->Count;i++)
	{
		FileEntry = (PFILE_ENTRY)FileList->Pointers[i];
		TotalDataSize += FileEntry->SizeSectors;
	}

	if (TotalDataSize > ((DVD_CAPACITY - dwGapToLeaveNearEdge) - DVD_START_SECTOR))
		return FALSE;

	if (!Padding)
		CurrentSector = DVD_START_SECTOR;
	else
		CurrentSector = (DVD_CAPACITY - dwGapToLeaveNearEdge) - TotalDataSize;

	*ImageTotalSize = MakeMultipleOf(CurrentSector + TotalDataSize, 32);

	for(i = 0;i < FileList->Count;i++)
	{
		FileEntry = (PFILE_ENTRY)FileList->Pointers[i];

		if (FileEntry->SizeBytes)
			FileEntry->StartSector = CurrentSector;

		CurrentSector += FileEntry->SizeSectors;
	}

	return TRUE;
}
