#include <stdlib.h>
#include <memory.h>

#include "BuildDirectoryTree.h"
#include "ListEnumFiles.h"

static void BuildTree(PDIRENTRY_INFO Table, DWORD NbElements, DWORD Element)
{
	int		i;
	DWORD	Interval;
	DWORD	NewElementLeft = 0xFFFFFFFF;
	DWORD	NewElementRight = 0xFFFFFFFF;

	// Mark the element as 'taken'
	Table[Element].Left = 0xFFFFFFFF;
	Table[Element].Right = 0xFFFFFFFF;
	Table[Element].Flag = 1;

	// If we're not at the start of the table and the element immediately
	// to the left is not 'taken'
	if ((Element > 0) && (!Table[Element - 1].Flag))
	{
		Interval = 0;

		// Scan 'to the left'
		for(i = Element - 1;i >= 0;i--)
		{
			if (Table[i].Flag)
				break;

			Interval++;
		}

		// Find the element at the middle of the interval
		NewElementLeft = Element - (Interval / 2) - 1;

		// Mark it to the left of the current element...
		Table[Element].Left = NewElementLeft;
	}

	// If we're not at the end of the table and the element immediately
	// to the right is not 'taken'
	if ((Element < NbElements - 1) && (!Table[Element + 1].Flag))
	{
		Interval = 0;

		// Scan 'to the right'
		for(i = Element + 1;i < NbElements;i++)
		{
			if (Table[i].Flag)
				break;

			Interval++;
		}

		// Find the element at the middle of the interval
		NewElementRight = Element + ((Interval - 1) / 2) + 1;

		// Mark it to the right of the current element...
		Table[Element].Right = NewElementRight;
	}

	// Recurse to the element on the left, if needed
	if (NewElementLeft != 0xFFFFFFFF)
	{
		BuildTree(Table, NbElements, NewElementLeft);
	}

	// Recurse to the element on the right, if needed
	if (NewElementRight != 0xFFFFFFFF)
	{
		BuildTree(Table, NbElements, NewElementRight);
	}
}

static int FlattenTree(PDIRENTRY_INFO Table, int Element)
{
	int	LastElement = Element;

	if (Table[Element].Left != 0xFFFFFFFF)
	{
		Table[LastElement].Next = &Table[Table[Element].Left];
		LastElement = FlattenTree(Table, Table[Element].Left);
	}

	if (Table[Element].Right != 0xFFFFFFFF)
	{
		Table[LastElement].Next = &Table[Table[Element].Right];
		LastElement = FlattenTree(Table, Table[Element].Right);
	}

	return LastElement;
}

void BuildDirectoryTree(
		PLIST FileList,
		PFILE_ENTRY Directory,
		PDIRENTRY_INFO *OutDirentryInfo,
		DWORD *OutNbEntries,
		PDIRENTRY_INFO *OutHead)
{
	ENUM_RECORD		EnumRecord;
	PFILE_ENTRY		FileEntry;
	PDIRENTRY_INFO	DirentryInfo;
	PDIRENTRY_INFO	Head;
	DWORD			NbEntries = 0;
	DWORD			CurrentEntry; 

	*OutNbEntries = 0;

	if (ListFindFirstFile(
			&EnumRecord,
			FileList,
			Directory->Filename,
			&FileEntry))
	{
		// Start by counting elements
		do
		{
			NbEntries++;
		} while(ListFindNextFile(&EnumRecord, &FileEntry));

		// Allocate space to store the DIRENTRY_INFOs
		DirentryInfo = (PDIRENTRY_INFO)calloc(NbEntries, sizeof(DIRENTRY_INFO));

		*OutNbEntries = NbEntries;
		*OutDirentryInfo = DirentryInfo;

		// Rewind the enum record
		EnumRecord.Index = 0;

		// Create a list with all the elements
		CurrentEntry = 0;
		while(ListFindNextFile(&EnumRecord, &FileEntry))
		{
			DirentryInfo[CurrentEntry].Entry = FileEntry;
			DirentryInfo[CurrentEntry].Next = NULL;
			CurrentEntry++;
		}
 
		if (NbEntries)
		{
			// Build the search tree
			BuildTree(DirentryInfo, NbEntries, (NbEntries - 1) / 2);

			// Reorganize the elements according to the XDVDFS 'restrictions'
			// This is needed to obtain a disk readable on XBox
			Head = &DirentryInfo[(NbEntries - 1) / 2];
			FlattenTree(DirentryInfo, (NbEntries - 1) / 2);
			*OutHead = Head;
		}
		else
			*OutHead = NULL;

		ListFindClose(&EnumRecord);
	}
}
