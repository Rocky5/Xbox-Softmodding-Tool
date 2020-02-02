#include "stdafx.h"
#include "partition.h"

void part_sort(XboxPartitionTable *PartTbl, ULONG *starts, ULONG *ends)
{
	int i;
	ULONG temp;

	for (i = 0; i < 14; i++)
	{
		starts[i] = PartTbl->TableEntries[i].LBAStart;
		ends[i] = PartTbl->TableEntries[i].LBASize + PartTbl->TableEntries[i].LBAStart;
	}
	//sort
	for (i = 0; i < 13; i++)
	{
		if (starts[i] > starts[i+1])
		{
			temp = starts[i];
			starts[i] = starts[i+1];
			starts[i+1] = temp;
			temp = ends[i];
			ends[i] = ends[i+1];
			ends[i+1] = temp;
			i = -1;
		}
	}
}

int part_get_active_count(XboxPartitionTable *PartTbl)
{
	int i, count = 0;

	for (i = 0; i < 14; i++)
	{
		if (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE)
			count++;
	}

	return count;
}

ULONG part_get_first_free_start(XboxPartitionTable *PartTbl)
{
	int i;
	ULONG starts[14], ends[14];

	if (!part_get_active_count(PartTbl))
		return XBOX_SWAPPART1_LBA_START;

	part_sort(PartTbl, starts, ends);

	for (i = 0; i < 13; i++)
	{
		if (!starts[i])
			continue;

		if (ends[i] < starts[i+1])
			return ends[i];
	}

	return ends[13];

}

ULONG part_get_current_free(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors)
{
	int i;
	ULONG starts[14], ends[14];

	if (!part_get_active_count(PartTbl))
		return (TotalSectors - 1) - XBOX_SWAPPART1_LBA_START;

	part_sort(PartTbl, starts, ends);

	for (i = 0; i < 13; i++)
	{
		if (!starts[i])
			continue;

		if (starts[i] == PartTbl->TableEntries[PartNumber].LBAStart)
			return starts[i+1] - ends[i];
	}

	return TotalSectors - ends[13];

}

ULONG part_get_ceil(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors)
{
	int i, ceil = -1;

	for (i = 0; i < 14; i++)
	{
		if (i == PartNumber)
			continue;

		if (PartTbl->TableEntries[i].LBAStart > PartTbl->TableEntries[PartNumber].LBAStart)
		{
			if (ceil < 0 || PartTbl->TableEntries[i].LBAStart < PartTbl->TableEntries[ceil].LBAStart)
			{
				ceil = i;
			}
		}
	}
	if (ceil < 0)
		return TotalSectors - (PartTbl->TableEntries[PartNumber].LBAStart + PartTbl->TableEntries[PartNumber].LBASize);
	else
		return PartTbl->TableEntries[ceil].LBAStart - (PartTbl->TableEntries[PartNumber].LBAStart + PartTbl->TableEntries[PartNumber].LBASize);
}

void part_add_size(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG addition)
{
	ULONG max = part_get_current_free(PartTbl, PartNumber, TotalSectors);

	if (addition > max)
		addition = max;

	PartTbl->TableEntries[PartNumber].LBASize += addition;
}

void part_sub_size(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG subtraction)
{
	if (PartTbl->TableEntries[PartNumber].LBASize <= subtraction)
		PartTbl->TableEntries[PartNumber].LBASize = 0;
	else
		PartTbl->TableEntries[PartNumber].LBASize -= subtraction;
}

void part_change_size(XboxPartitionTable *PartTbl, int PartitionNum, int TotalSectors, int TrigValue, int change_start, int Direction, bool fine)
{
	int num_sectors;

	void (*change_ops[])(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG addition) =
	{ part_add_size, part_sub_size, part_down, part_up };

	if (fine)
	{
		num_sectors = 1;
		Sleep(250);
	}
	else if (TrigValue < 0x60)
	{
		num_sectors = 16;
		Sleep(250);
	}
	else if (TrigValue > 0xf0)
	{
		num_sectors = SECTORS_PER_CLUSTER * 0x20000;
	}
	else
	{
		TrigValue >>= 4;
		num_sectors = SECTORS_PER_CLUSTER * 2 << (TrigValue - 4);
	}

	change_ops[change_start * 2 + (Direction < 0)](PartTbl, PartitionNum, TotalSectors, num_sectors);
}

bool part_enable(XboxPartitionTable *PartTbl, ULONG TotalSectors, int PartNumber)
{
	ULONG free = part_get_free_sectors(PartTbl, TotalSectors);

	if (free < 32)
		return false;

	PartTbl->TableEntries[PartNumber].LBAStart = part_get_first_free_start(PartTbl);
	PartTbl->TableEntries[PartNumber].LBASize = 32;
	PartTbl->TableEntries[PartNumber].Flags |= PE_PARTFLAGS_IN_USE;

	return true;
}

void part_disable(XboxPartitionTable *PartTbl, int PartNumber)
{
	PartTbl->TableEntries[PartNumber].Flags &= ~PE_PARTFLAGS_IN_USE;
	PartTbl->TableEntries[PartNumber].LBAStart = 0;
	PartTbl->TableEntries[PartNumber].LBASize = 0;
}

int part_get_last_available(XboxPartitionTable *PartTbl)
{
	int i, n;

	for (i = 0, n = 13; i <= n; i++)
	{
		if (PartTbl->TableEntries[i].Flags ^ PE_PARTFLAGS_IN_USE)
		{
			n = i;
			break;
		}
	}

	return n;
}

void part_up(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG subtraction)
{
	if (PartTbl->TableEntries[PartNumber].LBASize <= subtraction)
		PartTbl->TableEntries[PartNumber].LBASize = 0;
	else
		PartTbl->TableEntries[PartNumber].LBASize -= subtraction;

	part_sub_size(PartTbl, PartNumber, TotalSectors, subtraction);

	PartTbl->TableEntries[PartNumber].LBAStart += subtraction;
}

DWORD part_get_current_free_below(XboxPartitionTable *PartTbl, int PartNumber)
{
	int i;
	ULONG starts[14], ends[14];

	if (PartTbl->TableEntries[PartNumber].LBAStart == XBOX_SWAPPART1_LBA_START)
		return 0;

	part_sort(PartTbl, starts, ends);

	for (i = 14; i >= 1; i--)
	{
		if (!ends[i])
			continue;

		if (ends[i] <= PartTbl->TableEntries[PartNumber].LBAStart)
			return PartTbl->TableEntries[PartNumber].LBAStart - ends[i];
	}

	return starts[0] - XBOX_SWAPPART1_LBA_START;
}

void part_down(XboxPartitionTable *PartTbl, int PartNumber, ULONG TotalSectors, ULONG addition)
{
	ULONG max = part_get_current_free_below(PartTbl, PartNumber);

	if (addition > max)
		addition = max;

	PartTbl->TableEntries[PartNumber].LBAStart -= addition;
	PartTbl->TableEntries[PartNumber].LBASize += addition;
}

void part_even(XboxPartitionTable *PartTbl, ULONG TotalSectors)
{
	int i, num_active = 0;
	ULONG free_sectors = part_get_free_sectors(PartTbl, TotalSectors);

	for (i = 5; i < 14; i++)
	{
		if (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE)
		{
			num_active++;
			free_sectors += PartTbl->TableEntries[i].LBASize;
			PartTbl->TableEntries[i].LBAStart = 0;
			PartTbl->TableEntries[i].LBASize = 0;
		}
	}

	if (!num_active)
		return;

	free_sectors /= num_active;

	for (i = 5; i < 14; i++)
	{
		if (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE)
		{
			PartTbl->TableEntries[i].LBAStart = part_get_first_free_start(PartTbl);
			PartTbl->TableEntries[i].LBASize = free_sectors - (free_sectors % 16);
		}
	}
}

void part_init_unused(XboxPartitionTable *PartTbl)
{
	int i;

	for (i = 5; i < 14; i++)
	{
		if (PartTbl->TableEntries[i].Flags ^ PE_PARTFLAGS_IN_USE)
		{
			PartTbl->TableEntries[i].LBAStart = 0;
			PartTbl->TableEntries[i].LBASize = 0;
		}
	}
}

ULONG part_get_free_sectors(XboxPartitionTable *PartTbl, ULONG TotalSectors)
{
	int i;
	ULONG Used = 0;

	for (i = 0; i < 14; i++)
	{
		if (PartTbl->TableEntries[i].Flags | PE_PARTFLAGS_IN_USE)
			Used += PartTbl->TableEntries[i].LBASize;
	}
	if (TotalSectors < Used + XBOX_SWAPPART1_LBA_START)
		return 0;
	else
		return TotalSectors - Used - XBOX_SWAPPART1_LBA_START;
}

ULONG part_get_used_sectors(XboxPartitionTable *PartTbl)
{
	int i;
	ULONG Used = 0;

	for (i = 0; i < 14; i++)
	{
		if (PartTbl->TableEntries[i].Flags | PE_PARTFLAGS_IN_USE)
			Used += PartTbl->TableEntries[i].LBASize;
	}
	return Used;
}

void part_setup_standard_partitions(XboxPartitionTable *PartTbl, XboxPartitionTable *BackupPartTbl, int TotalSectors, int Type)
{
	memcpy(PartTbl, BackupPartTbl, sizeof(XboxPartitionTable));

	PartTbl->TableEntries[5].Flags |= PE_PARTFLAGS_IN_USE;
	PartTbl->TableEntries[5].LBAStart = XBOX_PART6_LBA_START;
	switch (Type)
	{
	case 0:
		if (TotalSectors & 0xf0000000)
			PartTbl->TableEntries[5].LBASize = 0xfffffffUL - PartTbl->TableEntries[5].LBAStart;
		else
			PartTbl->TableEntries[5].LBASize = TotalSectors - PartTbl->TableEntries[5].LBAStart;
			PartTbl->TableEntries[6].Flags &= ~PE_PARTFLAGS_IN_USE;
		break;
	case 1:
		PartTbl->TableEntries[5].LBASize = TotalSectors - PartTbl->TableEntries[5].LBAStart;
		PartTbl->TableEntries[6].Flags &= ~PE_PARTFLAGS_IN_USE;
		break;
	case 2:
		if (TotalSectors & 0xf0000000)
		{
			PartTbl->TableEntries[5].LBASize = 0xfffffffUL - PartTbl->TableEntries[5].LBAStart;
			PartTbl->TableEntries[6].LBAStart = 0xfffffffUL;
			PartTbl->TableEntries[6].LBASize = TotalSectors - 0xfffffffUL;
			PartTbl->TableEntries[6].Flags |= PE_PARTFLAGS_IN_USE;
		}
		else
		{
			PartTbl->TableEntries[5].LBASize = TotalSectors - PartTbl->TableEntries[5].LBAStart;
			PartTbl->TableEntries[6].Flags &= ~PE_PARTFLAGS_IN_USE;
		}
		break;
	case 3:
		PartTbl->TableEntries[5].LBASize = (ULONG) (TotalSectors - PartTbl->TableEntries[5].LBAStart) / 2;
		PartTbl->TableEntries[6].LBAStart = PartTbl->TableEntries[5].LBAStart + PartTbl->TableEntries[5].LBASize;
		PartTbl->TableEntries[6].LBASize = TotalSectors - PartTbl->TableEntries[6].LBAStart;
		PartTbl->TableEntries[6].Flags |= PE_PARTFLAGS_IN_USE;
		break;
	default:
		break;
	}
	for (int i = 7; i < 14; i++)
		PartTbl->TableEntries[i].Flags &= ~PE_PARTFLAGS_IN_USE;

	part_init_unused(PartTbl);

}