;----------------------------------------
;       tray state stuff
;----------------------------------------
	mov     eax,tray_data1		; get tray state
	push	eax
        push    byte 0
        push    byte 3
        push    byte 20h
 	call	dword [HalReadSMBusValue]
        cmp     byte [tray_data1], 10h	; 10h = open, anything else is closed 
	jne     near go_nkp		; normal boot nkp
;----------------------------------------
%ifdef  TRI_BOOT
;----------------------------------------
	mov	eax, 0077h       	; "orange blink open for now"
	call    set_led			; go set it
;-------
	mov	eax,0x1FFFFFFF
dly_0	dec	eax
	jnz     short dly_0		; ~ 1 sec 
;----------------------------------------
	push	byte 1			; close tray
 	push	byte 0
 	push	byte 0Ch  		; tray_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]
;----------------------------------------
TS_lp:	mov	eax,0x01FFFFFF
dly_1	dec	eax
	jnz     short dly_1		; ~ 1/15 sec
;-------
	mov     eax, tray_data2		; read after tray is closed
	push	eax
        push    byte 0
        push    byte 3
        push    byte 20h
 	call	dword [HalReadSMBusValue]
;-------
        cmp     byte [tray_data2],40h	; 40h = no disk 
	je      short do_ad		; we need to alt dash
        cmp     byte [tray_data2],60h	; 60h = game disk 
	jne     short TS_lp		; loop till disk is read
;-------
 	mov	eax, 00EBh       	; Orange Red Orange Green
	call    set_led			; change it (xmas tree) and boot Cdrom
	mov	esi,[LaunchDataPage]
	mov	ebx,[esi]
	jmp	near go_cd		; boot cd
;----------------------------------------
%endif  ; for tri-boot
do_ad:  
%ifndef PATCH_BACK_MS_KEY
	mov	eax, ALTDASH_LED_COLOR
	call    set_led			; go set it
%endif
%ifdef  PATCH_BACK_MS_KEY
	mov	eax, PATCHBACK_LED_COLOR
	call    set_led	   		; go set it
;-------------------------------------------------------------------------------
; This modifies the habibi public key back to MS public key.
;-------------------------------------------------------------------------------
	mov	ebx,[XePublicKeyData]
	mov	esi,80010000h
	test	ebx,ebx
	jnz	.chk
.s_key:	mov	ebx,esi
	inc	esi
.chk:	cmp	dword [ebx],31415352h
	jne	.s_key
	cmp	dword [ebx+10h],10001h
	jne	.s_key
.sk_lp: inc	ebx
	cmp	dword [ebx],0A44B1BBDh ^ 2DD78BD6h
	jne	.sk_lp
	cli
	mov	ecx,cr0
	push	ecx
	and	ecx,0FFFEFFFFh
	mov	cr0,ecx
	xor	dword [ebx],2DD78BD6h
	pop	ecx
	mov	cr0,ecx
	sti
%endif             ; patch_back_MS_key
;-------------------------------------------------------------------------------
; This executes the XBE specified by T_OPEN_PATH
;-------------------------------------------------------------------------------
        mov     esi, [LaunchDataPage]
	mov	edi, 0x00001000
        mov     ebx, [esi]
        test    ebx, ebx
        jnz     m_ok
        push    edi
        call    dword [MmAllocateContiguousMemory]
        mov     ebx, eax
        mov     [esi], eax
m_ok:   push    byte 0x01
        push    edi
        push    ebx
        call    dword [MmPersistContiguousMemory]
        cld
        mov     edi, ebx
        xor     eax, eax
        mov     ecx, dword 0x00000400
        rep     stosd
        or      dword [ebx], byte -0x01
        mov     [ebx+4], eax
        lea     edi, [ebx+8]
        lea     esi, [d_path]
        push    byte DASHSZ
	pop     ecx
        rep     movsb
go_cd:  push    byte 0x02
        call    dword [HalReturnToFirmware]
.inf:	jmp	short .inf
;----------------------------------------
%ifdef  PATCH_BACK_MS_KEY
%define T_OPEN_PATH  PB_OPEN_PATH
%else
%define T_OPEN_PATH  OPEN_PATH
%endif
;----------------------------------------
	align   4
d_path: db      T_OPEN_PATH,0
DASHSZ: equ     $-d_path
;----------------------------------------
set_led:
	push	eax			; must be dword size
	push	byte 0
 	push	byte 8 			; led_register
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]
 	push	byte 1
 	push	byte 0
 	push	byte 7 			; led_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]
	ret	
;--------------------------------------------------------------------------------------
tray_data1	dd 0		; put tray data here (yes, it's in the code section)
tray_data2      dd 0  		; 2nd read if tray was open at boot (after close)
;-------------------------------------------------------------------------------------- 
	align   4
go_nkp: mov	eax, NKP_LED_COLOR      ; nkp led color "closed"
	call    set_led	   		; go set it