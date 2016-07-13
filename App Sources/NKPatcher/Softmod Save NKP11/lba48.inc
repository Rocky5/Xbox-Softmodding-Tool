%ifndef LBA48_INC
%define LBA48_INC


%macro	PT_ENTRY	5
	db	%1	;partition name
	dd	%2	;partition flags
	dd	%3	;partition LBA start
	dd	%4	;partition LBA size
	dd	%5	;reserved
%endmacro

_PE_OFS__part_name	equ	(0)
_PE_OFS__part_flags	equ	(16)
PE_PARTFLAGS_IN_USE	equ	0x80000000
_PE_OFS__part_lba_start	equ	(20)
_PE_OFS__part_lba_size	equ	(24)
_PE_OFS__part_reserved	equ	(28)
_PE_SIZ__part_entry	equ	(32)

MAX_PARTITIONS		equ	14
PARTITION_MAGIC_LEN	equ	16

_PT_OFS__part_entries	equ	48

PARTITION_TABLE_SIZE	equ	(_PT_OFS__part_entries + \
				 _PE_SIZ__part_entry*MAX_PARTITIONS)


%endif ; LBA48_INC
