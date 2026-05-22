#define IOCTL_VIRTUAL_CDROM_DETACH		0x1EE7CD02
#define IOCTL_VIRTUAL_DETACH			0xCE52B02

#define OBJ_CASE_INSENSITIVE			0x00000040L
#define FILE_SYNCHRONOUS_IO_NONALERT	0x00000020
#define FILE_NON_DIRECTORY_FILE			0x00000040

#define NT_SUCCESS(StatCode)  ((NTSTATUS)(StatCode) >= 0)

extern "C" {
	typedef ULONG NTSTATUS;
	typedef ULONG ACCESS_MASK;

	typedef struct _OBJECT_ATTRIBUTES
	{
			HANDLE RootDirectory;
			PANSI_STRING ObjectName;
			ULONG Attributes;
	} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

	typedef struct _IO_STATUS_BLOCK
	{
			NTSTATUS Status;
			ULONG Information;
	} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
	
	typedef VOID (*PIO_APC_ROUTINE)(
			PVOID ApcContext,
			PIO_STATUS_BLOCK pIoStatusBlock,
			ULONG Reserved
	);
	
	extern NTSTATUS __stdcall NtOpenFile(
		OUT PHANDLE pHandle,
		IN ACCESS_MASK DesiredAccess,
		IN POBJECT_ATTRIBUTES pObjectAttributes,
		OUT PIO_STATUS_BLOCK pIOStatusBlock,
		IN ULONG ShareAccess,
		IN ULONG OpenOptions
	);
	
	extern NTSTATUS __stdcall NtClose(HANDLE Handle);
	extern VOID __stdcall HalReturnToFirmware(unsigned int);
	extern VOID __stdcall RtlInitAnsiString(void *destp, void *srcp);
	extern NTSTATUS __stdcall IoDismountVolumeByName(PANSI_STRING DeviceName);
	
	extern NTSTATUS __stdcall IoDismountVolumeByName(
		PANSI_STRING DeviceName
	);
	
	/* Xbox Kernel Functions */
	extern NTSTATUS __stdcall NtDeviceIoControlFile(
		HANDLE Handle,
		HANDLE Event,
		PIO_APC_ROUTINE ApcRoutine,
		PVOID ApcContext,
		PIO_STATUS_BLOCK IoStatusBlock,
		ULONG IoControlCode,
		PVOID InputBuffer,
		ULONG InputBufferLength,
		PVOID OutputBuffer,
		ULONG OutputBufferLength
	);

};