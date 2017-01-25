;;;
;;; virteeprom.asm
;;; 
;;; Copyright 2004 rmenhal
;;; 
;;; Licensed under GNU General Public License version 2. See the file COPYING
;;; for details.
;;;
%macro	virteeprom 2
%ifdef VIRTUAL_EEPROM
	mov	eax,%1
	mov	ebx,%2
	call	installvirteeprom
%endif
%endmacro
%ifdef VIRTUAL_EEPROM
	CODE_SECTION
	align	4
installvirteeprom:
	push	eax			; save them
	push	ebx
	lea	eax,[ee_fp]		; eeprom off file name
	call	open_file		; go try to open eeprom_off file
	cmp     [esi+4],dword 01h	; status was put here by NtOpenfile (8 bytes total)
	jne	short do_ve.		; file not there !
;---------------------------------------
	push	dword [ebx]
	call	dword [NtClose]
	add	esp,4+4			; unwind stack
	ret				; were done, no virtual eeprom 
;---------------------------------------
do_ve.	pop	ebx
	pop	eax
	mov	edi,[expd_space]
	test	edi,edi
	jz	near .exit
	mov	[virteeprom_open.cache+2],eax
	mov	esi,[memdiff]
	lea	ebp,[edi+virteeprom_open-virteeprom_patch-5]
	sub	ebp,ebx
	mov	edx,[ebx+esi+1]
	mov	[ebx+esi+1],ebp
	lea	edx,[edx+ebx+5]
	setcalljmptarget edx, virteeprom_patch, virteeprom_open.var_diskstuff, edi
	mov	ebx,[kexports]
	add	ebx,esi
	mov	ecx,virteeprom_patch
	push	esi
	mov	esi,.exp_tab
	call	tune_exp_refs
	pop	esi
	mov	eax,[ebx + KERNEL_EXPORTS.HalReadSMBusValue]
	add	eax,80010000h
	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+virteeprom_HalReadSMBusValue_hook-virteeprom_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp
	mov	byte [eax+esi+5],90h ; nop
	add	eax,byte 5
	setcalljmptarget eax, virteeprom_patch, virteeprom_HalReadSMBusValue_hook.var_jback_cont, edi
	mov	eax,[ebx + KERNEL_EXPORTS.HalWriteSMBusValue]
	add	eax,80010000h
	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+virteeprom_HalWriteSMBusValue_hook-virteeprom_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp
	mov	byte [eax+esi+5],90h ; nop
	add	eax,byte 5
	setcalljmptarget eax, virteeprom_patch, virteeprom_HalWriteSMBusValue_hook.var_jback_cont, edi
	add	[virteeprom_open.sefa+1],edi
	add	[virteeprom_open.hnd+1],edi
	add	[virteeprom_HalReadSMBusValue_hook.hnd+2],edi
	add	[virteeprom_HalWriteSMBusValue_hook.hnd+2],edi
	add	[virteeprom_open.eeprom_file+4],edi
	add	[virteeprom_open.first+1],edi
	mov	ebp,esi
	mov	esi,virteeprom_patch
	mov	ecx,VIRTUAL_EEPROM_SIZE/4
	add	edi,ebp
	rep	movsd
	sub	edi,ebp
	mov	[expd_space],edi
.exit:	
	ret
.exp_tab:		
	set_exp_entry TUNE_CALL, virteeprom_open, NtOpenFile, 1
	set_exp_entry TUNE_CALL, virteeprom_HalReadSMBusValue_hook, NtReadFile, 1
%ifndef DISABLE_VIRTUAL_EEPROM_WRITE
	set_exp_entry TUNE_CALL, virteeprom_HalWriteSMBusValue_hook, NtWriteFile, 1
%endif
	set_exp_entry_last
	DATA_SECTION
align 16
virteeprom_patch:	
virteeprom_open:
	VCALL	diskstuff
	push	eax
	mov	ecx,esp
	push	eax
	push	eax
	mov	eax,esp
	push	byte 40h
.sefa:	push	(.eeprom_file - virteeprom_patch)
	push	byte 0
	mov	edx,esp
	push	byte 62h	; FILE_SYNCHRONOUS_IO_NONALERT |
				; FILE_NON_DIRECTORY_FILE |
				; FILE_WRITE_THROUGH
	push	byte 3		; FILE_SHARE_READ | FILE_SHARE_WRITE
	push	eax
	push	edx
	push	120003h		; STANDARD_RIGHTS_READ | STANDARD_RIGHTS_WRITE |
				; FILE_READ_DATA | FILE_WRITE_DATA | SYNCHRONIZE
	push	ecx
	VCALL	NtOpenFile_1
	add	esp,byte 12+8
	test	eax,eax
	pop	eax
	jge	.openok
	xor	eax,eax
.openok:
.hnd:	mov	[virteeprom_handle - virteeprom_patch],eax
.first:	mov	eax,virteeprom_first_boot - virteeprom_patch
	cmp	byte [eax],0
	jne	.skip_cache_invalidate
	mov	byte [eax],1
.cache:	and	dword [8003B040h],byte 0	; Invalidate kernel EEPROM cache so
					; so it will read our new one in.
.skip_cache_invalidate:	
	ret
.eeprom_filestr	db VIRTUAL_EEPROM_PATH, 0
.eeprom_file	dw $-.eeprom_filestr-1, $-.eeprom_filestr
		dd .eeprom_filestr-virteeprom_patch
virteeprom_HalReadSMBusValue_hook:
	push	ebp
	mov	ebp,esp
	mov	al,[ebp+8]
	and	al,~1
	cmp	al,0A8h
	je	.virtread
.goreal:
	sub	esp,byte 18h
	VJMP	jback_cont
.virtread:
.hnd:	mov	ecx,[virteeprom_handle - virteeprom_patch]
	test	ecx,ecx
	jz	.goreal
	push	esi
	push	eax
	push	eax
	mov	esi,esp
	xor	edx,edx
	mov	eax,[ebp+14h]
	mov	[eax],edx
	cmp	byte [ebp+10h],0
	setne	dl
	inc	dl
	movzx	eax,byte [ebp+0Ch]
	push	byte 0
	push	eax
	push	esp
	push	edx
	push	dword [ebp+14h]
	push	esi
	push	byte 0
	push	byte 0
	push	byte 0
	push	ecx
	VCALL	NtReadFile_1
	add	esp,byte 8+8
	pop	esi
	test	eax,eax
	mov	eax,0C0000185h	; STATUS_IO_DEVICE_ERROR
	jl	.err
	xor	eax,eax
.err:	
	leave
	ret	16
virteeprom_HalWriteSMBusValue_hook:	
	push	ebp
	mov	ebp,esp
	mov	al,[ebp+8]
	and	al,~1
	cmp	al,0A8h
	je	.virtread
.goreal:
	sub	esp,byte 18h
	VJMP	jback_cont
.virtread:
.hnd:	mov	ecx,[virteeprom_handle - virteeprom_patch]
	test	ecx,ecx
%ifdef ENABLE_REAL_EEPROM_WRITE
	jz	.goreal
%else
	jz	.exit_success
%endif
%ifndef DISABLE_VIRTUAL_EEPROM_WRITE	
	push	esi
	push	eax
	push	eax
	mov	esi,esp
	xor	edx,edx
	cmp	byte [ebp+10h],0
	setne	dl
	inc	dl
	movzx	eax,byte [ebp+0Ch]
	push	byte 0
	push	eax
	push	esp
	push	edx
	lea	eax,[ebp+14h]
	push	eax
	push	esi
	push	byte 0
	push	byte 0
	push	byte 0
	push	ecx
	VCALL	NtWriteFile_1
	add	esp,byte 8+8
	pop	esi
	test	eax,eax
	mov	eax,0C0000185h	; STATUS_IO_DEVICE_ERROR
	jl	.err
%endif ; !DISABLE_VIRTUAL_EEPROM_WRITE
.exit_success:	
	xor	eax,eax
.err:
	leave
	ret	16
;;; --------------------------------------------------------------------------
;;; Read/write variables
;;; --------------------------------------------------------------------------
align 4
virteeprom_handle	dd 0
virteeprom_first_boot	db 0
align 16
VIRTUAL_EEPROM_SIZE	equ $-virteeprom_patch
%else 	; !VIRTUAL_EEPROM
VIRTUAL_EEPROM_SIZE	equ 0
%endif	; !VIRTUAL_EEPROM