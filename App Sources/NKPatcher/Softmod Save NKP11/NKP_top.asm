;
;   NKP top
; 
;----------------------------------------
;	new NKP startup by XMAN954
;----------------------------------------
;%define	DEBUG_ON
	cld
	lea	eax,[readfilestr]	; try to open the main config file 
	call	ld_cfg
;---------------------------------------
	lea	eax,[options+0] 	; addr to put data (fan speed)
	mov	[byte_pnt],eax		; save it
	mov	[byte_pnt+4],dword 01	; read 1 byte
	lea	eax,[read_FAN]		; try to open the config file 
	call	load_byte
;---------------------------------------
	lea	eax,[options+8] 	; addr to put data (led color)
	mov	[byte_pnt],eax		; save it
	mov	[byte_pnt+4],dword 01	; read 1 byte
	lea	eax,[read_LED]		; try to open the config file 
	call	load_byte
;---------------------------------------
	lea	eax,[options+20]	; addr to put data (video settings)
	mov	[byte_pnt],eax		; save it
	mov	[byte_pnt+4],dword 01	; read 1 byte
	lea	eax,[read_VIDEO]	; try to open the config file 
	call	load_byte
;---------------------------------------
%ifdef	DEBUG_ON
	call	write_debug_file	; <<<<<<<<<<<<<<
%endif
;;;----------------------------------------
;;;;	test the clock date 
;;;----------------------------------------
	xor	eax,eax
	push	eax
	push	eax
	mov	ebx,esp 		; space for current time
	push	eax
	push	eax
	mov	esi,esp 		; space for minimum time
	push	ebx
	call	dword [KeQuerySystemTime]
	push	esi
	lea	eax,[mintime]
	push	eax
	call	dword [RtlTimeFieldsToTime]
	test	eax,eax
	jz	short tst_tr		; bail	  
	mov	eax,[ebx]
	mov	edx,[ebx+4]
	sub	eax,[esi]
	sbb	edx,[esi+4]
	jnc	short tst_tr		; time is ok
;;;----------------------------------------
;;;;	date is NOT set
;;;----------------------------------------
	mov	eax, CYCLE_BLINK	; R,O,G,Off
	call	set_led 		; set led
	mov	eax, [options+8]	; nkp led color "closed"
	mov	[igr_led+0],eax 	; save igr color
	jmp	near go_led		; with full NKP options (nkp will check the clock.xbe)
;---------------------------------------
;	test tray state stuff
;---------------------------------------
tst_tr: mov	ebx,[options+4] 	; get tray options
	test	ebx,01			; tray mask
	je	near go_nkp		; nope
;---------------------------------------
tr_lp0. mov	eax,0x07FFFF
dly_1	dec	eax
	jnz	short dly_1
;-------	
	lea	eax,[tray_data1]	; get tray state
	push	eax
	push	byte 0
	push	byte 3
	push	byte 20h
	call	dword [HalReadSMBusValue]
;-------
	cmp	byte [tray_data1], 40h	; 40h = no disk
	je	near go_nkp		; normal boot nkp
	cmp	byte [tray_data1], 60h	; 60h = disk
	je	near go_nkp		; normal boot nkp
	cmp	byte [tray_data1], 10h	; 10h = open
	jne	short tr_lp0.
;----------------------------------------
;	tray is open 
;----------------------------------------
	lea	eax,[readfilestr_open]
	call	ld_cfg			; try to open the " tray open config file "
;-------
	mov	ebx,[options+4] 	; reload tray options
	test	ebx,02			; tri-boot mask
	jne	do_tri.
	test	ebx,32			; close tray mask
	je	short do_ad.		; no "close tray", so do open dash
	call	close_tray		; close it
	jmp	short do_ad.		; do open dash now
;----------------------------------------
do_tri. mov	eax, 0055h		; "orange fast blink, open for now"
	call	set_led 		; go set it
;-------
	mov	eax,0x1FFFFFFF
dly_0	dec	eax
	jnz	short dly_0		; ~ 1 sec + 3 sec (tray catch up)
	call	close_tray		; close it
;----------------------------------------
TS_lp:	mov	eax,0x07FFFF
dly_2	dec	eax
	jnz	short dly_2	
;-------
	lea	eax,[tray_data2]	; read after tray is closed
	push	eax
	push	byte 0
	push	byte 3
	push	byte 20h
	call	dword [HalReadSMBusValue]
;-------
	cmp	byte [tray_data2],40h	; 40h = no disk 
	je	do_ad.			; we need to alt dash
	cmp	byte [tray_data2],60h	; 60h = game disk 
	jne	TS_lp			; loop till disk is read
;-------
set_cd. mov	eax,[options+28]	; tri-boot led color  
	mov	[igr_led+0],eax 	; save igr color
	call	set_led 		; set it and boot Cdrom
	mov	esi,[LaunchDataPage]
	mov	ebx,[esi]
	jmp	go_cd			; boot cd
;----------------------------------------
do_ad.	test	ebx,64			; king_boot mask	
	je	short no_kb
;-------  King boot start --------------
	lea	eax,[READ1]		; #1 file test
	call	open_file		; go try to open it
	cmp	[esi+4],dword 01h	; status 
	jne	do_pb.			; not there so, patch back and run xb0xdash.xbe
	push	dword [ebx]
	call	dword [NtClose] 	; close the file !!!
	mov	eax,[options+16]	; open led color
	call	set_led 		; go set it
	lea	eax,[DB4]		; open path 
	jmp	short tst_f.		; habibi patch only
;-------  King boot end ----------------
no_kb:	test	ebx,04			; patch back mask
	jne	do_pb.			; patch back, so do it
;----------------------------------------
	mov	eax,[options+12]	; open led color
	mov	[igr_led+0],eax 	; save igr color
	call	set_led 		; go set it
	test	ebx,08			; open_dash with full NKP options mask
	jne	op_nkp			; do it
	lea	eax,[DB4]		; open path
tst_f.	call	open_file		; go try to open The file
	cmp	[esi+4],dword 01h	; status was put here by NtOpenfile (8 bytes total)
	je	short go_od.		; with habibi patch only
	jmp	short tr_er1.		; file is not there
;----------------------------------------
do_pb.	mov	eax,[options+16]	; pb led color
	call	set_led 		; go set it
	lea	eax,[DB5]		; patch back path
	call	open_file		; go try to open The file
;-------
	cmp	[esi+4],dword 01h	; status was put here by NtOpenfile (8 bytes total)
	jne	short tr_er1.		; file is not there
	push	ebx			; save pointer
	call	pb_ms_key		; patch it back sub
	pop	ebx			; restore pointer
;---------------------------------------
go_od.	push	dword [ebx]
	call	dword [NtClose]
;---------------------------------------
; execute the XBE specified by open dash
;---------------------------------------
	call	do_LDP			; go setup the LDP
	or	dword [ebx], byte -0x01
	mov	[ebx+4], eax
	lea	edi, [ebx+8]
	mov	esi,[handle_save+4]	; location
	mov	ecx,[handle_save+0]	; size
	inc	ecx			; add 1
	rep	movsb
go_cd:	push	byte 0x02
	call	dword [HalReturnToFirmware]
.inf:	jmp	short .inf
;----------------------------------------
tr_er1. mov	eax,0x00A5
	call	set_led
;----------------------------------------
;    lets do the open dash with all NKP options
;----------------------------------------
op_nkp: lea	eax,[DB4]		; open dash file 
	jmp	set_db
;----------------------------------------
;    lets boot with all NKP options
;----------------------------------------
go_nkp: mov	eax, [options+8]	; nkp led color "closed"
	mov	[igr_led+0],eax 	; save igr color
	call	set_led  
go_led: lea	eax,[DB1]		; first dashfile in list
set_db	mov	[nkp_dash+0],eax	; save it (will be tested later in nkpatcher.asm)
;;;-------------------------------------
	mov	eax,[options+0] 	; SET THE FAN SPEED
	cmp	eax,50
	jg	short no_fan.		; invalid value
	cmp	eax,10	
	jle	short no_fan.		; invalid value
;-------
	mov	eax,[options+0] 
	push	eax			; it s there
	push	byte 0
	push	byte 6			; fan_register
	push	byte 20h		; pic_address
	call	dword [HalWriteSMBusValue]
;-------
	push	byte 1			; custom speed
	push	byte 0
	push	byte 5			; fan_mode
	push	byte 20h		; pic_address
	call	dword [HalWriteSMBusValue]
	jmp	go_fp.
;-------
no_fan. mov	eax, 10 		; preload for igr
	mov	[options+0],eax 	; set stock speed 20%
;------
go_fp.	mov	ebx,[options+4] 	; load tray options
	test	ebx,32			; close tray mask
	je	near new_start		; run NKP <<<<<<<<<<<<
;-------
	mov	eax,0x07FFFF		; ~ 1/100 sec 
dly_c	dec	eax
	jnz	short dly_c
	call	close_tray		; close it, if open
	jmp	near new_start		; now run NKP <<<<<<<<<<<<
;;;--------------------------------------------------------------------------
;;;				 Subs
;;;--------------------------------------------------------------------------
open_file:				; eax must point to file string
	call	get_sz			; go get size of string
;-------
	lea	ebx,[handle_save+12]	; has pointer to > handle_save+0
;-------
	push	byte 40h		; some unknown attribute ????
	lea	ecx,[handle_save]	; len of file string 
	push	ecx			; stack it
	push	byte 0			; some unknown attribute ????
	mov	edi,esp 		; Object attributes (point to them)
;-------
	lea	esi,[IO_status] 	; IO status (point to space to put them)
	mov	[esi+4],dword -1	; mess up status value, tested later
;--------------------- actual call -----
	push	byte 40h + 20h		; IN ULONG CreateOptions > FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE
	push	byte 02h		; IN ULONG ShareAccess	 > FILE_SHARE_READ 
	push	esi			; OUT PIO_STATUS_BLOCK IoStatusBlock
	push	edi			; IN POBJECT_ATTRIBUTES ObjectAttributes
	push	dword 120003h		; IN ACCESS_MASK DesiredAccess (src NKP EEPROM stuff) ????
	push	ebx			; OUT PHANDLE FileHandle
	call	dword [NtOpenFile]
	add	esp,4+4+4		; unwind stack !!!
	ret
;;;--------------------------------------------------------------------------
get_sz: mov	[handle_save+4],eax	; save the location of file string	
	xor	ebx,ebx 		; start at 0
	mov	ecx,79			; set max len
nx_chr. cmp	byte [eax+ebx],0	; test it  > thanks DUS !!!
	je	nx_out. 		; found end
	inc	ebx			; count it
	loopnz	nx_chr. 		; maxcount
	nop				; error set later (yet to do)!!!!
nx_out. mov	[handle_save],ebx	; save len
	ret
;;;--------------------------------------------------------------------------
ld_cfg: call	open_file		; go try to open config file
	cmp	[esi+4],dword 01h	; status was put here by NtOpenfile (8 bytes total)
	jne	short no_cfg.		; file not there !
;-------
	lea	eax,[fstart]		; start addr to put data 
	add	esi, 8			; IO status (point to next space to put them)
	xor	ecx,ecx
	push	ecx			; IN PLARGE_INTEGER ByteOffset
	push	dword fs_end-fstart	; IN ULONG Length "file size"
	push	eax			; IN PVOID Buffer "data start" 
	push	esi			; OUT PIO_STATUS_BLOCK IoStatusBlock
	push	ecx			; IN PVOID ApcContext OPTIONAL
	push	ecx			; IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	push	ecx			; IN HANDLE Event OPTIONAL
	push	dword [ebx]		; IN HANDLE FileHandle
	call	dword [NtReadFile]	; read the file into overlay
;-------
	push	dword [ebx]
	call	dword [NtClose]
no_cfg. ret 
;;;-------------------------------------
load_byte:
	call	open_file		; go try to open config file
	cmp	[esi+4],dword 01h	; status was put here by NtOpenfile (8 bytes total)
	jne	short no_byt.		; file not there !
;-------
	mov	eax,[byte_pnt]		; start addr to put data 
	add	esi, 8			; IO status (point to next space to put them)
	xor	ecx,ecx
	push	ecx			; IN PLARGE_INTEGER ByteOffset
	mov	ecx,[byte_pnt+4]	; size of data 
	push	ecx			; IN ULONG Length "file size"
	push	eax			; IN PVOID Buffer "data start" 
	push	esi			; OUT PIO_STATUS_BLOCK IoStatusBlock
	xor	ecx,ecx
	push	ecx			; IN PVOID ApcContext OPTIONAL
	push	ecx			; IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	push	ecx			; IN HANDLE Event OPTIONAL
	push	dword [ebx]		; IN HANDLE FileHandle
	call	dword [NtReadFile]	; read the file into overlay
;-------
	push	dword [ebx]
	call	dword [NtClose]
no_byt. ret 
;;;--------------------------------------------------------------------------
close_tray:
	push	byte 1			; close tray
	push	byte 0
	push	byte 0Ch		; tray_mode
	push	byte 20h		; pic_address
	call	dword [HalWriteSMBusValue]
	ret
;;;--------------------------------------------------------------------------
set_led:
	cmp	eax, 000fh		; green value
	je	short set_auto. 	; if green set to auto
;-------
	push	eax			; must be dword size
	push	byte 0
	push	byte 8			; led_register
	push	byte 20h		; pic_address
	call	dword [HalWriteSMBusValue]
	push	byte 1			; manual mode
	push	byte 0
	push	byte 7			; led_mode
	push	byte 20h		; pic_address
	call	dword [HalWriteSMBusValue]
	ret
set_auto.
	push	byte 0			; auto mode
	push	byte 0
	push	byte 7			; led_mode
	push	byte 20h		; pic_address
	call	dword [HalWriteSMBusValue]
	ret
;;;--------------------------------------------------------------------------
do_LDP: mov	esi, [LaunchDataPage]
	mov	edi, 0x00001000
	mov	ebx, [esi]
	test	ebx, ebx
	jnz	m_ok
	push	edi
	call	dword [MmAllocateContiguousMemory]
	mov	ebx, eax
	mov	[esi], eax
m_ok:	push	byte 0x01
	push	edi
	push	ebx
	call	dword [MmPersistContiguousMemory]
	cld
	mov	edi, ebx
	xor	eax, eax
	mov	ecx, dword 0x00000400	; zero all of LDP
	rep	stosd
	ret
;;;--------------------------------------------------------------------------
; This modifies the habibi public key back to MS public key.
;;;--------------------------------------------------------------------------
pb_ms_key:
	mov	ebx,[XePublicKeyData]
	mov	esi,80010000h
	test	ebx,ebx
	jnz	.chk
.s_key: mov	ebx,esi
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
	ret
;;;----------------------------------------
;;;;	go test the clock date 
;;;----------------------------------------
ck_clk: xor	eax,eax
	push	eax
	push	eax
	mov	ebx,esp 		; space for current time
	push	eax
	push	eax
	mov	esi,esp 		; space for minimum time
	push	ebx
	call	dword [KeQuerySystemTime]
	push	esi
	lea	eax,[mintime]
	push	eax
	call	dword [RtlTimeFieldsToTime]
	test	eax,eax
	jz	near ck_skp		; bail	  
	mov	eax,[ebx]
	mov	edx,[ebx+4]
	sub	eax,[esi]
	sbb	edx,[esi+4]
	jnc	near ck_skp		; time is ok
;--------------------------------------
;	do clock set, load e:\NKP11\clock.xbe
;--------------------------------------
	lea	eax,[clk_ph]		; MS xboxdash.xbe @ E:\NKP11\clock.xbe
	call	open_file		; go try to open it
	cmp	[esi+4],dword 01h	; status was put here by NtOpenfile (8 bytes total)
	jne	near ck_skp		; file not there bail !
	push	dword [ebx]		; set to handle_save+12
	call	dword [NtClose] 	; close it
;-------
	add	esp,byte 8+8+4+4	; unstack it + return call * 2 deep (NO turning back)
	lea	eax,[clk_ph]		; set path xboxdash.xbe (clock.xbe)
	call	near get_sz		; used to setup handle_save for LDP
	call	near do_LDP		; go setup the LDP
	lea	edi, [ebx+8]
	mov	esi, [handle_save+4]	; get file string
	mov	ecx, [handle_save+0]	; get len from "open_file > ck_sz"
	inc	ecx			; add 1 to len
	rep	movsb			; move dash path into DP
	mov	[ebx+000h], dword 0x01	; use parmeters
	mov	[ebx+400h], dword 0x03	; parameter for settings in dashboard
	mov	[ebx+408h], dword 0x01	; CLOCK within settings
	push	byte 0x02		; quick reboot
	call	dword [HalReturnToFirmware]
	jmp	short $
;-------
ck_skp: add	esp,byte 8+8		; unstack it clock is OK
	ret
;;;--------------------------------------------------------------------------
%ifdef	DEBUG_ON
write_debug_file:
	lea	eax,[infofilestr]
	mov	[infofile+4],eax
	push	eax
	mov	ebx,esp 		; File handle
;-------
	lea	esi,[IO_status+16]	; IO status (point to space to put them)
;-------
	push	byte 40h
	lea	eax,[infofile]
	push	eax
	push	byte 0
	mov	edi,esp 	; Object attributes
;--------------------- actual call -----
	push	byte 62h		; IN ULONG CreateOptions
	push	byte 03h		; IN ULONG CreateDisposition
	push	byte 03h		; IN ULONG ShareAccess (read/write)
	push	80h			; IN ULONG FileAttributes
	push	byte 0			; IN PLARGE_INTEGER AllocationSize OPTIONAL
	push	esi			; OUT PIO_STATUS_BLOCK IoStatusBlock
	push	edi			; IN POBJECT_ATTRIBUTES ObjectAttributes
	push	dword 40100000h 	; IN ACCESS_MASK DesiredAccess
	push	ebx			; OUT PHANDLE FileHandle
	call	dword [NtCreateFile]
;---------------------------------------
	lea	eax,[IO_status] 	; start addr to get data to write for debug file
	lea	eax,[80000000h] 	; start of kernel
	lea	esi,[spare]		; IO status (point to space to put them)
	xor	ecx,ecx
;--------------------- actual call -----
	push	ecx			; IN PLARGE_INTEGER ByteOffset
	push	dword 327680		; IN ULONG Length "file size" ??????? 16,32 bits
	push	eax			; IN PVOID Buffer "data start" 
	push	esi			; OUT PIO_STATUS_BLOCK IoStatusBlock
	push	ecx			; IN PVOID ApcContext OPTIONAL
	push	ecx			; IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	push	ecx			; IN HANDLE Event OPTIONAL
	push	dword [ebx]		; IN HANDLE FileHandle
	call	dword [NtWriteFile]
;---------------------------------------
	push	dword [ebx]
	call	dword [NtClose]
	add	esp,16			; unwind stack !!!
	ret
%endif
	align	16 
	db	' Copyright 2004 rmenhal '
	db	' http://forums.xbox-scene.com '
	db	' modified by Xman954	'
%ifdef	KINGROACH_VERSION
	db	' Kingroach boot Version '
%endif
;-----------------------------------------
%define TV0	0
%define TV1	0
%define TV2	0
%define TV3	0
%define TV4	0
%define TV5	0
%define TV6	0
%define TV7	0
%define TV8	0
%define VFR	0
%define VFP	0
%define VFF	0
%define FF_VAL	5
;------------------------------
%ifndef FAN_SPEED
%define FAN_SPEED 10
%endif
;------------------------------
%ifdef	TRAY_BOOT
%define TV0	1
%endif
%ifdef	TRI_BOOT
%define TV1	2
%endif
%ifdef	PATCH_BACK_MS_KEY
%define TV2	4
%endif
%ifdef	OPEN_DASH_WITH_NKP
%define TV3	8
%endif
%ifdef	OPEN_DASH_shadowC_OFF
%define TV4	16
%endif
%ifdef	CLOSE_TRAY_ALLWAYS
%define TV5	32
%endif
%ifdef	KINGROACH_VERSION
%define TV6	64
%endif
%ifdef	BOOT_DASH_ONLY
%define TV7	128
%endif
%ifdef	DISABLE_VIRTUAL_EEPROM_TRAY_OPEN
%define TV8	256
%endif
;------------------------------
%ifdef	VIDEO_FORCE_PROGRESSIVE
%define VFP	1
%endif
%ifdef	VIDEO_FORCE_RGB
%define VFR	2
%endif
%ifdef	FLICKER_FILTER
%define VFF	4
%endif
%ifdef	FLICKER_FILTER_VALUE 
%define FF_VAL FLICKER_FILTER_VALUE
%endif
;------------------------------
%define VID_SET 	VFR+VFP+VFF
%define TRAY_VAL	TV0+TV1+TV2+TV3+TV4+TV5+TV6+TV7+TV8
;------------------------------
	align	16  
;------------------------------
IO_status:	dd 0,0,0,0
		dd 0,0,0,0
handle_save:	dd 0,0,0,handle_save
spare:		dd 0FFFFFFFFh,0FFFFFFFFh,0,0	; markers
nkp_dash	dd 0,0,0,0			; pointer to dash , len of dash str
byte_pnt	dd 0,1
igr_led 	dd 0,0
;------------------------------
options:		; ****** options overlay starts here !!!!!
fstart: dd	FAN_SPEED, TRAY_VAL, NKP_LED_COLOR, ALTDASH_LED_COLOR
	dd	PATCHBACK_LED_COLOR, VID_SET, FF_VAL, TRIBOOT_LED_COLOR
	dd	0,0,0,0
	times	64-$+fstart db 0
;------------------------------
DB1:	db	DASHBOARD_PATH
	times	80-$+DB1 db 0
;------------------------------
DB2:	db	DB_PATH2
	times	80-$+DB2 db 0
;------------------------------
DB3:	db	DB_PATH3
	times	80-$+DB3 db 0
;------------------------------
DB4:	db	OPEN_PATH
	times	80-$+DB4 db 0
;------------------------------
DB5:	db	PB_OPEN_PATH
	times	80-$+DB5 db 0
;------------------------------
fs_end: align	4
;------------------------------
READ1:		db	'\Device\Harddisk0\Partition2\nkpatcher\configs\boot1.bin',0,0 
ee_fp:		db	'\Device\Harddisk0\Partition2\nkpatcher\configs\EEPROM_off.bin',0,0 
sc_fp:		db	'\Device\Harddisk0\Partition2\nkpatcher\configs\shadowc_off.bin',0,0	
clk_ph: 	db	'\Device\Harddisk0\Partition2\nkpatcher\configs\clock.xbe',0,0 
;-------------------------------------------------------------------------------------------
%ifdef	ACTUAL_C_DRIVE
%define ACD	ACTUAL_C_DRIVE
%else
%define ACD	2
%endif
;-----------
rescuedash1:	db	'\Device\Harddisk0\Partition',ACD+030h,'\nkpatcher\rescuedash\default.xbe',0,0
rescuedash2:	db	'\Device\Harddisk0\Partition2\nkpatcher\rescuedash\default.xbe',0,0
;--------------------------------------------------------------------------------------------
infofilestr:	db '\Device\Harddisk0\Partition1\debug.bin',0		; E:\debug.bin
infofile	dd $-infofilestr-1,0,0,0 ; < space for handle 
;-----------
readfilestr:		db CFG_FILE,0,0 	; config file name
readfilestr_open:	db CFG_FILE_OPEN,0,0	; config file open name
read_FAN:	db	'\Device\Harddisk0\Partition2\nkpatcher\configs\FAN.bin',0,0
read_LED:	db	'\Device\Harddisk0\Partition2\nkpatcher\configs\LED.bin',0,0 
read_VIDEO:	db	'\Device\Harddisk0\Partition2\nkpatcher\configs\VIDEO.bin',0,0
;;; --------------------------------------------------------------------------
tray_data1	dd 0		; put tray data here 
tray_data2	dd 0		; 2nd read if tray was open at boot (after close)
;--------------------------------------------------------------------------------------
mintime:	dw 2016 	; Year
		dw 1		; Month
		dw 1	; Day
		dw 12		; Hour
		dw 0		; Minute
		dw 0		; Second
		dw 0		; Milliseconds
		dw 0		; Weekday (ignored)
;-------------------------------------------