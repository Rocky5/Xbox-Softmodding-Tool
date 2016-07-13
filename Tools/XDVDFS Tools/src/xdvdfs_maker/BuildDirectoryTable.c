#include <stdlib.h>
#include <memory.h>
#include <xfile.h>

#include "BuildDirectoryTree.h"
#include "BuildDirectoryTable.h"
#include "ListEnumFiles.h"
#include "Endian.h"
#include "Tools.h"

// Build the directory tree and create the corresponding directory table
void BuildDirectoryTable(
		PLIST FileList,
		PFILE_ENTRY Directory,
		PBYTE Buffer)
{
	PDIRENTRY_INFO	DirentryInfo;
	PDIRENTRY_INFO	Element;
	PDIRENTRY_INFO	Head;
	PXDVDFS_ENTRY	XDVDFSEntry;
	DWORD			EntrySize;
	DWORD			NbEntries = 0;
	DWORD			CurrentOffset = 0;
	DWORD			i;
	LPSTR			Filename;

	// Fill the space occupied by the directory table with 0xFFs
	memset(Buffer, 0xFF, Directory->SizeSectors * 2048);

	// Build the directory tree
	BuildDirectoryTree(FileList, Directory, &DirentryInfo, &NbEntries, &Head);

	// If the directory is not empty
	if (NbEntries)
	{
		Element = Head;
		while(Element)
		{
			// Get the size of the entry
			Filename = XFileGetFilename(Element->Entry->Filename);
			EntrySize = MakeMultipleOf(14 + strlen(Filename), 4);

			// Check if the entry fits in the current sector, if not advance to
			// the next sector
			if ((CurrentOffset > 0)&&
					((MakeMultipleOf(CurrentOffset, 2048) - CurrentOffset)
					 < EntrySize))
					CurrentOffset = MakeMultipleOf(CurrentOffset, 2048);

			// Get a pointer to the XDVDFS_ENTRY in the buffer and store its
			// position
			XDVDFSEntry = (PXDVDFS_ENTRY)(Buffer + CurrentOffset);
			Element->Offset = CurrentOffset / 4;

			// Fill the entry
			XDVDFSEntry->LeftOffset = INTEL_WORD(0);
			XDVDFSEntry->RightOffset = INTEL_WORD(0);
			XDVDFSEntry->StartSector = INTEL_DWORD(Element->Entry->StartSector);
			XDVDFSEntry->TotalSize = INTEL_DWORD(Element->Entry->SizeBytes);

			if (Element->Entry->Attributes == XFILE_ATTRIBUTE_DIRECTORY)
				XDVDFSEntry->Attributes = 0x30;
			else
				XDVDFSEntry->Attributes = 0x20;

			XDVDFSEntry->FilenameLength = strlen(Filename);
			memcpy(
				XDVDFSEntry->Filename,
				Filename,
				XDVDFSEntry->FilenameLength);

			// Get the next entry
			CurrentOffset += EntrySize;
			Element = Element->Next;
		}

		// Fill the Left and Right fields in all the XDVDFS_ENTRY structures,
		// now that we know their position in the directory table
		for(i = 0;i < NbEntries;i++)
		{
			XDVDFSEntry = (PXDVDFS_ENTRY)(Buffer + DirentryInfo[i].Offset * 4);

			if (DirentryInfo[i].Left != 0xFFFFFFFF)
			{
				XDVDFSEntry->LeftOffset = INTEL_WORD(
					(WORD)(DirentryInfo[DirentryInfo[i].Left].Offset));
			}

			if (DirentryInfo[i].Right != 0xFFFFFFFF)
			{
				XDVDFSEntry->RightOffset = INTEL_WORD(
					(WORD)(DirentryInfo[DirentryInfo[i].Right].Offset));
			}
		}

		// Free the memory allocated by BuildDirectoryTree
		free(DirentryInfo);
	}
}
