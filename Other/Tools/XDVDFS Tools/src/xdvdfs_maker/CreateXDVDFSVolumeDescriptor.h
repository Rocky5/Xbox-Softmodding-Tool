#ifndef __CREATEXDVDFSVOLUMEDESCRIPTOR_H__
#define __CREATEXDVDFSVOLUMEDESCRIPTOR_H__

#include <stdtypes.h>

extern void CreateXDVDFSVolumeDescriptor(
				DWORD RootStartSector, 
				DWORD RootSize, 
				PBYTE Buffer);

#endif
