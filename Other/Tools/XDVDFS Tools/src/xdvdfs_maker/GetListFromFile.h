#ifndef __GETLISTFROMFILE_H__
#define __GETLISTFROMFILE_H__

#include <list.h>

extern BOOL GetListFromFile(
		PLIST FileAlphaList, 
		PLIST FileOrderList, 
		char *OrderFile,
		BOOL UseNewLayoutFormat);

int OptimizeLayout(BOOL optAuto, BOOL opt1, BOOL opt2, char *OrderFilename, char *OptimizedOrderFile);

#endif

