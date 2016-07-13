;;;
;;; xboxapp.asm
;;;
;;; Copyright 2004 rmenhal
;;;
;;; Licensed under GNU General Public License version 2. See the file COPYING
;;; for details.
;;;
;;;
;;; Compile using the command:
;;; 	nasm -o nkpatcher.xbe xboxapp.asm
;;;
;;; Originally nkpatcher patches were adapted and expanded from Complex !Loader
;;; 4034 patches (which are from EvoX 2.2 bios). Some patches were taken from
;;; EvoX M7. By now (nkpatcher7) the patching has been quite heavily modified
;;; and features go well past those of EvoX M7.
;;;
;;; Supports kernels 3944, 4034, 4817, 5101, 5530, 5713 and 5838.
;;;

%include "header.asm"
 
	align 4
kernel_thunk:	
AvGetSavedDataAddress:		dd	0x80000000 + 1
AvSetSavedDataAddress:		dd	0x80000000 + 4
HalReadSMBusValue:		dd	0x80000000 + 45
HalReturnToFirmware:		dd	0x80000000 + 49
HalWriteSMBusValue:		dd	0x80000000 + 50
KeQuerySystemTime:		dd	0x80000000 + 128
KeRaiseIrqlToDpcLevel:		dd	0x80000000 + 129
KfLowerIrql:			dd	0x80000000 + 161
LaunchDataPage:			dd	0x80000000 + 164
MmAllocateContiguousMemory:	dd	0x80000000 + 165
MmAllocateContiguousMemoryEx:	dd	0x80000000 + 166
MmFreeContiguousMemory:		dd	0x80000000 + 171
MmGetPhysicalAddress:		dd	0x80000000 + 173
MmPersistContiguousMemory:	dd	0x80000000 + 178
MmQueryAllocationSize:		dd	0x80000000 + 180
NtClose:			dd	0x80000000 + 187
NtCreateFile:                   dd	0x80000000 + 190
NtDeviceIoControlFile:		dd	0x80000000 + 196
NtOpenFile:			dd	0x80000000 + 202
NtReadFile:			dd	0x80000000 + 219
NtSetInformationFile            dd      0x80000000 + 226
NtSetSystemTime:		dd 	0x80000000 + 228
NtWriteFile:			dd	0x80000000 + 236
RtlTimeFieldsToTime:		dd 	0x80000000 + 304
XboxKrnlVersion:		dd	0x80000000 + 324
XePublicKeyData:		dd	0x80000000 + 355
				dd	0 ; end of table

%include "config.inc"
%include "nkpatcher.inc"

	align 4
xa_kernel_info_addr	dd 0
xa_feature_param	times FEATURE_PARAMETERS_size db 0
	align 4

start:	
%include "NKP_top.asm"			; top of nkp <<<<<<<<<<<<<<
new_start:


	cld
chkkernelversion:
	mov	esi,[XboxKrnlVersion]
	test	esi,esi
	jz	near reboot.doboot

	cmp	dword [esi],byte 1
	jne	near reboot.doboot
	cmp	word [esi+6],byte 1
	jne	near reboot.doboot

	push	80010000h
	call	nkpatcher_get_kernel_info
	test	eax,eax
	jz	near reboot.doboot
	
	mov	[xa_kernel_info_addr],eax
	mov	ebx,eax

%ifdef LBA48
	push	xa_feature_param
	call	xa_lba48_fill_parameters
%endif
	
	push	ebx
	call	expand_kernel

patchkernel:		
	call	dword [KeRaiseIrqlToDpcLevel]
	push	eax

	cli
	mov	eax,cr0
	push	eax
	and	eax,0FFFEFFFFh
	mov	cr0,eax

	mov	eax,cr3
	mov	cr3,eax
	wbinvd

	call	expand_code_segment

	call	patchmskeyback

	push	ebx
	call	patchheaders

	push	xa_feature_param
	push	ebx
	push	80010000h
	call	dword [ebx + KERNEL_INFO.patcher]

	mov	eax,cr3
	mov	cr3,eax
	wbinvd

	pop	eax
	mov	cr0,eax
	sti

	pop	ecx
	call	dword [KfLowerIrql]

;----------------------------------------
;	start of modifications by XMAN954
;
	call    near ck_clk  			; do clock check

reboot:
	mov	esi,[LaunchDataPage]
	mov	ebx,[esi]


	mov	eax,[options+4]		; reload tray options
	test	eax,128			; boot dash only mask
	jne	.bootdash		; boot the dash now


%ifndef NORMAL_BOOT_ALWAYS
	mov	eax,[xa_kernel_info_addr]
	mov	eax,[eax + KERNEL_INFO.bootflags_addr]
	test	byte [eax],80h
	jnz	.bootdash
%endif

	test	ebx,ebx
	jz	.doboot

	push	ebx
	and	dword [esi],byte 0
	call	dword [MmFreeContiguousMemory]
	jmp	short .doboot
;
;	end of modifications by XMAN954
;----------------------------------------
.bootdash:
	mov	eax,[nkp_dash+4],	; get size
	cmp     al, 1
	je      .doboot			; bail for error to DVD

	mov	edi,1000h
	test	ebx,ebx
	jnz	.memok

	push	edi
	call	dword [MmAllocateContiguousMemory]
	test	eax,eax
	jz	.doboot
	mov	ebx,eax
	mov	[esi],eax
.memok:	

	push	byte 1
	push	edi
	push	ebx
	call	dword [MmPersistContiguousMemory]

	mov	edi,ebx
	xor	eax,eax
	mov	ecx,400h
	rep	stosd

	or	dword [ebx],byte -1

.doboot:
	push	byte 2
	call	dword [HalReturnToFirmware]
.inf:	jmp	short .inf

;--------------------- subs -------------------

patchmskeyback:
	mov	eax,[XePublicKeyData]
	test	eax,eax
	jz	.fail

	mov	dword [eax+10h],10001h
	mov	dword [eax+110h],0A44B1BBDh
.fail:
	ret

expand_kernel:
.top_addr		equ 0-4
.expansion_end_addr_ex	equ .top_addr-4
.local_var_size		equ -.expansion_end_addr_ex
	
	push	ebp
	mov	ebp,esp
	sub	esp,byte .local_var_size
	
	push	ebx
	mov	ebx,[ebp+8]
	mov	eax,[ebx + KERNEL_INFO.expansion_size]
	test	eax,eax
	jz	near .done

	push	esi
	push	edi
	
	mov	edx,[ebx + KERNEL_INFO.top_var_addr]
	mov	edx,[edx]
	mov	[ebp + .top_addr],edx
	add	eax,edx
	mov	[ebp + .expansion_end_addr_ex],eax
	
	call	dword [AvGetSavedDataAddress]
	mov	edi,eax
	test	eax,eax
	jz	.expand

	push	edi
	call	dword [MmQueryAllocationSize]
	mov	esi,eax

	push	edi
	call	dword [MmGetPhysicalAddress]

	lea	edx,[eax+esi]
	cmp	[ebp + .expansion_end_addr_ex],eax
	jbe	.expand
	cmp	edx,[ebp + .top_addr]
	jbe	.expand
	
	push	404h
	push	byte 0
	push	byte -1
	push	byte 0
	push	esi
	call	dword [MmAllocateContiguousMemoryEx]
	mov	edi,eax
	test	eax,eax
	jz	.error

	push	edi
	call	dword [MmGetPhysicalAddress]

	lea	edx,[eax+esi]
	cmp	[ebp + .expansion_end_addr_ex],eax
	jbe	.movedata
	cmp	edx,[ebp + .top_addr]
	ja	.error

.movedata:	
	push	esi
	push	edi
	call	dword [ebx + KERNEL_INFO.kernel_move_saved_data]

.expand:
	mov	eax,[ebp + .expansion_end_addr_ex]
	dec	eax
	push	byte 4
	push	byte 0
	push	eax
	push	dword [ebp + .top_addr]
	push	dword [ebx + KERNEL_INFO.expansion_size]
	call	dword [MmAllocateContiguousMemoryEx]
	test	eax,eax
	jz	.error

	mov	edx,[ebx + KERNEL_INFO.top_var_addr]
	mov	ecx,[ebx + KERNEL_INFO.expansion_size]
	add	[edx],ecx

	mov	[ebx + KERNEL_INFO.expd_space],eax
		
	push	byte 1
	push	ecx
	push	eax
	call	dword [MmPersistContiguousMemory]

.error:	
	pop	edi	
	pop	esi

.done:	
	pop	ebx
	leave
	ret	4



;;; Expand code segment size too. This is required for kernels 5530 and above,
;;; because those kernels downsize code segment when they discard the INIT
;;; section. It doesn't do really anything for kernels 5101 or less.
;;;
;;; This will also make Phoenix Bios Loader 1.3 - 1.4.1 work with 5530 and 5713
;;; kernel boxes.
	
expand_code_segment:
	mov	eax,.fjmp
	push	eax
	push	eax
	push	eax
	push	eax
	sgdt	[esp+2]
	pop	eax
	pop	eax
	mov	edx,cs
	add	eax,edx
	mov	[esp+4],edx
	mov	word [eax],0FFFFh
	or	byte [eax+6],0Bh
	jmp	far [esp]
.fjmp:
	pop	eax
	pop	eax
	ret
	
	
;;; If we expanded the kernel, xboxkrnl.exe headers must be made consistent.

patchheaders:	
	mov	ecx,[esp+4]

	cmp	dword [ecx + KERNEL_INFO.expd_space],byte 0
	je	.done

	push	esi
	mov	esi,80010000h

	add	esi,[esi+3Ch]

	movzx	eax,word [esi+6]			; NumberOfSections
	movzx	edx,word [esi+14h]		; SizeOfOptionalHeader
	dec	eax
	dec	eax
	imul	eax,byte 28h
	add	esi,edx
	add	esi,eax
	add	esi,byte 18h			; Second to last section

	mov	eax,[ecx + KERNEL_INFO.expansion_size]

	add	[esi+8],eax			; VirtualSize
	add	[esi+10h],eax			; SizeOfRawData

	sub	[esi+28h+8],eax			; INIT section VirtualSize
	sub	[esi+28h+10h],eax		; INIT section SizeOfRawData
	add	[esi+28h+0Ch],eax		; INIT section VirtualAddress
	add	[esi+28h+14h],eax		; INIT section PointerToRawData

	pop	esi
.done:	
	ret	4
	
;;; --------------------------------------------------------------------------
;;; Include Xbox application LBA48 stuff
;;; --------------------------------------------------------------------------

%ifdef LBA48
%include "xa_lba48.asm"
%endif

;;; --------------------------------------------------------------------------
;;; Include patcher source
;;; --------------------------------------------------------------------------

%define INCLUDE_MODE
%define CODE_SECTION
%define DATA_SECTION

%include "nkpatcher.asm"



;;; 
;;; END OF CODE
;;; 
%include "footer.asm"
