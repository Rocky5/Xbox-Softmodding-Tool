;;;
;;; xa_lba48.asm
;;; 
;;; LBA48 support by Paul Bartholomew (oz_paulb)
;;; Copyright (c) 2003 - Released under GNU Public License
;;; 
;;; Modified and adapted for nkpatcher by rmenhal
;;; Copyright 2004 rmenhal
;;; 
;;; Licensed under GNU General Public License version 2. See the file COPYING
;;; for details.
;;;

%include "diskcdromdef.inc"
%include "lba48.inc"


	
DEF_PARTMETHOD_ONLY_STANDARD_PARTITIONS			equ	0
DEF_PARTMETHOD_PART6_REST_OF_DRIVE			equ	1
DEF_PARTMETHOD_PART6_UNDER_137GB_PART7_REST_OF_DRIVE	equ	2
DEF_PARTMETHOD_PART6_UNDER_137GB_NO_PART7			equ	3



XBOX_SWAPPART1_LBA_START	equ 0x400
XBOX_SWAPPART_LBA_SIZE	equ 0x177000
XBOX_SWAPPART2_LBA_START	equ (XBOX_SWAPPART1_LBA_START + XBOX_SWAPPART_LBA_SIZE)
XBOX_SWAPPART3_LBA_START	equ (XBOX_SWAPPART2_LBA_START + XBOX_SWAPPART_LBA_SIZE)

XBOX_SYSPART_LBA_START	equ (XBOX_SWAPPART3_LBA_START + XBOX_SWAPPART_LBA_SIZE)
XBOX_SYSPART_LBA_SIZE	equ 0xfa000

XBOX_MUSICPART_LBA_START	equ (XBOX_SYSPART_LBA_START + XBOX_SYSPART_LBA_SIZE)
XBOX_MUSICPART_LBA_SIZE	equ 0x9896b0

XBOX_STANDARD_MAX_LBA	equ (XBOX_MUSICPART_LBA_START + XBOX_MUSICPART_LBA_SIZE)



xa_lba48_partition_table:	
.hdr:
	db	'****PARTINFO****'
	times 32 db 0	; reserved

	PT_ENTRY	'XBOX MUSIC      ', \
			PE_PARTFLAGS_IN_USE, \
			XBOX_MUSICPART_LBA_START, \
			XBOX_MUSICPART_LBA_SIZE, \
			0
	PT_ENTRY	'XBOX SYSTEM     ', \
			PE_PARTFLAGS_IN_USE, \
			XBOX_SYSPART_LBA_START, \
			XBOX_SYSPART_LBA_SIZE, \
			0
	PT_ENTRY	'XBOX GAME SWAP 1', \
			PE_PARTFLAGS_IN_USE, \
			XBOX_SWAPPART1_LBA_START, \
			XBOX_SWAPPART_LBA_SIZE, \
			0
	PT_ENTRY	'XBOX GAME SWAP 2', \
			PE_PARTFLAGS_IN_USE, \
			XBOX_SWAPPART2_LBA_START, \
			XBOX_SWAPPART_LBA_SIZE, \
			0
	PT_ENTRY	'XBOX GAME SWAP 3', \
			PE_PARTFLAGS_IN_USE, \
			XBOX_SWAPPART3_LBA_START, \
			XBOX_SWAPPART_LBA_SIZE, \
			0
xa_lba48_partition_table_first_available:
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0
	PT_ENTRY	'                ',0,0,0,0

%if ($-xa_lba48_partition_table) != PARTITION_TABLE_SIZE
	%error Partition table is bad.
%endif
	
	times (512-PARTITION_TABLE_SIZE) db 0



xa_lba48_def_partition_method:	
	db	PARTITION_METHOD

xa_lba48_ignore_HD_part_table:
%ifdef IGNORE_HD_PARTITION_TABLE
	db	1
%else
	db	0
%endif

align 4
xa_lba48_identify_data:
	times 512 db 0


	



xa_lba48_fill_parameters:
.io_status		equ 0 - 8
.ata_pass		equ .io_status - ATA_PASS_THROUGH_size
.part_buf		equ .ata_pass - 512
.local_var_size		equ -.part_buf

	;; [esp+4] = Address to FEATURE_PARAMETERS for filling LBA48 parameters
	
	push	ebp
	mov	ebp,esp
	sub	esp,.local_var_size

	push	ebx
	push	esi
	push	edi
		

	lea	esi,[ebp + .io_status]	; IO status block
	
	push	eax
	mov	ebx,esp
	
	push	byte 40h
	push	.part0_ansistr
	push	byte 0
	mov	edx,esp		; Object attributes

	push	byte 20h	; FILE_SYNCHRONOUS_IO_NONALERT
	push	byte 3		; FILE_SHARE_READ | FILE_SHARE_WRITE
	push	esi
	push	edx
	push	80100000h	; GENERIC_READ | SYNCHRONIZE
	push	ebx
	call	dword [NtOpenFile]
	add	esp,byte 12
	pop	ebx
	test	eax,eax
	jl	near .exit


	xor	eax,eax

	lea	edi,[ebp + .ata_pass]

	mov	[edi + ATA_PASS_THROUGH.IdeReg],eax
	mov	[edi + ATA_PASS_THROUGH.IdeReg+4],eax
	mov	byte [edi + ATA_PASS_THROUGH.IdeReg + IDEREGS.bCommandReg],0ECh ; ATA Identify Device command
	mov	dword [edi + ATA_PASS_THROUGH.DataBufferSize],512
	mov	dword [edi + ATA_PASS_THROUGH.DataBuffer],xa_lba48_identify_data
	
	push	byte ATA_PASS_THROUGH_size
	push	edi
	push	byte ATA_PASS_THROUGH_size
	push	edi
	push	IOCTL_IDE_PASS_THROUGH
	push	esi
	push	eax
	push	eax
	push	eax
	push	ebx
	call	dword [NtDeviceIoControlFile]
	test	eax,eax
	jl	.exit_close

	
	call	.get_drive_total_sectors

	mov	edx,[ebp+8]
	mov	[edx + FEATURE_PARAMETERS.lba48_total_sectors],eax
	mov	dword [edx + FEATURE_PARAMETERS.lba48_partition_table],xa_lba48_partition_table

	
	cmp	byte [xa_lba48_ignore_HD_part_table],0
	jne	.usual_parts

	lea	edi,[ebp + .part_buf]
	
	xor	eax,eax
	push	eax
	push	eax
	push	esp
	push	512
	push	edi
	push	esi
	push	eax
	push	eax
	push	eax
	push	ebx
	call	dword [NtReadFile]
	pop	ecx
	pop	edx
	test	eax,eax
	jl	.usual_parts

	mov	esi,edi
	mov	edi,xa_lba48_partition_table.hdr
	push	byte (PARTITION_MAGIC_LEN/4)
	pop	ecx
	repe	cmpsd
	jne	.usual_parts

	mov	ecx,PARTITION_TABLE_SIZE-PARTITION_MAGIC_LEN
	rep	movsb
	jmp	short .exit_close

.usual_parts:
	mov	ecx,[ebp+8]
	mov	ecx,[ecx + FEATURE_PARAMETERS.lba48_total_sectors]
	call	.setup_usual_partition_table

.exit_close:
	push	ebx
	call	dword [NtClose]
	
.exit:
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret



.part0_strz		db '\Device\Harddisk0\Partition0',0
.part0_ansistr		dw $-.part0_strz-1, $-.part0_strz
			dd .part0_strz


.ID_OFS__CMD_SET_SUPPORTED	equ	(83*2)
.BITMASK__CMD_SET_HAS_LBA48	equ	(1<<10)

.ID_OFS__TOTAL_USER_SECTORS_LBA28	equ	(60*2)

.ID_OFS__TOTAL_USER_SECTORS_LBA48_LOW	equ	(100*2)
.ID_OFS__TOTAL_USER_SECTORS_LBA48_HIGH	equ	(102*2)


	
.get_drive_total_sectors:	
	mov	edx,xa_lba48_identify_data
	
	mov	eax,[edx + .ID_OFS__CMD_SET_SUPPORTED]
	test	eax,.BITMASK__CMD_SET_HAS_LBA48
	jnz	short .drive_supports_lba48

.no_lba48_support:
	mov	eax,[edx + .ID_OFS__TOTAL_USER_SECTORS_LBA28]
	jmp	short .have_total_sectors

.drive_supports_lba48:
	cmp	dword [edx + .ID_OFS__TOTAL_USER_SECTORS_LBA48_HIGH],byte 0
	ja	short .drive_is_huge__limit_to_32bits
	mov	eax,[edx + .ID_OFS__TOTAL_USER_SECTORS_LBA48_LOW]
	jmp	short .have_total_sectors

.drive_is_huge__limit_to_32bits:
	or	eax,byte -1

.have_total_sectors:	
	ret
	




.setup_usual_partition_table:
	push	ebx
	push	esi
	push	edi
	
	mov	eax,ecx
	mov	edx,ecx
	cmp	eax,XBOX_STANDARD_MAX_LBA
	ja	.try_extra_partitions
.j_skip_extra_partitions:	
	jmp	.skip_extra_partitions

.try_extra_partitions:	

	cmp	byte [xa_lba48_def_partition_method],DEF_PARTMETHOD_ONLY_STANDARD_PARTITIONS
	jz	short .j_skip_extra_partitions



	sub	eax,XBOX_STANDARD_MAX_LBA
	;; eax now free space at end of drive

	mov	ebx,eax
	xor	ecx,ecx
	;; ebx holds part6 size (default to all free space)
	;; ecx holds part7 size (default to zero)

	;; if drive isn't > 137gb, just create part6
	
	cmp	edx,0x0fffffff
	jbe	.create_partitions

.drive_larger_than_137gb:
	cmp	byte [xa_lba48_def_partition_method],DEF_PARTMETHOD_PART6_REST_OF_DRIVE
	jz	.create_partitions

	mov	ebx,0x0fffffff - XBOX_STANDARD_MAX_LBA
	;; ebx holds part6 size (rest of space before 137gb)

	cmp	byte [xa_lba48_def_partition_method],DEF_PARTMETHOD_PART6_UNDER_137GB_NO_PART7
	jz	.create_partitions

	mov	ecx,edx				;edx holds total drive sectors
	sub	ecx,XBOX_STANDARD_MAX_LBA	;ecx now free space at end of
						;drive
	sub	ecx,ebx				;subtract space used for part6
	;; ecx holds part7 size

.create_partitions:	

.drive_less_or_equal_to_137gb:

	;; ebx = part6 size
	;; ecx = part7 size
	;; edx = total drive sectors

	mov	eax,xa_lba48_partition_table_first_available
	
	mov	esi,.drive_f_str
	lea	edi,[eax + _PE_OFS__part_name]
	times 4 movsd

	mov	dword [eax + _PE_OFS__part_lba_start],XBOX_STANDARD_MAX_LBA
	mov	dword [eax + _PE_OFS__part_lba_size],ebx

	mov	dword [eax + _PE_OFS__part_flags],PE_PARTFLAGS_IN_USE

	
	test	ecx,ecx
	jz	.partitions_created

	add	eax,byte _PE_SIZ__part_entry
	
	mov	esi,.drive_g_str
	lea	edi,[eax + _PE_OFS__part_name]
	times 4 movsd
	
	mov	dword [eax + _PE_OFS__part_lba_start],XBOX_STANDARD_MAX_LBA
	add	dword [eax + _PE_OFS__part_lba_start],ebx
	mov	dword [eax + _PE_OFS__part_lba_size],ecx

	mov	dword [eax + _PE_OFS__part_flags],PE_PARTFLAGS_IN_USE

	
.partitions_created:
.skip_extra_partitions:	
.partitions_done:

	pop	edi
	pop	esi
	pop	ebx	
	ret


align 4
.drive_f_str	db 'DRIVE F:        '
.drive_g_str	db 'DRIVE P:        '
