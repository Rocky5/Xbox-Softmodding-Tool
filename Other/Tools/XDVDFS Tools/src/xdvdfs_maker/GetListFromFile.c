#include <stdlib.h>
#include <memory.h>
#include <xfile.h>
#include <list.h>

#include "FileEntry.h"
#include "Tools.h"

static struct stLayoutLine
{
	char DirOrFile;
	char szFileOrDirName[MAX_PATH];
	unsigned int nStartSector;
	unsigned int nFileSize;
};

// array to store list of layout file lines
static struct stLayoutLine **pLayoutLines = NULL;
static int nTotalLayoutLines = 0;
static int nLayer1StartPos = 0;

static int ExtractParam(int nParamNum, char *szSourceData, char *szDestBuffer);
static int FreeLayoutListArray();
static BOOL CheckIfDualLayer();

static void RemoveCRLF(char *Path)
{
	while( (Path[strlen(Path) - 1] == 0xA) || (Path[strlen(Path) - 1] == 0xD) )
		Path[strlen(Path) - 1] = 0;
}

BOOL GetListFromFile(
		PLIST FileAlphaList, 
		PLIST FileOrderList, 
		char *OrderFile, BOOL UseNewLayoutFormat)
{
	PFILE_ENTRY	FileEntry;
	char		Line[MAX_PATH];
	char		TmpBuf[MAX_PATH];
	FILE*		FilePointer;

	FilePointer = fopen(OrderFile, "rt");
	if (!FilePointer)
		return FALSE;

	fgets(Line, MAX_PATH, FilePointer);
	RemoveCRLF(Line);
	XFileFixSlashes(Line);

	if (UseNewLayoutFormat)
	{
		while(strlen(Line))
		{
			// make sure it not a comment line or blank line
			if (Line[0] != '#' && Line[0] != '\r' && Line[0] != '\n' && Line[0] != ' ')
			{
				// get the file or directory name
				ExtractParam(1, Line, TmpBuf);
				
				// see if its a directory line
				if (Line[0] == 'D')
				{
					FileEntry = (PFILE_ENTRY)malloc(strlen(TmpBuf) + 16 + 1);
					FileEntry->Attributes = XFILE_ATTRIBUTE_DIRECTORY;
				}
				else
				{
					FileEntry = (PFILE_ENTRY)malloc(strlen(TmpBuf) + 16 + 1);
					FileEntry->Attributes = XFILE_ATTRIBUTE_FILE;
				}

				FileEntry->SizeBytes = 0;
				FileEntry->SizeSectors = 0;
				FileEntry->StartSector = 0;
				strcpy(FileEntry->Filename, TmpBuf);

				ListAdd(FileAlphaList, (PVOID)FileEntry); 
				ListAdd(FileOrderList, (PVOID)FileEntry); 

				Line[0] = 0;
				TmpBuf[0] = 0;
				fgets(Line, MAX_PATH, FilePointer);
				RemoveCRLF(Line);
				XFileFixSlashes(Line);
			}
			else
			{
				Line[0] = 0;
				TmpBuf[0] = 0;
				fgets(Line, MAX_PATH, FilePointer);
				RemoveCRLF(Line);
				XFileFixSlashes(Line);
			}
		}
	}
	else
	{
		while(strlen(Line))
		{
			if (Line[0] != '#' && Line[0] != '\r' && Line[0] != '\n' && Line[0] != ' ')
			{
				if (Line[0] == '<')
				{
					Line[strlen(Line) - 1] = 0;
					strcpy(Line, Line + 1);

					FileEntry = (PFILE_ENTRY)malloc(strlen(Line) + 16 + 1);
					FileEntry->Attributes = XFILE_ATTRIBUTE_DIRECTORY;
				}
				else
				{
					FileEntry = (PFILE_ENTRY)malloc(strlen(Line) + 16 + 1);
					FileEntry->Attributes = XFILE_ATTRIBUTE_FILE;
				}

				FileEntry->SizeBytes = 0;
				FileEntry->SizeSectors = 0;
				FileEntry->StartSector = 0;
				strcpy(FileEntry->Filename, Line);

				ListAdd(FileAlphaList, (PVOID)FileEntry); 
				ListAdd(FileOrderList, (PVOID)FileEntry); 

				Line[0] = 0;
				fgets(Line, MAX_PATH, FilePointer);
				RemoveCRLF(Line);
				XFileFixSlashes(Line);
			}
			else
			{
				Line[0] = 0;
				fgets(Line, MAX_PATH, FilePointer);
				RemoveCRLF(Line);
				XFileFixSlashes(Line);
			}
		}
	}

	ListSort(FileAlphaList, FileListAlphaSort);

	fclose(FilePointer);

	return TRUE;
}

static int ExtractParam(int nParamNum, char *szSourceData, char *szDestBuffer)
{
	int nCount = 0;
	int nSepsFound = 0;
	char *Ptr = NULL;

	Ptr = szSourceData;

	// first we move to the start of the parameter we want
	while (*Ptr != '\0' && *Ptr != '\r' && *Ptr != '\n' && nSepsFound != nParamNum)
	{
		if (*Ptr == ',')
		{
			nSepsFound++;
		}
		
		Ptr++;
	}

	if (nSepsFound == nParamNum)
	{
		// we should now be at the start of the data we want so stick it in our buffer
		while (*Ptr != '\0' && *Ptr != '\r' && *Ptr != '\n' && *Ptr != ',')
		{
			szDestBuffer[nCount] = *Ptr;			
			nCount++;
			Ptr++;
		}
		
		szDestBuffer[nCount] = '\0';			
	}

	return 0;
}

int OptimizeLayout(BOOL optAuto, BOOL opt1, BOOL opt2, char *OrderFilename, char *OptimizedOrderFile)
{
	// here we create a copy of the layout file that was passed in and apply
	// the selected optimzation strategy to it.
	FILE *fpLayoutFile = NULL;
	FILE *fpOptimizedLayoutFile = NULL;
	int nReturnCode = 0;
	char szLineBuf[400 + 1];
	char szFileOrDirName[MAX_PATH];
	char szFileSize[20];
	char szStartSector[20];
	int nLayer0End = 0;
	int nLayer1Start = 0;
		
	memset(szLineBuf, '\0', 400 + 1);
	
	// first we read in the passed layout file and store it in a structure
	if ((fpLayoutFile = fopen(OrderFilename, "rt")) != NULL)
	{
		BOOL bDualLayer = FALSE;
		BOOL bIsOriginalDisc = TRUE;

		// create the name of the optimized layout file based upon the original
		// layout file name so you get MyLayout.lyt-Optimized
		sprintf(OptimizedOrderFile, "%s-Optimized", OrderFilename);

        if ((fpOptimizedLayoutFile = fopen(OptimizedOrderFile, "wt")) != NULL)
		{
			while (fgets(szLineBuf, 400, fpLayoutFile) != NULL)
			{
				szFileOrDirName[0] = szFileSize[0] = szStartSector[0] = '\0';

				// ignore comment lines
				if (szLineBuf[0] != '#' && szLineBuf[0] != '\r' && szLineBuf[0] != '\n' && 
					szLineBuf[0] != ' ')
				{
					// add an element to the array
					pLayoutLines = (struct stLayoutLine **)realloc(pLayoutLines, (nTotalLayoutLines + 1) * sizeof(struct stLayoutLine *));

					// allocate memory for one `struct stDumperLine`
					pLayoutLines[nTotalLayoutLines] = (struct stLayoutLine *)malloc(sizeof(struct stLayoutLine));
					
					// blank everything out
					pLayoutLines[nTotalLayoutLines]->DirOrFile = '\0';
					pLayoutLines[nTotalLayoutLines]->szFileOrDirName[0] = '\0';
					pLayoutLines[nTotalLayoutLines]->nFileSize = 0;
					pLayoutLines[nTotalLayoutLines]->nStartSector = 0;					
				
					// get type - dir or file
					pLayoutLines[nTotalLayoutLines]->DirOrFile = szLineBuf[0];
					
					// get file/dir name
					ExtractParam(1, szLineBuf, pLayoutLines[nTotalLayoutLines]->szFileOrDirName);
					
					// get file size	
					ExtractParam(2, szLineBuf, szFileSize);
					pLayoutLines[nTotalLayoutLines]->nFileSize = atoi(szFileSize);

					// get start sector
					ExtractParam(3, szLineBuf, szStartSector);
					pLayoutLines[nTotalLayoutLines]->nStartSector = atoi(szStartSector);

					if (pLayoutLines[nTotalLayoutLines]->nStartSector == 264)
					{
						// we must be creating an iso from a backup
						bIsOriginalDisc = FALSE;
					}
		
					nTotalLayoutLines++;
				}

				memset(szLineBuf, '\0', 400);
			}

			bDualLayer = CheckIfDualLayer();

			// apply the correct optimization strategy
			if ((optAuto || opt1 || opt2) && !bDualLayer)
			{
				printf("ERROR: Single layer discs cannot be automatically optimized as\n"
					   "       their layout is already presumed to be correct. You can\n"
					   "       manually tweak their layout by directly editing the layout\n"
					   "       file and running this program without any optimization flags.\n");
				
				nReturnCode = -1;
			}
			else if (opt1)
			{
				// write out all layer 1 files 1st so they are nearest to the inside of
				// of the disc << less important
				int nCurrentLine = 0;
	
				// header comment section lets em know exactly what we have just done
				fputs("# OPTMIZATION TYPE : '-opt1' All layer 1 files were assumed less important than layer 0\n"
					  "#                    files and placed nearest to the inside of the disc and kept in the\n"
					  "#                    same sequential order.\n", fpOptimizedLayoutFile);
				
				fputs("#\n# <<<<< LAYER 1 FILES START HERE >>>>>\n#\n", fpOptimizedLayoutFile);

				for(nCurrentLine = nLayer1StartPos; nCurrentLine < nTotalLayoutLines; nCurrentLine++)
				{
					fprintf(fpOptimizedLayoutFile, "%c,%s,%d,%d\n", pLayoutLines[nCurrentLine]->DirOrFile,
													pLayoutLines[nCurrentLine]->szFileOrDirName,
													pLayoutLines[nCurrentLine]->nFileSize,
													pLayoutLines[nCurrentLine]->nStartSector);
				}

				fputs("#\n# <<<<< LAYER 0 FILES START HERE >>>>>\n#\n", fpOptimizedLayoutFile);

				// now write out all layer 0 files so they are nearest to the outside of
				// of the disc I.E. >> more important so they get more speed
				for(nCurrentLine = 0; nCurrentLine < (nLayer1StartPos - 1); nCurrentLine++)
				{
					fprintf(fpOptimizedLayoutFile, "%c,%s,%d,%d\n", pLayoutLines[nCurrentLine]->DirOrFile,
													pLayoutLines[nCurrentLine]->szFileOrDirName,
													pLayoutLines[nCurrentLine]->nFileSize,
													pLayoutLines[nCurrentLine]->nStartSector);
				}
			}
			else if (opt2)
			{
				// write out all layer 0 files 1st so they are nearest to the inside of
				// of the disc << less important
				int nCurrentLine = 0;
	
				// header comment section lets em know exactly what we have just done
				fputs("# OPTMIZATION TYPE : '-opt2' All layer 1 files were assumed less important than layer 0\n"
					  "#                    files and placed nearest to the inside of the disc but their order was\n"
					  "#                    reversed in the interest of CAV gains rather than seek times\n", fpOptimizedLayoutFile);
				
				fputs("#\n# <<<<< LAYER 1 FILES START HERE >>>>>\n#\n", fpOptimizedLayoutFile);

				for(nCurrentLine = (nTotalLayoutLines - 1); nCurrentLine != (nLayer1StartPos - 1); nCurrentLine--)
				{
					fprintf(fpOptimizedLayoutFile, "%c,%s,%d,%d\n", pLayoutLines[nCurrentLine]->DirOrFile,
													pLayoutLines[nCurrentLine]->szFileOrDirName,
													pLayoutLines[nCurrentLine]->nFileSize,
													pLayoutLines[nCurrentLine]->nStartSector);
				}

				fputs("#\n# <<<<< LAYER 0 FILES START HERE >>>>>\n#\n", fpOptimizedLayoutFile);

				// now write out all layer 0 files so they are nearest to the outside of
				// of the disc I.E. >> more important so they get more speed
				for(nCurrentLine = 0; nCurrentLine < (nLayer1StartPos - 1); nCurrentLine++)
				{
					fprintf(fpOptimizedLayoutFile, "%c,%s,%d,%d\n", pLayoutLines[nCurrentLine]->DirOrFile,
													pLayoutLines[nCurrentLine]->szFileOrDirName,
													pLayoutLines[nCurrentLine]->nFileSize,
													pLayoutLines[nCurrentLine]->nStartSector);
				}
			}

			fclose(fpOptimizedLayoutFile);
		}
		else
		{
			printf("ERROR: Couldnt open input layout file '%s'\n", OrderFilename);
			nReturnCode = -1;
		}

		fclose(fpLayoutFile);
	}
	else
	{
		printf("ERROR: Couldnt open input layout file '%s'\n", OrderFilename);
		nReturnCode = -1;
	}


	FreeLayoutListArray();

	return (nReturnCode);
}

static int FreeLayoutListArray()
{
	int nCurrentLine = 0;
	
	for(nCurrentLine = 0; nCurrentLine < nTotalLayoutLines; nCurrentLine++)
	{
		free(pLayoutLines[nCurrentLine]);
	}

	free(pLayoutLines);
	
	pLayoutLines = NULL;
	return 0;
}

static BOOL CheckIfDualLayer()
{
	BOOL bReturnCode = FALSE;
	int nCurrentLine = 0;
	
	for(nCurrentLine = 0; nCurrentLine < nTotalLayoutLines; nCurrentLine++)
	{
		// see if the current files start sector is in the layer 1 range
		if (pLayoutLines[nCurrentLine]->nStartSector > 1715631)
		{
			bReturnCode = TRUE;
			nLayer1StartPos = nCurrentLine;
			break;
		}
	}

	return (bReturnCode);
}