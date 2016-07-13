#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <list.h>
#include <xfile.h>

#include "FileEntry.h"
#include "GetListFromDirectory.h"
#include "GetListFromFile.h"
#include "EvaluateFileSizes.h"
#include "CalcFilePlacement.h"
#include "CreateISOVolumeDescriptors.h"
#include "CreateXDVDFSVolumeDescriptor.h"
#include "BuildDirectoryTable.h"
#include "Tools.h"


static int GenerateOrderFileFromList(PLIST FileOrderList, char *CreatedLayoutFile);

// Transfer Buffer, the larger the better
// Size must be a multiple of 2048 (FILE_FLAG_NO_BUFFERING on Windows)
BYTE	TransferBuffer[1 * 1048576];

// Copy a file into the image and pad size to a multiple of 2048 as needed
BOOL IncludeFile(
		PXFILE OutFile, 
		char *Filename, 
		char *RootDir, 
		DWORD FileSize,
		PBYTE Buffer,
		DWORD BufferSize)
{
	XFILE	InFile;
	PBYTE	CompleteFilename;
	DWORD	Remainder;

	// Start by getting the full filename (RootDir + Filename)
	CompleteFilename = (PBYTE)malloc(strlen(Filename) + strlen(RootDir) + 1);
	strcpy(CompleteFilename, RootDir);
	strcat(CompleteFilename, Filename);

	// Open the file
	if (!XFileOpen(&InFile, CompleteFilename, XFILE_READ))
	{
		printf("%s: Can't open file, aborting...\n", CompleteFilename);
		free(CompleteFilename);
		return FALSE;
	}

	// Copy file data into the image
	Remainder = FileSize;
	while(Remainder > 0)
	{
#ifdef _WIN32
		// Under Windows we must make sure each read operation is for a size that is a
		// multiple of a sector (see FILE_FLAG_NO_BUFFERING)
		XFileRead(&InFile, Buffer, MakeMultipleOf(min(Remainder, BufferSize), 2048));
#else
		XFileRead(&InFile, Buffer, min(Remainder, BufferSize));
#endif

		XFileWrite(OutFile, Buffer, min(Remainder, BufferSize));
		Remainder -= min(Remainder, BufferSize);

		DisplayGauge(FileSize - Remainder, FileSize);
	}

	// Pad to the next sector if needed
	if (FileSize % 2048)
	{
		Remainder = 2048 - (FileSize % 2048);
		memset(Buffer, 0, Remainder);
		XFileWrite(OutFile, Buffer, Remainder);
	}

	// Clear the progression gauge, close the file and release the memory
	// allocated for the complete filename
	ClearGauge();
	XFileClose(&InFile);
	free(CompleteFilename);

	return TRUE;
}

void Usage(void)
{
	fprintf(stderr, "\nXDVDFS_Maker v2.0 (by CloneXB)\n\nUSAGE: xdvdfs_maker [Options] [Root Directory] [Image File]"
					"\n\nOptions:\n"
					"-h\n   Displays this text.\n\n"
#ifdef _WIN32
					"-pad\n   Will pad the image to 4.38Gb. (NTFS REQUIRED)\n\n"
#else
					"-pad\n   Will pad the image to 4.38Gb.\n\n"
#endif
					"-OEIBuf [n]\n   Will leave a gap of [n] Mega Bytes near the outside edge of the disc when\n"
		                   "   padding an image to 4.38 GB. This option can be used to avoid writing to areas\n"
						   "   of imperfections sometimes found near the outside of cheaper discs.\n\n"

					"-order [Filename]\n   Will use the file order layout specified in the text file.\n\n"

					"-ordercreate [Filename]\n   Will auto generate a file order layout file that can be manually\n"
						                     "   optimized later.\n\n"

					"-opt1\n   Must be used in combination with -newlayoutformat. Optimizes the layout\n"
					       "   order file further by assuming that files from layer 1 are less important\n"
						   "   than files on layer 0 and therefore moves all layer 1 files to the start of\n"
						   "   the disc where dvd read speed is slower than the outside of the disc.\n\n"

   					"-opt2\n   Must be used in combination with -newlayoutformat. Optimizes the layout\n"
					       "   order file further by assuming that files from layer 1 are less important\n"
						   "   than files on layer 0 and therefore moves all layer 1 files to the start of\n"
						   "   the disc but reverses their order in the interest of read speed gains over\n"
						   "   seek times.\n\n"


					       "NOTE: Comment lines in the layout file, which begin with a #, will be ignored.\n"
						   "      Blank lines will also be ignored.\n"


#ifdef _WIN32
					"\nNote: If you're creating an image on a FAT32 volume, be careful that the image\n"
					"size do not exceed 2Gb. No check is done.\n"
#endif
					"\n\n");
}

int main(int argc, char *argv[])
{
	// List representing the order of files on the DVD
	LIST		FileOrderList;
	// Alphabetically sorted list to generate directory structure
	LIST		FileAlphaList;
	// Total size of the DVD, in sectors
	DWORD		ImageTotalSize;
	// Our image file
	XFILE		ImageFile;
	// A pointer of FILE_ENTRY to save some indirections
	PFILE_ENTRY	FileEntry;
	// Current sector when writing the image
	DWORD		CurrentSector;
	// All purpose counter... ;)
	DWORD		i;
	// Outer Edge Integrity Buffer
	DWORD OEIBuffer = 0;
	// The filename of our XDVDFS image
	char		*ImageFilename = NULL;
	// Directory where we will find the files
	char		*RootDirectory = NULL;
	// Optional file specifying the order of files on the DVD
	char		*OrderFilename = NULL;
	// Optional file generated containing the order of files on the DVD	
	char		*CreatedLayoutFile = NULL;
	// If true, the image will be padded to 4.38Gb
	DWORD		Padding = FALSE;

	BOOL        NewLayoutFormat = TRUE;
	BOOL		optAuto = FALSE;
	BOOL		opt1 = FALSE;
	BOOL		opt2 = FALSE;
	int         nOptimizationsRequired = 0;

	char OptimizedOrderFile[MAX_PATH];

	// Check for sufficient number of arguments
	if (argc < 2)
	{
		Usage();
		return 0;
	}
	
	// Parse command line options
	for(i = 1;i < (DWORD)argc;i++)
	{
		if (argv[i][0] == '-')
		{
			if (!stricmp(argv[i], "-h"))
			{
				Usage();
				return 0;
			}
			else if (!stricmp(argv[i], "-pad"))
			{
				Padding = TRUE;
			}
			else if (!stricmp(argv[i], "-optAuto"))
			{
				optAuto = TRUE;
				nOptimizationsRequired++;
			}
			else if (!stricmp(argv[i], "-opt1"))
			{
				opt1 = TRUE;
				nOptimizationsRequired++;
			}
			else if (!stricmp(argv[i], "-opt2"))
			{
				opt2 = TRUE;
				nOptimizationsRequired++;
			}
			else if (!stricmp(argv[i], "-order"))
			{
				i++;
				OrderFilename = argv[i];
			}
			else if (!stricmp(argv[i], "-ordercreate"))
			{
				i++;
				CreatedLayoutFile = argv[i];
			}
			else if (!stricmp(argv[i], "-OEIBuf"))
			{
				i++;
				OEIBuffer = atoi(argv[i]);
			}
			else
				printf("%s: Unknown option.\n", argv[i]);
		}
		else
		{
			if (!RootDirectory)
				RootDirectory = argv[i];
			else if (!ImageFilename)
				ImageFilename = argv[i];
			else
				printf("%s: Ignoring extra parameter.\n", argv[i]);
		}
	}

	if (OrderFilename && CreatedLayoutFile)
	{
		printf("ERROR: You cannot use -order and -ordercreate options together, you\n"
			   "       can either have one or the other.\n");
		return 0;
	}

	OptimizedOrderFile[0] = '\0';

	// Check if we were given enough information to do our work
	if ((!RootDirectory)||(!ImageFilename))
	{
		printf("You MUST provide a root directory and an image filename.\n");
		return -1;
	}

	// make sure they only passed in 1 or less optimization parameters
	if (nOptimizationsRequired > 1)
	{
		printf("You can only select ONE optimization method.\n");
		return -1;
	}

	XFileRemoveTrailingSlash(RootDirectory);

	// Create our lists. One representing the file order on the disk,
	// the other all files in alphabetical order.
	ListCreate(&FileAlphaList, 16384, 16384);
	ListCreate(&FileOrderList, 16384, 16384);

	if (OrderFilename)
	{
		// optimize the layout file based upon the user selected optmization strategy
		// and give them the opportunity to manually tweak the layout
		if (optAuto || opt1 || opt2)
		{
			printf("Optimizing Layout....\n");

			if (OptimizeLayout(optAuto, opt1, opt2, OrderFilename, OptimizedOrderFile) == -1)
			{
				return -1;
			}
			else
			{
				printf("An optimized version of your layout file '%s' has been created as "
					   "'%s'. If you would like to preview the optimized layout file or "
					   "manually tweak it then go and do so now. Once you are satisfied with "
					   "the layout....\n\n"
					   ">>>>> Press ENTER to continue <<<<<\n", OrderFilename, OptimizedOrderFile);
				
				// wait for them to press a key
				getchar();

				// we now want to use our new optimized layout file
				OrderFilename = OptimizedOrderFile;
			}
		}
		
		// Get the list of files to include in the image from the file order file
		if (!GetListFromFile(&FileAlphaList, &FileOrderList, OrderFilename, NewLayoutFormat))
		{
			printf("%s: Error while opening / parsing order file !\n", OrderFilename);
			ListDestroy(&FileOrderList, FALSE);
			ListDestroy(&FileAlphaList, TRUE);
			return -1;
		}
	}
	else
	{
		// Get the list of files to include in the image from the root directory
		if (!GetListFromDirectory(&FileAlphaList, &FileOrderList, RootDirectory))
		{
			printf("%s: Can't open directory, or directory empty !\n", RootDirectory);
			ListDestroy(&FileOrderList, FALSE);
			ListDestroy(&FileAlphaList, TRUE);
			return -1;
		}
		else if (CreatedLayoutFile)
		{
			if (GenerateOrderFileFromList(&FileOrderList, CreatedLayoutFile) != -1)
			{
				printf("The layout file '%s' has been automatically generated ready for you\n"
					   "to manually optimize. If you would like to manually optimize this layout\n"
					   "then do so now. Once you are satisfied with  the layout....\n\n"
					   ">>>>> Press ENTER to continue <<<<<\n", CreatedLayoutFile);
				
				// wait for them to press a key
				getchar();

				// we now want to use our new optimized layout file
				OrderFilename = CreatedLayoutFile;

				// now we re-create the order lists based on the layout file
				ListDestroy(&FileOrderList, FALSE);
				ListDestroy(&FileAlphaList, TRUE);

				ListCreate(&FileAlphaList, 16384, 16384);
				ListCreate(&FileOrderList, 16384, 16384);

				// Get the list of files to include in the image from the file order file
				if (!GetListFromFile(&FileAlphaList, &FileOrderList, OrderFilename, NewLayoutFormat))
				{
					printf("%s: Error while opening / parsing order file !\n", OrderFilename);
					ListDestroy(&FileOrderList, FALSE);
					ListDestroy(&FileAlphaList, TRUE);
					return -1;
				}
			}
			else
			{
				return 0;
			}
		}
	}

	// Get the file size of all files in the list. Directory record sizes are computed, too.
	puts("Evaluating file sizes...");
	if (!EvaluateFileSizes(&FileAlphaList, RootDirectory))
	{
		ListDestroy(&FileOrderList, FALSE);
		ListDestroy(&FileAlphaList, TRUE);
		return -1;
	}

	// Now compute the placement of all files on the disk
	puts("Computing files placement on the disk...");
	if (!CalcFilePlacement(&FileOrderList, Padding, &ImageTotalSize, OEIBuffer))
	{
		puts("Data won't fit on a DVD-R...");
		ListDestroy(&FileOrderList, FALSE);
		ListDestroy(&FileAlphaList, TRUE);
		return -1;
	}

	// Open / Create our image file
	if (!XFileOpen(&ImageFile, ImageFilename, XFILE_WRITE))
	{
		printf("%s: Can't open image file !\n", ImageFilename);
		ListDestroy(&FileOrderList, FALSE);
		ListDestroy(&FileAlphaList, TRUE);
		return -1;
	}

	// Generate a blank ISO9660 PVD only the volume size is filled
	puts("Generating system area...");
	WriteZeroedSectors(&ImageFile, 16, TransferBuffer, sizeof(TransferBuffer), FALSE);
	CreateISOVolumeDescriptors(ImageTotalSize, TransferBuffer);
	XFileWrite(&ImageFile, TransferBuffer, 2 * 2048);

	// Generate a XDVDFS PVD. I've omitted the creation time. Not too useful anyway...

	//FileEntry = (PFILE_ENTRY)FileOrderList.Pointers[0];
	FileEntry = (PFILE_ENTRY)FileAlphaList.Pointers[0];
	WriteZeroedSectors(&ImageFile, 14, TransferBuffer, sizeof(TransferBuffer), FALSE);
	CreateXDVDFSVolumeDescriptor(FileEntry->StartSector, FileEntry->SizeBytes, TransferBuffer);
	XFileWrite(&ImageFile, TransferBuffer, 2048);

	// Pad the image to the start of the data area
	puts("Writing pad data...");
	CurrentSector = 33;

	// CloneXB bugfix for putting / in other places
	if (FileOrderList.Count && OrderFilename)
	{
		FileEntry = (PFILE_ENTRY)FileOrderList.Pointers[0];
	}
	
	WriteZeroedSectors(
		&ImageFile, 
		FileEntry->StartSector - CurrentSector, 
		TransferBuffer, 
		sizeof(TransferBuffer), 
		TRUE);

	// CloneXB bugfix for putting / in other places
	if (FileOrderList.Count && OrderFilename)
	{
		FileEntry = (PFILE_ENTRY)FileAlphaList.Pointers[0];
	}

	CurrentSector = FileEntry->StartSector;
	
	// Write all files and directory records
	for(i = 0;i < FileOrderList.Count;i++)
	{
		FileEntry = (PFILE_ENTRY)FileOrderList.Pointers[i];

		if (FileEntry->Attributes == XFILE_ATTRIBUTE_DIRECTORY)
		{
			printf("<%s>\n", FileEntry->Filename);

			if (FileEntry->SizeBytes)
			{
				BuildDirectoryTable(&FileAlphaList, FileEntry, TransferBuffer);
				XFileWrite(&ImageFile, TransferBuffer, FileEntry->SizeSectors * 2048);
				CurrentSector += FileEntry->SizeSectors;
			}
		}
		else if (FileEntry->SizeBytes)
		{
			puts(FileEntry->Filename);

			if (!IncludeFile(
				&ImageFile, 
				FileEntry->Filename, 
				RootDirectory, 
				FileEntry->SizeBytes,
				TransferBuffer,
				sizeof(TransferBuffer)))
			{
				XFileClose(&ImageFile);
				ListDestroy(&FileOrderList, FALSE);
				ListDestroy(&FileAlphaList, TRUE);

				return -1;
			}

			CurrentSector += FileEntry->SizeSectors;
		}
	}

	// A XDVDFS disk must have a size in sectors that is a multiple of 32.
	// This is a requirement of the filesystem
	puts("Making image size a multiple of 32 sectors...");
	if (CurrentSector % 32)
		WriteZeroedSectors(&ImageFile, 32 - (CurrentSector % 32), TransferBuffer, sizeof(TransferBuffer), FALSE);
	
	// Close the image...
	XFileClose(&ImageFile);

	// Destroy the lists. Only one cleanup is needed because one
	// of the lists is a copy of the other
	ListDestroy(&FileOrderList, FALSE);
	ListDestroy(&FileAlphaList, TRUE);

	// We're done... :-)
	puts("Done...");
	
	return 0;
}


static int GenerateOrderFileFromList(PLIST FileOrderList, char *CreatedLayoutFile)
{
	int nReturnCode = 0;
	FILE *fpGeneratedFile = NULL;
	int i = 0;
	PFILE_ENTRY	tmpFileEntry = NULL;

	if ((fpGeneratedFile = fopen(CreatedLayoutFile, "wt")) != NULL)
	{
		fputs("# OPTMIZATION TYPE : No Optimization has been applied. This file was auto generated based\n"
			  "#                    on the supplied root directory structure. The purpose of this file is\n"
			  "#                    to provide a base layout order file which can be manually optimized by\n"
			  "#                    the user.\n#\n# <<<<< LAYER 0 FILES START HERE >>>>>\n#\n", fpGeneratedFile);

		
		// Write all files and directory records
		for(i = 0;i < FileOrderList->Count;i++)
		{
			tmpFileEntry = (PFILE_ENTRY)FileOrderList->Pointers[i];

			fprintf(fpGeneratedFile, "%c,%s,%d,%d\n", (tmpFileEntry->Attributes == XFILE_ATTRIBUTE_DIRECTORY) ? 'D' : 'F',
														tmpFileEntry->Filename,
														tmpFileEntry->StartSector,
														tmpFileEntry->SizeBytes);
		}

		fclose(fpGeneratedFile);
	}
	else
	{
		printf("ERROR: Failed to create generated layout file '%s'\n",CreatedLayoutFile); 
		nReturnCode = -1;
	}

	return (nReturnCode);
}