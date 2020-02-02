#include <xtl.h>

extern "C" {
#define STATUS_SUCCESS 0
#define STATUS_UNSUCCESSFUL -1

typedef ULONG ACCESS_MASK;
typedef struct tagANSI_STRING {
 unsigned short Length;
 unsigned short MaximumLength;
 PCHAR Buffer;
} ANSI_STRING;

typedef struct tagOBJECT_ATTRIBUTES {
 HANDLE RootDirectory;
 ANSI_STRING *ObjectName;
 ULONG Attributes;
} OBJECT_ATTRIBUTES;
#define OBJ_CASE_INSENSITIVE 0x00000040

typedef struct tagIO_STATUS_BLOCK {
 union {
  unsigned int Status;
  PVOID Pointer;
 } u1;
 ULONG_PTR Information;
} IO_STATUS_BLOCK;

extern unsigned int WINAPI NtDeviceIoControlFile(HANDLE handle, HANDLE event, PVOID pApcRoutine, PVOID pApcContext, IO_STATUS_BLOCK *pIoStatusBlock, ULONG IoControlCode, PVOID pInBuf, ULONG InBufLen, PVOID pOutBuf, ULONG OutBufLen);

extern unsigned int WINAPI NtCreateFile(HANDLE *pHandle, ACCESS_MASK DesiredAccess, OBJECT_ATTRIBUTES *pObjectAttributes, IO_STATUS_BLOCK *pIOStatusBlock, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions);
extern unsigned int WINAPI NtOpenFile(HANDLE *pHandle, ACCESS_MASK DesiredAccess, OBJECT_ATTRIBUTES *pObjectAttributes, IO_STATUS_BLOCK *pIOStatusBlock, ULONG ShareAccess, ULONG OpenOptions);
extern unsigned int WINAPI NtReadFile(HANDLE Handle, HANDLE Event, PVOID pApcRoutine, PVOID pApcContext, PVOID pIoStatusBlock, PVOID pBuffer, ULONG Length, PLARGE_INTEGER pByteOffset);
extern unsigned int WINAPI NtWriteFile(HANDLE Handle, HANDLE Event, PVOID pApcRoutine, PVOID pApcContext, PVOID pIoStatusBlock, PVOID pBuffer, ULONG Length, PLARGE_INTEGER pByteOffset);
extern unsigned int WINAPI NtClose(HANDLE Handle);

extern unsigned int WINAPI MmMapIoSpace(unsigned int, unsigned int, unsigned int);
extern void WINAPI MmUnmapIoSpace(unsigned int, unsigned int);
extern void WINAPI HalInitiateShutdown(void);
extern void WINAPI HalWriteSMBusValue(unsigned int, unsigned int, unsigned int, unsigned int);
extern void *XboxHDKey;
extern void *LaunchDataPage;
extern void WINAPI HalReturnToFirmware(unsigned int);
extern void WINAPI RtlInitAnsiString(void *destp, void *srcp);
}


#define     MAX_PARTITIONS     14

typedef struct tagPARTITION_ENTRY {
char pe_name[16];
unsigned long pe_flags; // bitmask
unsigned long pe_lba_start;
unsigned long pe_lba_size;
unsigned long pe_reserved;
} PARTITION_ENTRY;


// This flag (part of PARTITION_ENTRY.pe_flags) tells you whether/not a
// partition is being used (whether/not drive G is active, for example)
#define PE_PARTFLAGS_IN_USE 0x80000000

typedef struct tagPARTITION_TABLE {
char pt_magic[16];
unsigned char pt_reserved[32];
PARTITION_ENTRY pt_entries[MAX_PARTITIONS];
} PARTITION_TABLE;


#define IOCTL_CMD_LBA48_ACCESS 0xcafebabe
#define IOCTL_SUBCMD_GET_INFO 0

#define LBA48_GET_INFO_MAGIC1_IDX 0
#define LBA48_GET_INFO_MAGIC1_VAL 0xcafebabe
#define LBA48_GET_INFO_MAGIC2_IDX 1
#define LBA48_GET_INFO_MAGIC2_VAL 0xbabeface
#define LBA48_GET_INFO_PATCHCODE_VERSION_IDX 2
#define LBA48_GET_INFO_LOWCODE_BASE_IDX  3
#define LBA48_GET_INFO_HIGHCODE_BASE_IDX 4
#define LBA48_GET_INFO_PATCHSEG_SIZE_IDX 5
#define LBA48_GET_INFO_PART_TABLE_OFS_IDX 6

unsigned int
read_active_partition_table(PARTITION_TABLE *p_table)
{
ANSI_STRING a_file;
OBJECT_ATTRIBUTES obj_attr;
IO_STATUS_BLOCK io_stat_block;
HANDLE handle;
unsigned int stat;
unsigned int ioctl_cmd_in_buf[100];
unsigned int ioctl_cmd_out_buf[100];
unsigned int partition_table_addr;

OutputDebugString("*******************************\n");
return STATUS_UNSUCCESSFUL;

memset(p_table, 0, sizeof(PARTITION_TABLE));

RtlInitAnsiString(&a_file, "\\Device\\Harddisk0\\partition0");
obj_attr.RootDirectory = 0;
obj_attr.ObjectName = &a_file;
obj_attr.Attributes = OBJ_CASE_INSENSITIVE;

stat = NtOpenFile(&handle, (GENERIC_READ|0x00100000),
  &obj_attr, &io_stat_block, (FILE_SHARE_READ|FILE_SHARE_WRITE), 0x10);

if (stat != STATUS_SUCCESS) {
 return stat;
}

memset(ioctl_cmd_in_buf, 0, sizeof(ioctl_cmd_in_buf));
ioctl_cmd_in_buf[0] = IOCTL_SUBCMD_GET_INFO;

memset(ioctl_cmd_out_buf, 0, sizeof(ioctl_cmd_out_buf));

stat = NtDeviceIoControlFile(handle, 0, 0, 0, &io_stat_block,
 IOCTL_CMD_LBA48_ACCESS,
 ioctl_cmd_in_buf, sizeof(ioctl_cmd_in_buf),
 ioctl_cmd_out_buf, sizeof(ioctl_cmd_out_buf));

NtClose(handle);
if (stat != STATUS_SUCCESS) {
 return stat;
}

if ((ioctl_cmd_out_buf[LBA48_GET_INFO_MAGIC1_IDX] != LBA48_GET_INFO_MAGIC1_VAL) ||
 (ioctl_cmd_out_buf[LBA48_GET_INFO_MAGIC2_IDX] != LBA48_GET_INFO_MAGIC2_VAL)) {

 return STATUS_UNSUCCESSFUL;
}

partition_table_addr = ioctl_cmd_out_buf[LBA48_GET_INFO_LOWCODE_BASE_IDX];
partition_table_addr += ioctl_cmd_out_buf[LBA48_GET_INFO_PART_TABLE_OFS_IDX];

memcpy(p_table, (void *)partition_table_addr, sizeof(PARTITION_TABLE));

return STATUS_SUCCESS;
}


#if 0
//
// Xbox standard partition definitions
//

#define XBOX_SWAPPART1_LBA_START 0x400
#define XBOX_SWAPPART_LBA_SIZE  0x177000
#define XBOX_SWAPPART2_LBA_START (XBOX_SWAPPART1_LBA_START + XBOX_SWAPPART_LBA_SIZE)
#define XBOX_SWAPPART3_LBA_START (XBOX_SWAPPART2_LBA_START + XBOX_SWAPPART_LBA_SIZE)

#define XBOX_SYSPART_LBA_START  (XBOX_SWAPPART3_LBA_START + XBOX_SWAPPART_LBA_SIZE)
#define XBOX_SYSPART_LBA_SIZE  0xfa000

#define XBOX_MUSICPART_LBA_START (XBOX_SYSPART_LBA_START + XBOX_SYSPART_LBA_SIZE)
#define XBOX_MUSICPART_LBA_SIZE  0x9896b0

#define XBOX_STANDARD_MAX_LBA  (XBOX_MUSICPART_LBA_START + XBOX_MUSICPART_LBA_SIZE)

#define MAX_LBA28_SECTOR  0xfffffff

// The following is read from the drive at boot-up time.  I've just
// put my 200GB Maxtor's size here as a default
#define MY_DRIVE_LBA48_SIZE  0x17bd8800 // 200GB Maxtor

PARTITION_TABLE     Partition_table = {
   {'*','*','*','*','P','A','R','T','I','N','F','O','*','*','*','*'},
   {0},

   {
{ {'X','B','O','X',' ','M','U','S','I','C',' ',' ',' ',' ',' ',' '},
 PE_PARTFLAGS_IN_USE,
 XBOX_MUSICPART_LBA_START,XBOX_MUSICPART_LBA_SIZE,0 },
{ {'X','B','O','X',' ','S','Y','S','T','E','M',' ',' ',' ',' ',' '},
 PE_PARTFLAGS_IN_USE,
 XBOX_SYSPART_LBA_START,XBOX_SYSPART_LBA_SIZE,0 },
{ {'X','B','O','X',' ','G','A','M','E',' ','S','W','A','P',' ','1'},
 PE_PARTFLAGS_IN_USE,
 XBOX_SWAPPART1_LBA_START,XBOX_SWAPPART_LBA_SIZE,0 },
{ {'X','B','O','X',' ','G','A','M','E',' ','S','W','A','P',' ','2'},
 PE_PARTFLAGS_IN_USE,
 XBOX_SWAPPART2_LBA_START,XBOX_SWAPPART_LBA_SIZE,0 },
{ {'X','B','O','X',' ','G','A','M','E',' ','S','W','A','P',' ','3'},
 PE_PARTFLAGS_IN_USE,
 XBOX_SWAPPART3_LBA_START,XBOX_SWAPPART_LBA_SIZE,0 },
{ {'D','R','I','V','E',' ','F',':',' ',' ',' ',' ',' ',' ',' ',' '},
 PE_PARTFLAGS_IN_USE,
 XBOX_STANDARD_MAX_LBA, (MAX_LBA28_SECTOR-XBOX_STANDARD_MAX_LBA),0 },
{ {'D','R','I','V','E',' ','G',':',' ',' ',' ',' ',' ',' ',' ',' '},
 PE_PARTFLAGS_IN_USE,
 MAX_LBA28_SECTOR, (MY_DRIVE_LBA48_SIZE-MAX_LBA28_SECTOR),0 },
{ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
 0,0,0,0 },
{ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
 0,0,0,0 },
{ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
 0,0,0,0 },
{ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
 0,0,0,0 },
{ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
 0,0,0,0 },
{ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
 0,0,0,0 },
{ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
 0,0,0,0 },
   },
};

#endif