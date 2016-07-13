;;;
;;; lba48.asm
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

	
%macro	lba48 10
%ifdef LBA48
	mov	eax,%1
	mov	ebx,%2
	mov	ecx,%3
	mov	edx,%4
	push	%10
	push	%9
	push	%8
	push	%7
	push	%6
	push	%5
	call	installlba48
%endif
%endmacro


	
%ifdef LBA48

%include "diskcdromdef.inc"
%include "lba48.inc"


	CODE_SECTION
	
installlba48:	
	mov	edi,[expd_space]
	test	edi,edi
	jz	near .exit

	mov	esi,[memdiff]

	push	eax
	mov	eax,[caller_param]
	mov	ebp,[eax + FEATURE_PARAMETERS.lba48_total_sectors]
	pop	eax
	test	ebp,ebp
	jz	near .exit
	mov	[edx+esi],ebp

	mov	[lba48_IDESetupReadWrite_hook.rsize+1],edx
	mov	[lba48_IDESetupVerify_hook.rsize+1],edx

	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+lba48_PartitionCreate_hook-lba48_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp

	setcalljmptarget ebx, lba48_patch, lba48_PartitionCreate_hook.var_jfail, edi
	setcalljmptarget ecx, lba48_patch, lba48_PartitionCreate_hook.var_jok, edi
	

	mov	eax,[esp+4]
	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+lba48_IDESetupReadWrite_hook-lba48_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp

	mov	eax,[esp+8]
	setcalljmptarget eax, lba48_patch, lba48_IDESetupReadWrite_hook.var_jback, edi

	mov	eax,[esp+12]
	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+lba48_IDESetupVerify_hook-lba48_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp

	mov	eax,[esp+16]
	setcalljmptarget eax, lba48_patch, lba48_IDESetupVerify_hook.var_jback, edi

	mov	eax,[esp+20]
	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+lba48_DiskIoControl_hook-lba48_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp
	mov	byte [eax+esi+5],90h ; nop instruction

	add	eax,byte 5
	setcalljmptarget eax, lba48_patch, lba48_DiskIoControl_hook.var_jback_cont, edi

	mov	eax,[esp+24]
	setcalljmptarget eax, lba48_patch, lba48_DiskIoControl_hook.var_jback, edi

	

	add	[lba48_PartitionCreate_hook.ptab+1],edi
	add	[lba48_DiskIoControl_hook.codeb+1],edi


	push	edi
	mov	esi,[caller_param]
	mov	esi,[esi + FEATURE_PARAMETERS.lba48_partition_table]
	mov	edi,lba48_partition_table
	mov	ecx,512/4
	rep	movsd
	pop	edi
	

	mov	ebp,[memdiff]

	mov	esi,lba48_patch
	mov	ecx,LBA48_SIZE/4
	add	edi,ebp
	rep	movsd
	sub	edi,ebp

	mov	[expd_space],edi
.exit:	
	ret	24




	DATA_SECTION

align 16

lba48_patch:	

;;; Originally version was 2. Version 3 is expected by XBpartitioner.
	
LBA48_PATCHCODE_VERSION	equ 3
LBA48_PATCH_MAGIC_ID	equ 0xcafebabe

;;; The values here are unused, but let's keep them anyway in case some weird
;;; software expects them.
	
.magic_id:
	dd	LBA48_PATCH_MAGIC_ID
.version_id:
	dd	LBA48_PATCHCODE_VERSION
.def_partition_method:
	db	PARTITION_METHOD
.ignore_HD_part_table:
%ifdef IGNORE_HD_PARTITION_TABLE
	db	1
%else
	db	0
%endif
	;more flag bytes
	times 2 db 0
.partition_table_offset:
	dd	lba48_partition_table-lba48_patch

	times 48 db 0

	db	'LBA48 support by Paul Bartholomew (oz_paulb)  ',13,10
	db	'Copyright (c) 2003 - Released under GNU Public License',13,10
	db	'Modified and adapted for nkpatcher by rmenhal 2004',13,10
	db	0
	align	16, db 0



lba48_PartitionCreate_hook:

%ifdef VIRTUAL_C_DRIVE
%ifdef ACTUAL_C_DRIVE
	cmp	eax,byte (ACTUAL_C_DRIVE-1)
	jne	.not_c_drive
	push	byte (VIRTUAL_C_DRIVE-1)
	pop	eax
.not_c_drive:	
%endif
%endif
	
	cmp	eax,byte MAX_PARTITIONS
	jae	.fail

	imul	eax,byte _PE_SIZ__part_entry
.ptab:	add	eax,lba48_partition_table - lba48_patch + _PT_OFS__part_entries

	test	dword [eax + _PE_OFS__part_flags],PE_PARTFLAGS_IN_USE
	jz	.fail

	push	dword [eax + _PE_OFS__part_lba_start]
	mov	eax,dword [eax + _PE_OFS__part_lba_size]
	mov	edx,512
	mul	edx
	mov	[ebp-20h],eax
	mov	[ebp-1Ch],edx
	pop	eax
	VJMP	jok

.fail:	mov	eax,0C0000034h
	VJMP	jfail





IDE_REG_SEC_COUNT		equ	0x1f2
IDE_REG_LBA_LOW			equ	0x1f3
IDE_REG_LBA_MID			equ	0x1f4
IDE_REG_LBA_HIGH			equ	0x1f5
IDE_REG_DEVICE			equ	0x1f6
IDE_REG_COMMAND			equ	0x1f7

IDE_CMD_READ_DMA			equ	0xc8
IDE_CMD_READ_DMA_EXT		equ	0x25
IDE_CMD_WRITE_DMA		equ	0xca
IDE_CMD_WRITE_DMA_EXT		equ	0x35
IDE_CMD_READ_VERIFY_SECTORS	equ	0x40
IDE_CMD_READ_VERIFY_SECTORS_EXT	equ	0x42

IO_BUSMASTER_REG			equ	0xff60
IO_BUSMASTER_CMD_READ		equ	0x09
IO_BUSMASTER_CMD_WRITE		equ	0x01

IRQL_VALUE_RET			equ	2

ESI_OFS_READ_OR_WRITE_FLAG	equ	0
RWFLAG_IS_READ			equ	2
ESI_OFS_NUM_BYTES		equ	4
ESI_OFS_LBA			equ	12
ESI_OFS_LBA_LOW			equ	(ESI_OFS_LBA+0)
ESI_OFS_LBA_MID			equ	(ESI_OFS_LBA+1)
ESI_OFS_LBA_HIGH			equ	(ESI_OFS_LBA+2)
ESI_OFS_LBA_XHIGH		equ	(ESI_OFS_LBA+3)




lba48_IDESetupReadWrite_hook:
	push	eax
	push	ebx
	push	edx
	push	esi
	push	edi

	mov	eax,edi	;byte count
	shr	eax,9		;convert to sector count
	mov	edi,eax

.rsize:	mov	eax,[8003B438h]
	and	eax,0xf0000000
	test	eax,eax
	jz	.is_lba28

	mov	eax,edi	;num sectors
	add	eax,dword [esi+ESI_OFS_LBA]
	and	eax,0xf0000000
	test	eax,eax
	jz	short .is_lba28

.is_lba48:
	mov	dx,IDE_REG_SEC_COUNT
	mov	eax,edi
	xchg	ah,al
	out	dx,al				;COUNTREG (15:8) = high(sector count)
	xchg	ah,al
	out	dx,al				;COUNTREG (7:0) = low(sector count)


	mov	dx,IDE_REG_LBA_LOW
	mov	al,byte [esi+ESI_OFS_LBA_XHIGH]
	out	dx,al				;LBAREG (31:24) = LBA (31:24)
	mov	al,byte [esi+ESI_OFS_LBA_LOW]
	out	dx,al				;LBAREG (7:0) = LBA (7:0)

	mov	dx,IDE_REG_LBA_MID
	xor	al,al
	out	dx,al				;LBAREG (39:32) = 0
	mov	al,byte [esi+ESI_OFS_LBA_MID]
	out	dx,al				;LBAREG (15:8) = LBA (15:8)

	mov	dx,IDE_REG_LBA_HIGH
	xor	al,al
	out	dx,al				;LBAREG (47:40) = 0
	mov	al,byte [esi+ESI_OFS_LBA_HIGH]
	out	dx,al				;LBAREG (23:16) = LBA (23:16)

	mov	al,0x40
	mov	dx,IDE_REG_DEVICE
	out	dx,al


	cmp	byte [esi+ESI_OFS_READ_OR_WRITE_FLAG],RWFLAG_IS_READ
	jnz	short	.is_write_lba48


.is_read_lba48:
	mov	al,IDE_CMD_READ_DMA_EXT
	mov	cl,IO_BUSMASTER_CMD_READ
	jmp	short .write_ide_rw_command

.is_write_lba48:
	mov	al,IDE_CMD_WRITE_DMA_EXT
	mov	cl,IO_BUSMASTER_CMD_WRITE
	jmp	short .write_ide_rw_command



.is_lba28:
	mov	eax,edi
	mov	dx,IDE_REG_SEC_COUNT
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_LOW]
	mov	dx,IDE_REG_LBA_LOW
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_MID]
	mov	dx,IDE_REG_LBA_MID
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_HIGH]
	mov	dx,IDE_REG_LBA_HIGH
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_XHIGH]
	and	al,0x0f
	or	al,0xe0
	mov	dx,IDE_REG_DEVICE
	out	dx,al

	cmp	byte [esi+ESI_OFS_READ_OR_WRITE_FLAG],RWFLAG_IS_READ
	jnz	short	.is_write_lba28


.is_read_lba28:
	mov	al,IDE_CMD_READ_DMA
	mov	cl,IO_BUSMASTER_CMD_READ
	jmp	short .write_ide_rw_command

.is_write_lba28:
	mov	al,IDE_CMD_WRITE_DMA
	mov	cl,IO_BUSMASTER_CMD_WRITE

.write_ide_rw_command
	mov	dx,IDE_REG_COMMAND
	out	dx,al

	mov	al,cl
	mov	dx,IO_BUSMASTER_REG
	out	dx,al

	pop	edi
	pop	esi
	pop	edx
	pop	ebx
	pop	eax


	pop	edi	;calling code pushed these, so pop them
	pop	esi
.exit:
	mov	cl,IRQL_VALUE_RET

	VJMP	jback





lba48_IDESetupVerify_hook:
	push	eax
	push	ebx
	push	edx
	push	esi
	push	edi

	mov	eax,[esi+ESI_OFS_NUM_BYTES]
	shr	eax,9		;convert to sector count
	mov	edi,eax

.rsize:	mov	eax,[8003B438h]
	and	eax,0xf0000000
	test	eax,eax
	jz	.is_lba28

	mov	eax,edi	;num sectors
	add	eax,dword [esi+ESI_OFS_LBA]
	and	eax,0xf0000000
	test	eax,eax
	jz	short .is_lba28

.is_lba48:
	mov	dx,IDE_REG_SEC_COUNT
	mov	eax,edi
	xchg	ah,al
	out	dx,al				;COUNTREG (15:8) = high(sector count)
	xchg	ah,al
	out	dx,al				;COUNTREG (7:0) = low(sector count)


	mov	dx,IDE_REG_LBA_LOW
	mov	al,byte [esi+ESI_OFS_LBA_XHIGH]
	out	dx,al				;LBAREG (31:24) = LBA (31:24)
	mov	al,byte [esi+ESI_OFS_LBA_LOW]
	out	dx,al				;LBAREG (7:0) = LBA (7:0)

	mov	dx,IDE_REG_LBA_MID
	xor	al,al
	out	dx,al				;LBAREG (39:32) = 0
	mov	al,byte [esi+ESI_OFS_LBA_MID]
	out	dx,al				;LBAREG (15:8) = LBA (15:8)

	mov	dx,IDE_REG_LBA_HIGH
	xor	al,al
	out	dx,al				;LBAREG (47:40) = 0
	mov	al,byte [esi+ESI_OFS_LBA_HIGH]
	out	dx,al				;LBAREG (23:16) = LBA (23:16)

	mov	al,0x40
	mov	dx,IDE_REG_DEVICE
	out	dx,al

	mov	al,IDE_CMD_READ_VERIFY_SECTORS_EXT
	jmp	short .write_ide_command



.is_lba28:
	mov	eax,edi
	mov	dx,IDE_REG_SEC_COUNT
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_LOW]
	mov	dx,IDE_REG_LBA_LOW
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_MID]
	mov	dx,IDE_REG_LBA_MID
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_HIGH]
	mov	dx,IDE_REG_LBA_HIGH
	out	dx,al

	mov	al,byte [esi+ESI_OFS_LBA_XHIGH]
	and	al,0x0f
	or	al,0xe0
	mov	dx,IDE_REG_DEVICE
	out	dx,al

	mov	al,IDE_CMD_READ_VERIFY_SECTORS

.write_ide_command
	mov	dx,IDE_REG_COMMAND
	out	dx,al

	pop	edi
	pop	esi
	pop	edx
	pop	ebx
	pop	eax

	VJMP	jback




BP_OFS_IOCTL_ARG_DEVICE		equ	8
BP_OFS_IOCTL_ARG_PACKET		equ	0x0c

IOCTL_CMD_LBA48_ACCESS		equ	0xcafebabe
IOCTL_LBA48_SUBCMD_GET_INFO	equ	0x0

IOCTL_ARG_OFS_CMD_PACKET		equ	0x5c
IOCTL_ARG_OFS_OUT_PTR		equ	0x30
IOCTL_ARG_OFS_OUT_ACTUAL		equ	0x14

IOCTL_CMD_OFS_COMMAND		equ	0x10
IOCTL_CMD_OFS_OUT_BUFLEN		equ	0x4
IOCTL_CMD_OFS_IN_BUFLEN		equ	0x0c
IOCTL_CMD_OFS_IN_PTR		equ	0x8



lba48_DiskIoControl_hook:
	cmp	ecx,IOCTL_CMD_LBA48_ACCESS
	je	.got_my_ioctl_code
	sub	ecx,0x4d028
	VJMP	jback_cont

.got_my_ioctl_code:
	push	edi
	
;
;when we get here:
;[ebp+BP_OFS_IOCTL_ARG_DEVICE]	device struct?
;[ebp+BP_OFS_IOCTL_ARG_PACKET]	ioctl arg struct
;[ioctl struct+0x5c]			ioctl transfer packet (in data/len, out data/len, command)
;
	push	esi

	mov	eax,[ebp+BP_OFS_IOCTL_ARG_PACKET]
	mov	esi,[eax+IOCTL_ARG_OFS_CMD_PACKET]

	cmp	dword [esi+IOCTL_CMD_OFS_IN_BUFLEN],4
	jnb	short .inbuf_big_enough

	mov	eax,0xc0000004	;input length wrong
	jmp	short .exit

.inbuf_big_enough:
	cmp	dword [esi+IOCTL_CMD_OFS_OUT_BUFLEN],28
	jnb	short .outbuf_big_enough

	mov	eax,0xc0000023	;buffer too small
	jmp	short .exit

.outbuf_big_enough:
	mov	edi,[esi+IOCTL_CMD_OFS_IN_PTR]
	cmp	dword [edi],IOCTL_LBA48_SUBCMD_GET_INFO
	jz	short .get_info_cmd

	mov	eax,0xc0000010	;invalid request
	jmp	short .exit

.get_info_cmd:
	mov	edi,[eax+IOCTL_ARG_OFS_OUT_PTR]
	mov	dword [eax+IOCTL_ARG_OFS_OUT_ACTUAL],28
	
	mov	dword [edi],0xcafebabe
	mov	dword [edi+4],0xbabeface
	mov	dword [edi+8],LBA48_PATCHCODE_VERSION
; 	mov	dword [edi+12],LOWCODE_BASE
; 	mov	dword [edi+16],HIGHCODE_BASE
; 	mov	dword [edi+20],PATCHSEG_SIZE
; 	mov	dword [edi+24],(HIGHCODE_Partition_table-HIGHCODE_BASE)
.codeb:	mov	eax,0
	mov	dword [edi+12],eax
	mov	dword [edi+16],eax
	and	dword [edi+20],byte 0
	mov	dword [edi+24],lba48_partition_table - lba48_patch

	xor	eax,eax

.exit:
	pop	esi
	mov	edi,eax

	VJMP	jback



;;; --------------------------------------------------------------------------
;;; Read/write variables
;;; --------------------------------------------------------------------------

align 4

lba48_partition_table:	times 512 db 0



align 16

LBA48_SIZE	equ $-lba48_patch

%else 	; !LBA48

LBA48_SIZE	equ 0

%endif	; !LBA48
