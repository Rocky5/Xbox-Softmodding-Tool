#include <memory.h>

#include "CreateXDVDFSVolumeDescriptor.h"
#include "Endian.h"

const char *XDVDFSIdentifier = "MICROSOFT*XBOX*MEDIA";

void CreateXDVDFSVolumeDescriptor(DWORD RootStartSector, DWORD RootSize, PBYTE Buffer)
{
	memset(Buffer, 0, 2048);

	memcpy(Buffer, XDVDFSIdentifier, strlen(XDVDFSIdentifier));
	memcpy(Buffer + 0x7ec, XDVDFSIdentifier, strlen(XDVDFSIdentifier));

	*((PDWORD)(Buffer + 0x014)) = INTEL_DWORD(RootStartSector);
	*((PDWORD)(Buffer + 0x018)) = INTEL_DWORD(RootSize);
}
