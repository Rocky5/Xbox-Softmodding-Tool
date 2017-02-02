%include "config_ff_singleboot.settings.inc"
;-----------------------------------------
%define	TV0	1				; TRAY_BOOT							= 1
%define	TV1	0				; TRI_BOOT							= 2
%define	TV2	0				; PATCH_BACK_MS_KEY					= 4
%define	TV3	8				; OPEN_DASH_WITH_NKP				= 8
%define	TV4	0				; OPEN_DASH_shadowC_OFF				= 16
%define	TV5	0				; CLOSE_TRAY_ALLWAYS				= 32
%define	TV6	0				; KINGROACH_VERSION					= 64
%define	TV7	0				; BOOT_DASH_ONLY					= 128
%define	VFR	0				; VIDEO_FORCE_PROGRESSIVE			= 1
%define	VFP	0				; VIDEO_FORCE_RGB					= 2
%define VFF	4				; FLICKER_FILTER					= 4
%define FF_VAL 5			; FLICKER_FILTER_VALUE				= 5
%define FAN_SPEED 10
%define NKP_LED_COLOR 00FFh
%define ALTDASH_LED_COLOR 00FFh
%define PATCHBACK_LED_COLOR 000Fh
%define TRIBOOT_LED_COLOR 0063h
;------------------------------
%ifdef	FLICKER_FILTER
%define VFF	4
%endif
%ifdef	FLICKER_FILTER_VALUE 
%define FF_VAL FLICKER_FILTER_VALUE
%endif
;------------------------------
%define	VID_SET		VFR+VFP+VFF
%define TRAY_VAL	TV0+TV1+TV2+TV3+TV4+TV5+TV6+TV7
;------------------------------
fstart: dd	FAN_SPEED, TRAY_VAL, NKP_LED_COLOR, ALTDASH_LED_COLOR
	dd	PATCHBACK_LED_COLOR, VID_SET, FF_VAL, TRIBOOT_LED_COLOR
	dd	0,0,0,0
	times   64-$+fstart db 0
;------------------------------
;;; END OF FILE