;;;---------------------------------------------------------------------------
%define ORANGE			00FFh	   ; ----------------
%define ORANGE_BLINK	0077h	   ;
%define ORANGE_BLIP		0011h	   ;
%define RED				00F0h	   ;
%define RED_BLINK		0070h	   ;	YOU CAN
%define RED_BLIP		0010h	   ;
%define GREEN			000Fh	   ;	ADD YOUR OWN
%define GREEN_BLINK		0007h	   ; 
%define GREEN_BLIP		0001h	   ;
%define CYCLE			00EBh	   ; R,O,G,O
%define CYCLE_BLINK		0063h	   ; R,O,G,Off
%define LED_OFF 		0000h	   ; ----------------
;;;---------------------------------------------------------------------------
%define NKP_LED_COLOR			ORANGE			; < CHANGE IT HERE
%define ALTDASH_LED_COLOR		RED				; < CHANGE IT HERE
%define PATCHBACK_LED_COLOR		GREEN			; < CHANGE IT HERE
%define TRIBOOT_LED_COLOR		CYCLE_BLINK		; < CHANGE IT HERE
;;;---------------------------------------------------------------------------
;;; set fan speed with NKP modes only (if omitted then no fan speed change)
;%define FAN_SPEED    10 ; < CHANGE IT HERE ( % fan speed / 2 ) 10-50 << changed !!!!
;;;---------------------------------------------------------------------------
;;; tray boot enable (must enable to use dual/tri or patch back)
%define TRAY_BOOT 
;;; tri-boot enable < dual is default, with tray_boot enabled
;%define TRI_BOOT
;;; close the tray when NOT tri-booting (faster booting then tri-boot)
%define CLOSE_TRAY_ALLWAYS  
;;;---------------------------------------------------------------------------
;;;   default Dashboard path (and tray boot closed) 2=C 1=E 6=F 7=G
;;;---------------------------------------------------------------------------
%define DASHBOARD_PATH	'\Device\Harddisk0\Partition1\Prep\default.xbe'
%define DB_PATH2	'\Device\Harddisk0\Partition8\nkpatcher\dashloader\default.xbe'
%define DB_PATH3	'\Device\Harddisk0\Partition1\dashboard\default.xbe'
;;;---------------------------------------------------------------------------
;;;   tray OPEN dash, when key IS patched back (must be a MS signed xbe)
;;;			   MUST BE PARTITION 1 or 2 
;;;
;;; use this to patch back the MS key then boot PB_OPEN_PATH 
%define PATCH_BACK_MS_KEY
%define PB_OPEN_PATH   '\Device\Harddisk0\Partition2\orig_xboxdash.xbe'
;;;---------------------------------------------------------------------------
;;;   Dashboard path's for tray OPEN dash when using dual/tri-boot
;;;			 and NOT using PATCH_BACK_MS_KEY
;;;---------------------------------------------------------------------------
;;;   tray OPEN dash, when key IS NOT patched back (must be a habibi signed xbe
;;;		and MUST be partition 1 or 2 when NOT using OPEN_DASH_WITH_NKP)
;;;
%define OPEN_PATH	'\Device\Harddisk0\Partition2\nkpatcher\rescuedash\default.xbe'
;;; use this to enable all of NKP (disable to boot some other HACK)
%define	OPEN_DASH_WITH_NKP	
;;; use this to disable shadowC in open (alt) dash 
%define	OPEN_DASH_shadowC_OFF
;;;---------------------------------------------------------------------------
;;; VIDEO patches: 
;;; 
;;; VIDEO_FORCE_RGB = Patches kernel to output RGsB video signal (RGB with
;;;	      sync on green) if HDTV AV pack is detected. Intended for
;;;	      connecting the Xbox to VGA monitor. Connect AV pack Y output
;;;	      to monitor green input, Cr to monitor red input and Cb to
;;;	      monitor blue input. The monitor must support sync on green.
;;;
;;; VIDEO_FORCE_PROGRESSIVE = Patches kernel to convert 480i video modes to
;;;	      480p if HDTV AV pack is detected. Note that when using HDTV AV
;;;	      pack, progressive scan video output will be forced even if 480p
;;;	      video support is disabled in msdash.
;;; 
;;; Not enabled by default.
;%define VIDEO_FORCE_RGB
;%define VIDEO_FORCE_PROGRESSIVE
;;; Flicker Filter Patch (interlaced video only)
;;;
;;; When enabled any call to set the flicker filter value will be
;;; replaced by a call with the value FLICKER_FILTER_VALUE.
;;;
;;; Takes values in the range 0 (off) to 5 (max filter).
;;;
;;; Flicker filter patching is not enabled by default.
;%define FLICKER_FILTER
;%define FLICKER_FILTER_VALUE 2
;;; nkpatcher Xbox application reboot behaviour after patching has been done:
;;; 
;;; BOOT_DASH_ONLY = Don't boot DVD.
;;; NORMAL_BOOT_ALWAYS = Always boot first DVD then dashboard.
;;; 
;;; default is to boot DVD first, unless doing IGR to dash.
;%define BOOT_DASH_ONLY
%define NORMAL_BOOT_ALWAYS