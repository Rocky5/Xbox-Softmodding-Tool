%include "config_runtime.inc"
;-----------------------------------------
%define	TV0	0
%define	TV1	0
%define	TV2	0
%define	TV3	0
%define	TV4	0
%define	TV5	0
%define	TV6	0
%define	TV7	0
%define	TV8	0
%define	VFR	0
%define	VFP	0
%define VFF	0
%define FF_VAL  5
;------------------------------
%ifndef	FAN_SPEED
%define FAN_SPEED 10
%endif
;------------------------------
%ifdef 	TRAY_BOOT
%define	TV0	1
%endif
%ifdef 	TRI_BOOT
%define	TV1	2
%endif
%ifdef 	PATCH_BACK_MS_KEY
%define	TV2	4
%endif
%ifdef 	OPEN_DASH_WITH_NKP
%define	TV3	8
%endif
%ifdef	OPEN_DASH_shadowC_OFF
%define	TV4	16
%endif
%ifdef	CLOSE_TRAY_ALLWAYS
%define	TV5	32
%endif
%ifdef	KINGROACH_VERSION
%define TV6	64
%endif
%ifdef	BOOT_DASH_ONLY
%define TV7	128
%endif
%ifdef	DISABLE_VIRTUAL_EEPROM_TRAY_OPEN
%define TV8 256
%endif 
;------------------------------
%ifdef 	VIDEO_FORCE_PROGRESSIVE
%define	VFP	1
%endif
%ifdef 	VIDEO_FORCE_RGB
%define	VFR	2
%endif
%ifdef 	FLICKER_FILTER
%define	VFF	4
%endif
%ifdef  FLICKER_FILTER_VALUE 
%define FF_VAL FLICKER_FILTER_VALUE
%endif
;------------------------------
%define	VID_SET 	VFR+VFP+VFF
%define TRAY_VAL        TV0+TV1+TV2+TV3+TV4+TV5+TV6+TV7+TV8
;------------------------------
fstart: dd	FAN_SPEED, TRAY_VAL, NKP_LED_COLOR, ALTDASH_LED_COLOR
	dd	PATCHBACK_LED_COLOR, VID_SET, FF_VAL, TRIBOOT_LED_COLOR
	dd	0,0,0,0
	times   64-$+fstart db 0
;------------------------------
DB1:	db	DASHBOARD_PATH
	times   80-$+DB1 db 0
;------------------------------
DB2:	db	DB_PATH2
	times   80-$+DB2 db 0
;------------------------------
DB3:	db	DB_PATH3
	times   80-$+DB3 db 0
;------------------------------
DB4:	db	OPEN_PATH
	times   80-$+DB4 db 0
;------------------------------
DB5:	db	PB_OPEN_PATH
	times   80-$+DB5 db 0
;------------------------------
;;; END OF FILE