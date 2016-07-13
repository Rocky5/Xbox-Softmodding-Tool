;;;
;;; nkpatcher.asm
;;;
;;; Copyright 2004 rmenhal
;;;
;;; Licensed under GNU General Public License version 2. See the file COPYING
;;; for details.
;;;
;;; Originally nkpatcher patches were adapted and expanded from Complex !Loader
;;; 4034 patches (which are from EvoX 2.2 bios). Some patches were taken from
;;; EvoX M7. By now (nkpatcher7) the patching has been quite heavily modified
;;; and features go well past those of EvoX M7.
;;;
;;; Supports kernels 3944, 4034, 4817, 5101, 5530, 5713 and 5838.
;;;

%include "config.inc"
%include "kexports.inc"
%include "nkpatcher.inc"


%ifndef INCLUDE_MODE

	BITS 32

%define	CODE_SECTION section .text
%define	DATA_SECTION section .data
%define	BSS_SECTION section .bss

global nkpatcher_patch_kernel

%endif	; INCLUDE_MODE
	


;;; --------------------------------------------------------------------------
;;; Helper macros and functions for patch installers
;;; --------------------------------------------------------------------------

%macro	VCALL 1
	db 0E8h
.var_%1:db 0,0,0,0
%endmacro

	
%macro	VJMP 1
	db 0E9h
.var_%1:db 0,0,0,0
%endmacro


%macro	VPUSH 1
	db 68h
.var_%1:db 0,0,0,0
%endmacro


%macro	STOS_CONST 1
%if ((%1) / 4) < 6
	times ((%1) / 4) stosd
%else
  %if ((%1) / 4) > 127
	mov	ecx,(%1) / 4
  %else
	push	byte ((%1) / 4)
	pop	ecx
  %endif
	rep	stosd
%endif
	times ((%1) % 4) stosb
%endmacro


%macro	MOVS_CONST 1
%if ((%1) / 4) < 6
	times ((%1) / 4) movsd
%else
  %if ((%1) / 4) > 127
	mov	ecx,(%1) / 4
  %else
	push	byte ((%1) / 4)
	pop	ecx
  %endif
	rep	movsd
%endif
	times ((%1) % 4) movsb
%endmacro




%macro	setcalljmptarget 4
	sub	%1,%4
	sub	%1,%3-%2+4
	mov	[%3],%1
%endmacro


%macro	setcalljmptarget 3
	sub	%1,%2
	sub	%1,byte 5
	mov	[%2+%3+1],%1
%endmacro

%macro	setjcondneartarget 3
	sub	%1,%2
	sub	%1,byte 6
	mov	[%2+%3+2],%1
%endmacro



%define TUNE_GEN		0
%define TUNE_REL		1
%define TUNE_CALL	1
%define TUNE_JMP		1
%define TUNE_ABS		2
%define TUNE_PUSH	2
%define TUNE_MOVREG	2
%define TUNE_END		0xffffffff


%macro	set_exp_entry 4
	dd %1, KERNEL_EXPORTS.%3, %2.var_%3_%4
%endmacro

	
%macro	set_exp_entry_last 0
	dd TUNE_END, 0, 0
%endmacro


tune_exp_refs:
.loop:	
	mov	eax,[esi]
	cmp	eax,byte -1
	je	.done
	cmp	al,TUNE_REL
	je	.tune_rel
	cmp	al,TUNE_ABS
	je	.tune_abs
.next:	
	add	esi,byte 12
	jmp	short .loop
.done:
	ret

.tune_abs:	
	mov	eax,[esi+4]
	mov	edx,[esi+8]
	mov	eax,[ebx+eax]
	add	eax,80010000h
	mov	[edx],eax
	jmp	short .next

.tune_rel:
	mov	eax,[esi+4]
	mov	edx,[esi+8]
	mov	eax,[ebx+eax]
	add	eax,80010000h
	add	eax,ecx
	sub	eax,edx
	sub	eax,edi
	sub	eax,byte 4
	mov	[edx],eax
	jmp	short .next


;;; --------------------------------------------------------------------------
;;; Helpers end
;;; --------------------------------------------------------------------------

	

;;; --------------------------------------------------------------------------
;;; Non-sticky reset-on-eject
;;; --------------------------------------------------------------------------


%macro	stickyroe 5
	mov	eax,%1
	mov	ebx,%2
	mov	ecx,%3
	mov	edx,%4
	mov	esi,%5
	call	installstickyroe
%endmacro


%define STICKYROE_SIZE	(patchstickyroe.end-patchstickyroe)

	
;;; Patch tuner

	CODE_SECTION
	
installstickyroe:
	mov	edi,[ldr2_space]

	setcalljmptarget eax, patchstickyroe, patchstickyroe.var_jback, edi
	mov	[patchstickyroe.bootfl+1],ebx
	mov	[patchstickyroe.qboot+2],ecx
	setcalljmptarget edx, patchstickyroe, patchstickyroe.var_HalWriteSMBusValue_1, edi

	mov	eax,[memdiff]
	mov	byte [esi+eax],0C3h ; patch out HalEnableSecureTrayEject
	
	mov	esi,patchstickyroe
	push	byte STICKYROE_SIZE
	pop	ecx
	add	edi,eax
	rep	movsb
	sub	edi,eax

	mov	[ldr2_space],edi
	ret



;;; Actual patch

	DATA_SECTION

patchstickyroe:
.bootfl:mov	esi,8003C138h
	mov	eax,[esi]
	xor	edi,edi

.qboot:	cmp	byte [0x8003b87c],0x0
	jne	.decideroe
	test	al,0x8
	jne	.skroedsbl

	push	eax
.retry:	push	byte 1
	push	edi
	push	byte 0x19
	push	byte 0x20
	VCALL	HalWriteSMBusValue_1
	test	eax,eax
	jl	.retry
	pop	eax
.skroedsbl:

	
.decideroe:
	and	eax,byte -0x9
	cmp	byte [ebp+0xc],0x0
	jnz	.roeoff
	test	al,0x4
	jnz	.roeoff
	mov	edx,[10118h]
	test	byte [edx+0x9c+3],80h
	jz	.setflags

.roeoff:or	eax,byte +0x8
.setflags:	
	mov	[esi],eax

	xor	esi,esi
	VJMP	jback

.end:	

	

;;; --------------------------------------------------------------------------
;;; Non-sticky reset-on-eject patching end
;;; --------------------------------------------------------------------------



;;; --------------------------------------------------------------------------
;;; Media patching.
;;;
;;; Assumes non-sticky ROE has been / will be applied.
;;; Patches out signature check, section hash check, media and region checks.
;;; --------------------------------------------------------------------------


%macro	media 5
%ifdef GAME_REGION_AUTO
	mov	eax,%4
%ifndef GAME_REGION_IGNORE_REAL
	mov	esi,%5+2
%endif
%endif
	mov	ecx,%1
%ifndef EJECT_FIX
	mov	edx,%3
%endif
	mov	ebx,%2
	call	installmedia
%endmacro

	
%define MEDIA_SIZE	(patchmedia.end-patchmedia)

%ifdef GAME_REGION_AUTO
%define REGION_SIZE	(patchregion.end-patchregion)
%else
%define REGION_SIZE	0
%endif

	
	
;;; Patch tuner

	CODE_SECTION

installmedia:	
	mov	edi,[ldr1_space]

%ifndef EJECT_FIX
	setcalljmptarget edx, patchmedia, patchmedia.var_NtQueryVolumeInformationFile_1, edi
%endif
	setcalljmptarget ecx, patchmedia, patchmedia.var_jback, edi

%ifdef GAME_REGION_AUTO
%ifndef GAME_REGION_IGNORE_REAL
	mov	[patchmedia.bootfa+1],esi
%endif
	mov	[patchmedia.region+1],eax
	mov	eax,[ldr2_space]
	push	eax	
	setcalljmptarget eax, patchmedia, patchmedia.var_regpa, edi
	pop	eax
	push	edi

	mov	esi,patchregion
	push	byte REGION_SIZE
	pop	ecx
	add	[ldr2_space],ecx
	add	eax,[memdiff]
	mov	edi,eax
	rep	movsb
	pop	edi
%endif

	mov	eax,[memdiff]
	mov	byte [eax+ebx],0EBh	; section hash patch

	mov	esi,patchmedia
	mov	ecx,MEDIA_SIZE
	add	edi,eax
	rep	movsb
	sub	edi,eax

	mov	[ldr1_space],edi
	ret



;;; Actual patch

	DATA_SECTION

patchmedia:	
	mov	eax,[10158h]
	test	eax,80000000h
	jnz	.dbgxbe
	xor	eax,05B6D40B6h
	xor	dword [10128h],0A8FC57ABh
	jmp	short .adxok
.dbgxbe:xor	eax,0EFB1F152h
	xor	dword [10128h],094859D4Bh
.adxok:	mov	[10158h],eax

	mov	esi,[10118h]

	cmp	dword [esi],1D8h
	jbe	.skmp2
	and	byte [esi+1D8h],0FEh
.skmp2:	


%ifdef GAME_REGION_AUTO

.region:mov	edx,8003B2B8h

%ifndef GAME_REGION_IGNORE_REAL

.bootfa:mov	ecx,8003B298h+2
	cmp	byte [ecx],0
	jne	.realregionok
	mov	al,[edx]
	mov	[ecx],al	; store EEPROM region to unused boot flags
.realregionok:	

%endif	; !GAME_REGION_IGNORE_REAL

	mov	eax,[esi+0A0h]
	VCALL	regpa			; call game region auto patch

%endif	; GAME_REGION_AUTO


%ifdef EJECT_FIX
	or	dword [esi+9Ch],80FFFFFFh
%else
	push	eax
	push	eax
	mov	ecx,esp
	push	eax
	push	eax
	mov	eax,esp
	push	byte 4			; 4 = FileFsDeviceInformation
	push	byte 8
	push	ecx
	push	eax
	push	dword [ebp-8]
	VCALL	NtQueryVolumeInformationFile_1
	test	eax,eax
	pop	eax
	pop	eax
	pop	ecx
	pop	eax
	jl	.noroe
	mov	eax,00FFFFFFh
	cmp	ecx,byte 2
	je	.smfl
.noroe:	mov	eax,80FFFFFFh		; No reset-on-eject for hard disk
.smfl:	
%ifdef CD_DVD_ALWAYS_ROE
	mov	[esi+9Ch],eax
%else
	or	[esi+9Ch],eax
%endif
%endif	; !EJECT_FIX

	VJMP	jback

.end:	



%ifdef GAME_REGION_AUTO

patchregion:
%ifndef GAME_REGION_IGNORE_REAL

	movzx	ecx,byte [ecx]
	test	al,cl
	jz	.srp
.sfall:	push	ecx
	jmp	short .changeregion

%endif	; GAME_REGION_IGNORE_REAL
	
.srp:	test	al,byte GAME_REGION_PRIMARY
	jz	.grs
%ifdef GAME_REGION_IGNORE_REAL
.sfall:
%endif
	push	byte GAME_REGION_PRIMARY
	jmp	short .changeregion

.grs:	test	al,byte GAME_REGION_SECONDARY
	jz	.grt
	push	byte GAME_REGION_SECONDARY
	jmp	short .changeregion

.grt:	test	al,byte GAME_REGION_TERTIARY
	jz	.sfall
	push	byte GAME_REGION_TERTIARY

.changeregion:
	pop	dword [edx]
	ret

.end:	

%endif 	; GAME_REGION_AUTO


;;; --------------------------------------------------------------------------
;;; Media patching end
;;; --------------------------------------------------------------------------




;;; --------------------------------------------------------------------------
;;; Fix some XBE loader jump targets for media patch.
;;;
;;; Only required for kernels 5713, 5838. Must be applied *before* media patch.
;;; --------------------------------------------------------------------------

	
%macro	mptargetfix 2
	mov	edx,%1
	mov	eax,%2
	call	installmptargetfix
%endmacro



%define MPTARGETFIX_SIZE	(patchmptargetfix.end-patchmptargetfix)
	

;;; Patch tuner

	CODE_SECTION

installmptargetfix:	
	mov	edi,[ldr1_space]

	push	eax
	setcalljmptarget eax, patchmptargetfix, patchmptargetfix.var_jerr7b, edi
	pop	eax
	setcalljmptarget eax, patchmptargetfix, patchmptargetfix.var_jerr3a, edi

	mov	eax,[memdiff]
	lea	ecx,[edi+patchmptargetfix.err3a-patchmptargetfix]
	setjcondneartarget ecx, edx, eax
	
	mov	esi,patchmptargetfix
	push	byte MPTARGETFIX_SIZE
	pop	ecx
	add	edi,eax
	rep	movsb
	sub	edi,eax

	mov	[ldr1_space],edi
	ret



;;; Actual patch

	DATA_SECTION

patchmptargetfix:
	mov	edx,[10108h]
	mov	ecx,10000h
	add	edx,ecx
	mov	eax,[10118h]
	cmp	eax,ecx
	jb	.err7b
	add	eax,1D0h
	cmp	eax,edx
	ja	.err7b
	mov	eax,[10120h]
	cmp	eax,ecx
	jb	.err7b
	imul	ecx,dword [1011Ch],byte 38h
	add	ecx,eax
	cmp	ecx,edx
	jbe	.end
.err7b:
	mov	esi,0C000007Bh
	VJMP	jerr7b
.err3a:	
	mov	esi,0C000003Ah
	VJMP	jerr3a

.end:


;;; --------------------------------------------------------------------------
;;; Target fix end
;;; --------------------------------------------------------------------------

	
	
;;; --------------------------------------------------------------------------
;;; Partition 6 patching. Used only if LBA48 patch is disabled.
;;;
;;; Assumes media patch (uses that free space)
;;; --------------------------------------------------------------------------

	
%macro	part6 4
%ifndef LBA48
	mov	ebx,%1
	mov	ecx,%2
	mov	edx,%3
	mov	eax,%4
	call	installpart6
%endif
%endmacro



%ifndef LBA48

%define PART6_SIZE	(patchpart6.end-patchpart6)
	

;;; Patch tuner

	CODE_SECTION
	
installpart6:
	mov	edi,[sign_space]
	
	mov	[patchpart6.rsize+1],eax
	setcalljmptarget ecx, patchpart6, patchpart6.var_jfail, edi
	setcalljmptarget edx, patchpart6, patchpart6.var_jok, edi

	mov	edx,edi

	mov	eax,[memdiff]
	mov	esi,patchpart6
	push	byte PART6_SIZE
	pop	ecx
	add	edi,eax
	rep	movsb
	sub	edi,eax

	mov	byte [ebx+eax],0E9h	; jmp near
	setcalljmptarget edx, ebx, eax

	mov	[sign_space],edi
	ret



;;; Actual patch

	DATA_SECTION
	
patchpart6:
	cmp	ebx,byte 6
	je	.rsize
.fail:	mov	eax,0C0000034h
	VJMP	jfail
.rsize:	mov	eax,[8003B438h]
	sub	eax,0EE8AB0h
	jbe	.fail
	mov	edx,512
	mul	edx
	mov	[ebp-20h],eax
	mov	[ebp-1Ch],edx
	mov	eax,0EE8AB0h
	VJMP	jok

.end:


%else	; LBA48

%define PART6_SIZE	0

%endif	; LBA48
	

;;; --------------------------------------------------------------------------
;;; Partition 6 patching end
;;; --------------------------------------------------------------------------




;;; --------------------------------------------------------------------------
;;; Dashboard string patching
;;;
;;; Assumes media patch (uses that free space)
;;; --------------------------------------------------------------------------

	
%macro	bootdash 1
	mov	ebx,%1
	call	patchdashstring
%endmacro


	DATA_SECTION
				
dashstr		db DASHBOARD_PATH,0
DASHSTRLEN	equ $-dashstr


%define BOOTDASH_SIZE	DASHSTRLEN


	CODE_SECTION
	
patchdashstring:
	mov	edi,[sign_space]

	mov	eax,[memdiff]
	push	edi
	mov	esi,dashstr
	push	byte DASHSTRLEN
	pop	ecx
	add	edi,eax
	rep	movsb
	sub	edi,eax
	pop	dword [ebx+eax]
	
	mov	[sign_space],edi
	ret



;;; --------------------------------------------------------------------------
;;; Dash string patching end
;;; --------------------------------------------------------------------------



;;; --------------------------------------------------------------------------
;;; In-Game Reset
;;;
;;; Assumes media patch (uses that free space)
;;; --------------------------------------------------------------------------

	
%macro	igr 7
%ifdef IGR
	mov	eax,%1
	mov	ebx,%2
	mov	ecx,%3
%ifdef IGR_TO_DASHBOARD
%ifdef IGR_FULL_REBOOT
	mov	edx,%4
%else
	mov	edx,%5
	mov	esi,%6
	mov	ebp,%7
%endif
%endif
	call	installigr
%endif
%endmacro

	
%ifdef IGR


%define IGR_SIZE		(patchigr.end-patchigr)


	CODE_SECTION
	
installigr:
	mov	edi,[sign_space]

	setcalljmptarget ebx, patchigr, patchigr.var_KeRaiseIrqlToDpcLevel_1, edi
	setcalljmptarget ecx, patchigr, patchigr.var_HalReturnToFirmware_1, edi

%ifdef IGR_TO_DASHBOARD
%ifdef IGR_FULL_REBOOT
	setcalljmptarget edx, patchigr, patchigr.var_HalWriteSMBusValue_1, edi
%else
	mov	[patchigr.ldatp+1],edx
	setcalljmptarget esi, patchigr, patchigr.var_MmAllocateContiguousMemory_1, edi
	setcalljmptarget ebp, patchigr, patchigr.var_MmPersistContiguousMemory_1, edi
%endif
%endif

	mov	ebx,[memdiff]	
	lea	edx,[edi-80010000h]
	mov	[eax+ebx],edx

	lea	eax,[edi+patchigr.getstateret-patchigr]
	mov	[patchigr.gsradx+4],eax

	mov	esi,patchigr
	mov	ecx,IGR_SIZE
	add	edi,ebx
	rep	movsb
	sub	edi,ebx
			
	mov	[sign_space],edi
	ret



	DATA_SECTION

patchigr:
	mov	eax,[esp]
	cmp	eax,80000000h
	jae	.realf
	mov	edx,[eax]
	mov	dh,0
	cmp	edx,0C24008Bh
	jne	.realf
; 	cmp	byte [eax+4],8Bh
; 	jne	.realf
	cmp	dword [eax+6],0A3h
	jne	.realf

	sub	esp,byte 12
	push	esi
	push	edi
	lea	esi,[esp+12+8]
	lea	edi,[esp+8]
	push	byte 6
	pop	ecx
	rep	movsd
	pop	edi
	pop	esi

.gsradx:mov	dword [esp+12],0
	
.realf:
	VJMP	KeRaiseIrqlToDpcLevel_1	; jump to real KeRaiseIrqlToDpcLevel


.getstateret:
	test	eax,eax
	jnz	.gsro

%ifdef IGR_MASTER_PORT
	mov	eax,[esp+4]
	mov	eax,[eax]
	mov	eax,[eax]
	cmp	dword [eax+20],byte IGR_MASTER_PORT
	jne	.norst
%endif	; IGR_MASTER_PORT
	
	mov	eax,[esp+8]
	cmp	word [eax+4+8],0FFFFh
	jne	.norst
	test	byte [eax+4],20h
	jnz	.checkaltrst
	test	byte [eax+4],10h
	jz	.norst
	cmp	byte [eax+4+6],0
	jne	short .doboot
.norst:		
	xor	eax,eax
.gsro:	ret	8

.checkaltrst:	
	test	byte [eax+4],10h
	jnz	.bootdash
	cmp	byte [eax+4+6],0
	je	.norst


.bootdash:

%ifdef IGR_FULL_REBOOT
%ifdef IGR_TO_DASHBOARD
	push	byte 0Ch
	push	byte 0
	push	byte 1Bh
	push	byte 20h
	VCALL	HalWriteSMBusValue_1
%endif	; IGR_TO_DASHBOARD

.doboot:	
	push	byte 1

%else	; !IGR_FULL_REBOOT, do quick reboot

%ifdef IGR_TO_DASHBOARD
.ldatp:	mov	esi,0			; LaunchDataPage address
	mov	edi,1000h
	mov	ebx,[esi]
	test	ebx,ebx
	jnz	.memok

	push	edi
	VCALL	MmAllocateContiguousMemory_1
	test	eax,eax
	jz	.doboot
	mov	ebx,eax
	mov	[esi],eax
.memok:	

	push	byte 1
	push	edi
	push	ebx
	VCALL	MmPersistContiguousMemory_1

	mov	edi,ebx
	xor	eax,eax
	mov	ecx,400h
	rep	stosd

	or	dword [ebx],byte -1
%endif	; IGR_TO_DASHBOARD
	
.doboot:
	push	byte 2

%endif	; !IGR_FULL_REBOOT

	VCALL	HalReturnToFirmware_1
	

.end:


%else	; !IGR

%define IGR_SIZE		0

%endif	; !IGR

		

;;; --------------------------------------------------------------------------
;;; In-Game Reset patching end
;;; --------------------------------------------------------------------------



;;; --------------------------------------------------------------------------
;;; Disable Macrovision protection
;;; --------------------------------------------------------------------------

%macro	macrovision 2
	mov	eax,[memdiff]
	mov	edi,%1
	mov	ecx,%2
	mov	dword [eax+edi],ecx
	mov	dword [eax+edi+9*4],ecx
%endmacro

;;; --------------------------------------------------------------------------
;;; Macrovision patching end
;;; --------------------------------------------------------------------------



	
;;; --------------------------------------------------------------------------
;;; Xbox Live blocking
;;;
;;; Assumes Macrovision patch (uses that free space).
;;; --------------------------------------------------------------------------

%macro	xblblock 2
%ifdef XBL_BLOCK
	mov	eax,%1
	mov	ebx,%2
	call	installxblblock
%endif
%endmacro


%ifdef XBL_BLOCK
	
%define XBL_BLOCK_SIZE	(patchxblblock.end-patchxblblock)


	CODE_SECTION
	
installxblblock:
	mov	edi,[mvis_space]

	mov	edx,[memdiff]

	mov	byte [eax+edx],0E9h
	push	edi
	setcalljmptarget edi, eax, edx
	pop	edi
	
	mov	[patchxblblock.lnkst+1],ebx

	lea	eax,[edi+patchxblblock.xblsecstr-patchxblblock]
	mov	[patchxblblock.secso+1],eax
		
	mov	esi,patchxblblock
	mov	ecx,XBL_BLOCK_SIZE
	add	edi,edx
	rep	movsb
	sub	edi,edx
	
	mov	[mvis_space],edi
	ret



	DATA_SECTION

patchxblblock:
	mov	edx,[esp]
	cmp	edx,80000000h
	jae	.gentail

	push	esi
	push	edi
	push	ebx

	mov	ebx,[10120h]
	mov	eax,[1011Ch]
.secsearchl:
	mov	esi,[ebx+14h]
	test	esi,esi
	jz	.nxsec
.secso:	mov	edi,.xblsecstr
	push	byte XBLSECSTRLEN
	pop	ecx
	rep	cmpsb
	je	.secfound
.nxsec:	add	ebx,byte 38h
	dec	eax
	jnz	.secsearchl

	pop	ebx

.noblock:	
	pop	edi
	pop	esi
.gentail:	
.lnkst:	mov	eax,[0]
	ret	4

.secfound:
	mov	esi,[ebx+4]
	mov	edi,[ebx+8]
	add	edi,esi
	pop	ebx
	
	cmp	edx,esi
	jb	.chkouter
	cmp	edx,edi
	jb	.doblock

.chkouter:	
	cmp	word [edx],0CB8Ah
	jne	.noblock

	push	byte 28h
	pop	eax
.xnetsl:
	cmp	dword [edx],byte 20h
	jne	.nm
	cmp	word [edx-2],0A164h
	je	.xnetglsfound
.nm:	dec	edx
	dec	eax
	jnz	.xnetsl
	jmp	short .noblock

.xnetglsfound:
	mov	eax,[esp+16+8]
	cmp	eax,esi
	jb	.noblock
	cmp	eax,edi
	jae	.noblock

.doblock:
	pop	edi
	pop	esi
	xor	eax,eax
	ret	4


.xblsecstr	db 'XONLINE',0
XBLSECSTRLEN	equ $-.xblsecstr

.end:


%else	; !XBL_BLOCK

%define XBL_BLOCK_SIZE	0
	
%endif	; !XBL_BLOCK


;;; --------------------------------------------------------------------------
;;; XBL blocking end
;;; --------------------------------------------------------------------------

;;; --------------------------------------------------------------------------
;;; VIDEO patching
;;;
;;; Author: Krists Krilovs <pow@pow.za.net>
;;; --------------------------------------------------------------------------

%macro	videorgb 1
%ifdef VIDEO_FORCE_RGB
	mov	eax,%1
	call	installvideorgb
%endif
%endmacro


%ifdef VIDEO_FORCE_RGB
	
%define VIDEO_FORCE_RGB_SIZE	(patchvideorgb.end-patchvideorgb)


	CODE_SECTION
	
installvideorgb:
	mov	edi,[mvis_space]
	mov	edx,[memdiff]
	mov	ebx,[kexports]

	mov	ebx,[ebx+edx+KERNEL_EXPORTS.HalBootSMCVideoMode]
	cmp	byte [ebx+edx+80010000h],01h
	jnz	.done ; not using hdtv cable

	mov	byte [eax+edx],0E9h
	push	edi
	setcalljmptarget edi, eax, edx
	pop	edi

	mov	esi,patchvideorgb
	mov	ecx,VIDEO_FORCE_RGB_SIZE
	add	edi,edx
	rep	movsb
	sub	edi,edx
	
	mov	[mvis_space],edi
.done:	ret



	DATA_SECTION

patchvideorgb:
	xor	eax,eax
	mov	[esi+680630h],eax ; change to rgb
	mov	[esi+6808C4h],eax
	mov	[esi+68084Ch],eax
	pop	esi
	pop	ebx
	leave
	retn	18h
.end:


%else	; !VIDEO_FORCE_RGB

%define VIDEO_FORCE_RGB_SIZE	0
	
%endif	; !VIDEO_FORCE_RGB



%macro	videoprogr 0
%ifdef VIDEO_FORCE_PROGRESSIVE
	call	installvideoprogr
%endif
%endmacro


%ifdef VIDEO_FORCE_PROGRESSIVE
	
%define VIDEO_FORCE_PROGRESSIVE_SIZE	(patchvideoprogr.end-patchvideoprogr)


	CODE_SECTION
	
installvideoprogr:
	mov	edi,[mvis_space]
	mov	edx,[memdiff]
	mov	ebx,[kexports]

	mov	eax,[ebx+edx+KERNEL_EXPORTS.HalBootSMCVideoMode]
	cmp	byte [eax+edx+80010000h],01h
	jnz	.done ; not using hdtv cable

	mov	eax,[ebx+edx+KERNEL_EXPORTS.AvSetDisplayMode]
	add	eax,80010000h

	mov	byte [eax+edx],0E9h
	push	edi
	setcalljmptarget edi, eax, edx
	pop	edi

	add	eax,5
	mov	cl,[eax+edx] ; frame size
	mov	[patchvideoprogr.frame+2],cl
	mov	byte [eax+edx],90h ; nop
	setcalljmptarget eax, patchvideoprogr, patchvideoprogr.var_jback, edi

	mov	esi,patchvideoprogr
	mov	ecx,VIDEO_FORCE_PROGRESSIVE_SIZE
	add	edi,edx
	rep	movsb
	sub	edi,edx
	
	mov	[mvis_space],edi
.done:	ret



	DATA_SECTION

patchvideoprogr:
	push	ebp
	mov	ebp,esp
	push	eax
	mov	eax,[ebp+16]
	or	eax,eax
	js	.cont ; hdtv mode
	jz	.640 ; default mode (?)
	test	ah,01h
	mov	eax,88080801h ; 720x480p
	jz	.chng
.640:	mov	eax,88070701h ; 640x480p
.chng:	mov	[ebp+16],eax
.cont:	pop	eax
.frame:	sub	esp,1Ch
	VJMP	jback
.end:


%else	; !VIDEO_FORCE_PROGRESSIVE

%define VIDEO_FORCE_PROGRESSIVE_SIZE	0
	
%endif	; !VIDEO_FORCE_PROGRESSIVE
;;; --------------------------------------------------------------------------
;;; VIDEO end
;;; --------------------------------------------------------------------------	
		
;;; --------------------------------------------------------------------------
;;; EvoX M7 leftover patches
;;; --------------------------------------------------------------------------

%macro	m7extra 2
%ifdef INIT_SEC_PATCHES

	mov	eax,[memdiff]
	;; HD locking check bypass (?). No importance for nkpatcher and
	;; cannot be enabled anyway because inside INIT section.
	
	mov	word [eax+%1],9090h

	;; DVD drive check bypass (?). No importance for nkpatcher and
	;; cannot be enabled anyway because inside INIT section.

	mov	byte [eax+%2],0EBh

%endif	; INIT_SEC_PATCHES
%endmacro
	
;;; --------------------------------------------------------------------------
;;; EvoX M7 leftovers end
;;; --------------------------------------------------------------------------



;;; --------------------------------------------------------------------------
;;; Virtual C drive patch
;;; --------------------------------------------------------------------------

%include "diskimagedrv.asm"

;;; --------------------------------------------------------------------------
;;; Virtual C patch end
;;; --------------------------------------------------------------------------


;;; --------------------------------------------------------------------------
;;; Virtual CD/DVD drive patch
;;; --------------------------------------------------------------------------

%include "cdromimagedrv.asm"

;;; --------------------------------------------------------------------------
;;; Virtual CD/DVD end
;;; --------------------------------------------------------------------------

	
;;; --------------------------------------------------------------------------
;;; LBA48 patch
;;; --------------------------------------------------------------------------

%include "lba48.asm"

;;; --------------------------------------------------------------------------
;;; LBA48 end
;;; --------------------------------------------------------------------------
	

;;; --------------------------------------------------------------------------
;;; Virtual EEPROM patch
;;; --------------------------------------------------------------------------

%include "virteeprom.asm"

;;; --------------------------------------------------------------------------
;;; Virtual EEPROM end
;;; --------------------------------------------------------------------------
	

	
;;; --------------------------------------------------------------------------
;;; Definitions for available free space in the kernel
;;; --------------------------------------------------------------------------
		
%define SPACE_LDR1_3944_START	80030262h
%define SPACE_LDR1_3944_END	80030309h
%define SPACE_LDR2_3944_START	800303C5h
%define SPACE_LDR2_3944_END	8003043Bh
%define SPACE_SIGN_3944_START	8002FE6Ah
%define SPACE_SIGN_3944_END	8002FF9Eh
%define SPACE_MVIS_3944_START	80031192h
%define SPACE_MVIS_3944_END	80031270h

%define SPACE_LDR1_4034_START	80030262h
%define SPACE_LDR1_4034_END	8003033Ch
%define SPACE_LDR2_4034_START	800303F8h
%define SPACE_LDR2_4034_END	8003046Eh
%define SPACE_SIGN_4034_START	8002FE6Ah
%define SPACE_SIGN_4034_END	8002FF9Eh
%define SPACE_MVIS_4034_START	80030DCFh
%define SPACE_MVIS_4034_END	80030EADh

%define SPACE_LDR1_4817_START	8002F049h
%define SPACE_LDR1_4817_END	8002F141h
%define SPACE_LDR2_4817_START	8002F1FDh
%define SPACE_LDR2_4817_END	8002F273h
%define SPACE_SIGN_4817_START	8002EC4Ah
%define SPACE_SIGN_4817_END	8002ED85h
%define SPACE_MVIS_4817_START	8002FA3Fh
%define SPACE_MVIS_4817_END	8002FC7Dh

%define SPACE_LDR1_5101_START	8002F089h
%define SPACE_LDR1_5101_END	8002F181h
%define SPACE_LDR2_5101_START	8002F23Dh
%define SPACE_LDR2_5101_END	8002F2B3h
%define SPACE_SIGN_5101_START	8002EC8Ah
%define SPACE_SIGN_5101_END	8002EDC5h
%define SPACE_MVIS_5101_START	8002FA7Fh
%define SPACE_MVIS_5101_END	8002FD13h

%define SPACE_LDR1_5530_START	8002F456h
%define SPACE_LDR1_5530_END	8002F54Eh
%define SPACE_LDR2_5530_START	8002F60Ah
%define SPACE_LDR2_5530_END	8002F680h
%define SPACE_SIGN_5530_START	8002F057h
%define SPACE_SIGN_5530_END	8002F192h
%define SPACE_MVIS_5530_START	80030255h
%define SPACE_MVIS_5530_END	800305AEh

%define SPACE_LDR1_5713_START	8002F40Dh
%define SPACE_LDR1_5713_END	8002F574h
%define SPACE_LDR2_5713_START	8002F62Ah
%define SPACE_LDR2_5713_END	8002F6A1h
%define SPACE_SIGN_5713_START	8002F057h
%define SPACE_SIGN_5713_END	8002F192h
%define SPACE_MVIS_5713_START	8003026Fh
%define SPACE_MVIS_5713_END	800305C8h

%define SPACE_LDR1_5838_START	SPACE_LDR1_5713_START
%define SPACE_LDR1_5838_END	SPACE_LDR1_5713_END
%define SPACE_LDR2_5838_START	SPACE_LDR2_5713_START
%define SPACE_LDR2_5838_END	SPACE_LDR2_5713_END
%define SPACE_SIGN_5838_START	SPACE_SIGN_5713_START
%define SPACE_SIGN_5838_END	SPACE_SIGN_5713_END
%define SPACE_MVIS_5838_START	SPACE_MVIS_5713_START
%define SPACE_MVIS_5838_END	SPACE_MVIS_5713_END

;;; --------------------------------------------------------------------------
;;; Free space definitions end
;;; --------------------------------------------------------------------------


;;; --------------------------------------------------------------------------
;;; Compile-time check that used space actually fits into the available space
;;; --------------------------------------------------------------------------

%define USEDSPACE_LDR1_USUAL	(MEDIA_SIZE)
%define USEDSPACE_LDR1_3944	USEDSPACE_LDR1_USUAL
%define USEDSPACE_LDR1_4034	USEDSPACE_LDR1_USUAL
%define USEDSPACE_LDR1_4817	USEDSPACE_LDR1_USUAL
%define USEDSPACE_LDR1_5101	USEDSPACE_LDR1_USUAL
%define USEDSPACE_LDR1_5530	USEDSPACE_LDR1_USUAL
%define USEDSPACE_LDR1_5713	(USEDSPACE_LDR1_USUAL + MPTARGETFIX_SIZE)
%define USEDSPACE_LDR1_5838	USEDSPACE_LDR1_5713

%define USEDSPACE_LDR2_USUAL	(STICKYROE_SIZE + REGION_SIZE)
%define USEDSPACE_LDR2_3944	USEDSPACE_LDR2_USUAL
%define USEDSPACE_LDR2_4034	USEDSPACE_LDR2_USUAL
%define USEDSPACE_LDR2_4817	USEDSPACE_LDR2_USUAL
%define USEDSPACE_LDR2_5101	USEDSPACE_LDR2_USUAL
%define USEDSPACE_LDR2_5530	USEDSPACE_LDR2_USUAL
%define USEDSPACE_LDR2_5713	USEDSPACE_LDR2_USUAL
%define USEDSPACE_LDR2_5838	USEDSPACE_LDR2_5713

%define USEDSPACE_SIGN_USUAL	(PART6_SIZE + BOOTDASH_SIZE + IGR_SIZE)
%define USEDSPACE_SIGN_3944	USEDSPACE_SIGN_USUAL
%define USEDSPACE_SIGN_4034	USEDSPACE_SIGN_USUAL
%define USEDSPACE_SIGN_4817	USEDSPACE_SIGN_USUAL
%define USEDSPACE_SIGN_5101	USEDSPACE_SIGN_USUAL
%define USEDSPACE_SIGN_5530	USEDSPACE_SIGN_USUAL
%define USEDSPACE_SIGN_5713	USEDSPACE_SIGN_USUAL
%define USEDSPACE_SIGN_5838	USEDSPACE_SIGN_5713

%define USEDSPACE_MVIS_USUAL	(XBL_BLOCK_SIZE + VIDEO_FORCE_RGB_SIZE + VIDEO_FORCE_PROGRESSIVE_SIZE)
%define USEDSPACE_MVIS_3944	USEDSPACE_MVIS_USUAL
%define USEDSPACE_MVIS_4034	USEDSPACE_MVIS_USUAL
%define USEDSPACE_MVIS_4817	USEDSPACE_MVIS_USUAL
%define USEDSPACE_MVIS_5101	USEDSPACE_MVIS_USUAL
%define USEDSPACE_MVIS_5530	USEDSPACE_MVIS_USUAL
%define USEDSPACE_MVIS_5713	USEDSPACE_MVIS_USUAL
%define USEDSPACE_MVIS_5838	USEDSPACE_MVIS_5713

%define USEDSPACE_EXPD_USUAL	(DISK_IMAGE_PATCH_SIZE \
				+ CDROM_IMAGE_PATCH_SIZE \
				+ LBA48_SIZE \
				+ VIRTUAL_EEPROM_SIZE)
%define USEDSPACE_EXPD_3944	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_4034	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_4817	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5101	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5530	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5713	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5838	USEDSPACE_EXPD_5713


%macro	CHECK_SPACE 2
%if USEDSPACE_%1_%2 > (SPACE_%1_%2_END - SPACE_%1_%2_START)
	%error %1 space overrun with kernel %2 !
%endif
%endmacro


%macro	CHECK_SPACE_ALL 1
	CHECK_SPACE LDR1, %1
	CHECK_SPACE LDR2, %1
	CHECK_SPACE SIGN, %1
	CHECK_SPACE MVIS, %1
%endmacro


CHECK_SPACE_ALL 3944
CHECK_SPACE_ALL 4034
CHECK_SPACE_ALL 4817
CHECK_SPACE_ALL 5101
CHECK_SPACE_ALL 5530
CHECK_SPACE_ALL 5713
CHECK_SPACE_ALL 5838
	
;;; --------------------------------------------------------------------------
;;; Used space checking end
;;; --------------------------------------------------------------------------

	
;;; --------------------------------------------------------------------------
;;; Helper macros for patchers
;;; --------------------------------------------------------------------------


%macro	patcherinit 0
	pushad
	mov	eax,[esp+32+12]
	mov	[caller_param],eax
	mov	edx,[esp+32+8]
	push	edx
	mov	ecx,[esp+36+4]
	call	init_patcher_vars
%endmacro


%macro	patcherfinish 0
	pop	edx
	call	erasescrap
	popad
	ret	12
%endmacro


	CODE_SECTION

init_patcher_vars:
	push	ecx
	sub	ecx,80010000h
	mov	[memdiff],ecx

	call	initspace

	pop	ecx
.get_export_table:
	mov	eax,[ecx+3Ch]
	mov	eax,[ecx+eax+78h]
	mov	eax,[ecx+eax+1Ch]
	add	eax,ecx

	sub	eax,[memdiff]
	mov	[kexports],eax
	ret
	
	
	
initspace:
	mov	eax,[edx + KERNEL_INFO.ldr1_space]
	mov	[ldr1_space],eax
	mov	eax,[edx + KERNEL_INFO.ldr2_space]
	mov	[ldr2_space],eax
	mov	eax,[edx + KERNEL_INFO.sign_space]
	mov	[sign_space],eax
	mov	eax,[edx + KERNEL_INFO.mvis_space]
	mov	[mvis_space],eax
	mov	eax,[edx + KERNEL_INFO.expd_space]
	mov	[expd_space],eax
	ret
	
	
erasescrap:
	mov	ebp,[memdiff]

	mov	ecx,[edx + KERNEL_INFO.ldr1_space_end]
	mov	edi,[ldr1_space]
	sub	ecx,edi
	mov	al,0CCh
	add	edi,ebp
	rep	stosb

	mov	ecx,[edx + KERNEL_INFO.ldr2_space_end]
	mov	edi,[ldr2_space]
	sub	ecx,edi
	add	edi,ebp
	rep	stosb

	mov	ecx,[edx + KERNEL_INFO.sign_space_end]
	mov	edi,[sign_space]
	sub	ecx,edi
	add	edi,ebp
	rep	stosb

	mov	ecx,[edx + KERNEL_INFO.mvis_space_end]
	mov	edi,[mvis_space]
	sub	ecx,edi
	add	edi,ebp
	rep	stosb

	ret



;;; --------------------------------------------------------------------------
;;; Helpers end
;;; --------------------------------------------------------------------------




	DATA_SECTION
	
align 4
ldr1_space	dd 0
ldr2_space	dd 0
sign_space	dd 0
mvis_space	dd 0
expd_space	dd 0

kexports	dd 0
memdiff		dd 0
caller_param	dd 0		; feature parameters


struc			KERNEL_INFO
.version:		resd 1
.patcher:		resd 1
.bootflags_addr:		resd 1
.top_var_addr:		resd 1
.expansion_size:		resd 1
.kernel_move_saved_data:	resd 1

.ldr1_space:		resd 1
.ldr2_space:		resd 1
.sign_space:		resd 1
.mvis_space:		resd 1
.expd_space:		resd 1
.ldr1_space_end:		resd 1
.ldr2_space_end:		resd 1
.sign_space_end:		resd 1
.mvis_space_end:		resd 1
.expd_space_end:		resd 1
endstruc


%macro	kinfo_entry 4
	dd %1, patcher_%1, %2, %3, (USEDSPACE_EXPD_%1 + 0xFFF) & ~0xFFF, %4, \
		SPACE_LDR1_%1_START, \
		SPACE_LDR2_%1_START, \
		SPACE_SIGN_%1_START, \
		SPACE_MVIS_%1_START, \
		0, \
		SPACE_LDR1_%1_END, \
		SPACE_LDR2_%1_END, \
		SPACE_SIGN_%1_END, \
		SPACE_MVIS_%1_END, \
		0
%endmacro

%macro	kinfo_entry_last 0
	dd 0
%endmacro
	
	
kernel_info_table:
	kinfo_entry 3944, 8003BE58h, 8003bff0h, 80031656h
	kinfo_entry 4034, 8003BF18h, 8003c0b0h, 80031293h
	kinfo_entry 4817, 8003B198h, 8003b330h, 8002fd41h
	kinfo_entry 5101, 8003B298h, 8003B430h, 8002FDD7h
	kinfo_entry 5530, 8003C118h, 8003c2c4h, 8002ff55h
	kinfo_entry 5713, 8003C138h, 8003c2e4h, 8002ff6fh
	kinfo_entry 5838, 8003C138h, 8003c2e4h, 8002ff6fh
	kinfo_entry_last
	

;;; Only one user allowed at a time

	CODE_SECTION

nkpatcher_get_kernel_info:
	push	dword [esp+4]
	call	.get_kernel_version

	mov	edx,eax
	mov	eax,kernel_info_table
.sl:
	mov	ecx,[eax]
	jecxz	.notfound
	cmp	ecx,edx
	je	.done
	add	eax,byte KERNEL_INFO_size
	jmp	short .sl
.notfound:
	xor	eax,eax
.done:
	ret	4


.get_kernel_version:
	push	ebx
	push	esi
	
	mov	esi,[esp+4+8]
	
	mov	eax,[esi+3Ch]
	mov	ebx,[esi+eax+78h]
	add	ebx,esi
	mov	edx,[ebx+1Ch]
	add	edx,esi

	mov	ecx,324
	sub	ecx,[ebx+10h]
	mov	eax,[edx+4*ecx]
	add	eax,esi

	movzx	eax,word [eax+4]

	pop	esi
	pop	ebx
	ret	4

	
		
	
patcher_3944:
	patcherinit
	macrovision 800315F8h,800315F1h
	stickyroe 80030451h,8003BE58h,8003759Ch,80015A63h,800161BAh
	media 80030309h,8002FD80h,80018652h,8003BE74h,8003BE58h
	part6 8002598Fh,80025A5Bh,800259D1h,8003BFF8h
	bootdash 8003049Ch
	igr 800104C8h,80015688h,800165D9h,80015A63h,8003BE70h,8001F64Ah,8001F198h
	xblblock 8002EBA4h,8003BBC4h
	m7extra 80054F27h,800552BEh
	diskimagedrv 800259D2h,8003BB60h,80025A58h,8003BFF8h
	cdromimagedrv 80027743h,80012390h,800123A4h,8003BA20h
	lba48 80025985h,80025A5Bh,800259D1h,8003BFF8h, \
	      80025605h,8002564Eh,8002547Bh,800254AFh,80025837h,80025892h
	virteeprom 8003BC00h,80016B91h
	videorgb 8003118Ah
	videoprogr
	patcherfinish


patcher_4034:
	patcherinit
	macrovision 80031235h,8003122Eh
	stickyroe 80030484h,8003BF18h,8003765Ch,80015A63h,800161ABh
	media 8003033Ch,8002FD80h,80018653h,8003BF34h,8003BF18h
	part6 800259AFh,80025A7Bh,800259F1h,8003C0B8h
	bootdash 800304D1h
	igr 800104C8h,80015688h,800165CDh,80015A63h,8003BF30h,8001F65Fh,8001F1ADh
	xblblock 8002EBABh,8003BC84h
	m7extra 800551E6h,8005558Dh
	diskimagedrv 800259F2h,8003BC20h,80025A78h,8003C0B8h
	cdromimagedrv 80027763h,80012398h,800123ACh,8003BAE0h
	lba48 800259A5h,80025A7Bh,800259F1h,8003C0B8h, \
	      80025625h,8002566Eh,8002549Bh,800254CFh,80025857h,800258B2h
	virteeprom 8003BCC0h,80016B82h
	videorgb 800317A1h
	videoprogr
	patcherfinish

	
patcher_4817:
	patcherinit
	macrovision 80030436h,8003042Fh
	stickyroe 8002F289h,8003B198h,8003A8DCh,80014723h,80014E7Ah
	media 8002F141h,8002EB60h,80017318h,8003B1B8h,8003B198h
	part6 800246DFh,800247ABh,80024721h,8003B338h
	bootdash 8002F2D6h
	igr 800104E8h,80014348h,80015299h,80014723h,8003B3A8h,8001E3ADh,8001DEFBh
	xblblock 8002D986h,8003AF04h
	m7extra 80055A66h,80055DFFh
	diskimagedrv 80024722h,8003AEA0h,800247A8h,8003B338h
	cdromimagedrv 800264E7h,800123B8h,800123CCh,8003AD60h
	lba48 800246D5h,800247ABh,80024721h,8003B338h, \
	      80024355h,8002439Eh,800241CBh,800241FFh,80024587h,800245E2h
	virteeprom 8003AF40h,80015842h
	videorgb 80030996h
	videoprogr
	patcherfinish


patcher_5101:
	patcherinit
	macrovision 800304FBh,800304F4h
	stickyroe 8002F2C9h,8003B298h,8003A9DCh,80014733h,80014E8Ah
	media 8002F181h,8002EBA0h,80017328h,8003B2B8h,8003B298h
	part6 800246EFh,800247BBh,80024731h,8003B438h
	bootdash 8002F316h
	igr 800104E8h,80014358h,800152A9h,80014733h,8003B4A8h,8001E3BDh,8001DF0Bh
	xblblock 8002D9C2h,8003B004h
	m7extra 80055A71h,80055E6Ch
	diskimagedrv 80024732h,8003AFA0h,800247b8h,8003B438h
	cdromimagedrv 80026515h,800123B8h,800123CCh,8003AE60h
	lba48 800246E5h,800247BBh,80024731h,8003B438h, \
	      80024365h,800243AEh,800241DBh,8002420Fh,80024597h,800245F2h
	virteeprom 8003B040h,80015852h
	videorgb 80030A95h
	videoprogr
	patcherfinish

	
patcher_5530:
	patcherinit
	macrovision 80030ED7h,80030ED0h
	stickyroe 8002F696h,8003C118h,8003B85Ch,80014743h,8001500Bh
	media 8002F54Eh,8002EF6Fh,8001749Ah,8003C138h,8003C118h
	part6 800248BBh,80024987h,800248FDh,8003C2D0h
	bootdash 8002F6E3h
	igr 800104E8h,80013D04h,8001542Dh,80014743h,8003C340h,8001E4D3h,8001E021h
	xblblock 8002DD91h,8003BE84h
	m7extra 800552FEh,800556F4h
	diskimagedrv 800248FEh,8003BE20h,80024984h,8003C2D0h
	cdromimagedrv 80026919h,800123B0h,800123C4h,8003BCE0h
	lba48 800248B1h,80024987h,800248FDh,8003C2D0h, \
	      80024534h,8002457Dh,800243AAh,800243DEh,80024766h,800247C1h
	virteeprom 8003BEC0h,800159D2h
	videorgb 800315B2h
	videoprogr
	patcherfinish


patcher_5713:
patcher_5838:	
	patcherinit
	macrovision 80030EF1h,80030EEAh
	stickyroe 8002F6B0h,8003C138h,8003B87Ch,80014743h,8001500Bh
	mptargetfix 8002F5D2h,8002F6B0h
	media 8002F574h,8002EF6Fh,8001749Ah,8003C158h,8003C138h
	part6 800248BBh,80024987h,800248FDh,8003C2F0h
	bootdash 8002F6FDh
	igr 800104E8h,80013D04h,8001542Dh,80014743h,8003C360h,8001E4D3h,8001E021h
	xblblock 8002DD91h,8003BEA4h
	m7extra 8005531Eh,80055714h
	diskimagedrv 800248FEh,8003BE40h,80024984h,8003C2F0h
	cdromimagedrv 80026919h,800123B0h,800123C4h,8003BD00h
	lba48 800248B1h,80024987h,800248FDh,8003C2F0h, \
	      80024534h,8002457Dh,800243AAh,800243DEh,80024766h,800247C1h
	virteeprom 8003BEE0h,800159D2h
	videorgb 800315CCh
	videoprogr
	patcherfinish
