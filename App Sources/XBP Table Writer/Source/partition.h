
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

#define XBOX_SWAPPART1_LBA_START		0x400
#define XBOX_SWAPPART_LBA_SIZE			0x177000
#define XBOX_SWAPPART2_LBA_START		(XBOX_SWAPPART1_LBA_START + XBOX_SWAPPART_LBA_SIZE)
#define XBOX_SWAPPART3_LBA_START		(XBOX_SWAPPART2_LBA_START + XBOX_SWAPPART_LBA_SIZE)

#define XBOX_SYSPART_LBA_START			(XBOX_SWAPPART3_LBA_START + XBOX_SWAPPART_LBA_SIZE)
#define XBOX_SYSPART_LBA_SIZE			0xfa000

#define XBOX_MUSICPART_LBA_START		(XBOX_SYSPART_LBA_START + XBOX_SYSPART_LBA_SIZE)
#define XBOX_MUSICPART_LBA_SIZE			0x9896b0

#define XBOX_PART6_LBA_START			(XBOX_MUSICPART_LBA_START + XBOX_MUSICPART_LBA_SIZE)


#define XBOX_STANDARD_MAX_LBA			(XBOX_MUSICPART_LBA_START + XBOX_MUSICPART_LBA_SIZE)

#define PE_PARTFLAGS_IN_USE	0x80000000

#define CLUSTER_SIZE 0x4000
#define SECTORS_PER_CLUSTER 32

#define LBASIZE_512GB   1073741824
#define LBASIZE_256GB   536870912

typedef struct
{
	UCHAR Name[16];
	ULONG Flags;
	ULONG LBAStart;
	ULONG LBASize;
	ULONG Reserved;
} XboxPartitionTableEntry;


typedef struct
{
	UCHAR	Magic[16];
	char	Res0[32];
	XboxPartitionTableEntry	TableEntries[14];
} XboxPartitionTable;

typedef struct _FATX_SUPERBLOCK
{
	char	Tag[4];
	unsigned int		VolumeID;
	unsigned int	ClusterSize;
	USHORT	FatCopies;
	int		Resvd;
	char	Unused[4078];
} FATX_SUPERBLOCK;

void part_add_size(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG addition);
void part_sub_size(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG subtraction);
void part_up(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG addition);
void part_down(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG subtraction);
void part_even(XboxPartitionTable *PartTbl, ULONG TotalSectors);
void part_change_size(XboxPartitionTable *PartTbl, int PartitionNum, int TotalSectors, int TrigValue, int change_start, int Direction, bool fine = 0);
void part_init_unused(XboxPartitionTable *PartTbl);
void part_disable(XboxPartitionTable *PartTbl, int PartNumber);
bool part_enable(XboxPartitionTable *PartTbl, ULONG TotalSectors, int PartNumber);
int part_get_last_available(XboxPartitionTable *PartTbl);
ULONG part_get_free_sectors(XboxPartitionTable *PartTbl, ULONG TotalSectors);
ULONG part_get_used_sectors(XboxPartitionTable *PartTbl);
void part_setup_standard_partitions(XboxPartitionTable *PartTbl, XboxPartitionTable *BackupPartTbl, int TotalSectors, int Type);
ULONG part_get_ceil(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors);