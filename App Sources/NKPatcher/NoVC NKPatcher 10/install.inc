%ifndef CONFIG_INC
%define CONFIG_INC
;;; --------------------------------------------------------------------------
;;; Configuration for tray state and fan speed (tray_state/fan on top of NKpatcher)
;;; --------------------------------------------------------------------------
%define ORANGE       00FFh      ; ----------------
%define ORANGE_BLINK 0077h      ;
%define RED          00F0h      ;  YOU CAN
%define RED_BLINK    0070h      ;      ADD YOUR
%define GREEN        000Fh      ;       OWN
%define GREEN_BLINK  0007h      ; (IF YOU KNOW 
%define CYCLE        00EBh      ;      HOW TO)
%define LED_OFF      0000h      ; ----------------
%define NKP_LED_COLOR          RED      ; < CHANGE IT HERE, from table or it's hex value !!!!!!
%define ALTDASH_LED_COLOR      RED   ; < CHANGE IT HERE, from table or it's hex value !!!!!!
%define PATCHBACK_LED_COLOR    RED ; < CHANGE IT HERE, from table or it's hex value !!!!!!
;;; set fan speed (if omitted then no fan speed change)
;%define FAN_SPEED    0009h      ; < CHANGE IT HERE speed 10-50 * 2 = % (000Ah-0032h)
;;; tray dual/tri-boot enable
;%define TRAY_BOOT 
;;; tri-boot enable < dual is default, with tray_boot enabled
;%define TRI_BOOT
;;; tray open > patch back MS key or not, then run tray open dash 
;%define PATCH_BACK_MS_KEY
;;;---------------------------------------------------------------------------
;;;   Dashboard path's for tray OPEN dash when using dual/tri-boot
;;;---------------------------------------------------------------------------
;;;   tray OPEN dash, when key IS patched back (must be a MS signed xbe)
;;;                        MUST BE PARTITION 1 or 2 
;;;
%define PB_OPEN_PATH   ''
;;;---------------------------------------------------------------------------
;;;   tray OPEN dash, when key IS NOT patched back (must be a habibi signed xbe)
;;;                        MUST BE PARTITION 1 or 2 
;;;
%define OPEN_PATH      ''
;;;---------------------------------------------------------------------------
;;;   Tray closed Dashboard path, NKP will boot  2=C 1=E 6=F 7=G
;;;---------------------------------------------------------------------------
;;;                                
%define DASHBOARD_PATH '\Device\Harddisk0\Partition1\UDATA\21585554\000000000000\Softmod Files\UX.xbe'
;;;---------------------------------------------------------------------------
;;; VIDEO patches:
;;; 
;;; VIDEO_FORCE_RGB = Patches kernel to output RGsB video signal (RGB with
;;;           sync on green) if HDTV AV pack is detected. Intended for
;;;           connecting the Xbox to VGA monitor. Connect AV pack Y output
;;;           to monitor green input, Cr to monitor red input and Cb to
;;;           monitor blue input. The monitor must support sync on green.
;;;
;;; VIDEO_FORCE_PROGRESSIVE = Patches kernel to convert 480i video modes to
;;;           480p if HDTV AV pack is detected. Note that when using HDTV AV
;;;           pack, progressive scan video output will be forced even if 480p
;;;           video support is disabled in msdash.
;;; 
;;; Not enabled by default.
;%define VIDEO_FORCE_RGB
;%define VIDEO_FORCE_PROGRESSIVE
;;; --------------------------------------------------------------------------
;;; Configuration for nkpatcher
;;; --------------------------------------------------------------------------
;;;
;;; Reset-on-eject behaviour:	
;;;
;;; EJECT_FIX = Never reset on eject.
;;; CD_DVD_ALWAYS_ROE = Always reset on eject for CD/DVD loaded XBEs, never
;;;                     reset on eject for hard disk loaded XBEs.
;;;
;;; Default is to never reset on eject for hard disk loaded XBEs, but reset
;;; on eject CD/DVD loaded XBEs based on the ROE flag in the XBE header.
;%define EJECT_FIX
%define CD_DVD_ALWAYS_ROE
;;; Automatic game region settings:
;;;
;;; GAME_REGION_AUTO = Set game region automatically based on the game region
;;;                    field in XBE header. Since the header may have several
;;;                    regions enabled there's a choice to be made. First
;;;                    tried is the game region from EEPROM, then the primary
;;;                    through tertiary regions below.
;;; GAME_REGION_IGNORE_REAL = Ignore the region stored in EEPROM and only
;;;                           use primary through tertiary regions below.
;;;
;;; Default is to use only the game region stored in the EEPROM and not
;;; change it.
;%define GAME_REGION_AUTO
;%define GAME_REGION_IGNORE_REAL
%ifdef GAME_REGION_AUTO
;;; Game regions:
;;; 
;;; 1 = North America
;;; 2 = Japan
;;; 4 = Rest of world
%define GAME_REGION_PRIMARY	4
%define GAME_REGION_SECONDARY	1
%define GAME_REGION_TERTIARY	2
%endif	; GAME_REGION_AUTO
;;; In-Game Reset settings:	
;;;
;;; IGR = Enable IGR code
;;;
;;; Button combinations:
;;;	* triggers+back+start
;;;	* triggers+back+black
;;;	* triggers+start+black
;;; 
;;; IGR_TO_DASHBOARD = Reset to dashboard, not DVD. Changed button combos:
;;;	* triggers+back+start to dashboard
;;;	* triggers+back+black to dashboard
;;;	* triggers+start+black to first DVD, then dashboard
;;;
;;; IGR_FULL_REBOOT = Do a full reboot instead of the default quick reboot.
;;;                   Some games hang (Soul Calibur 2 during a match) with
;;;                   quick reboot.
;;;
;;; IGR_MASTER_PORT #port_number = Only allow IGR from pad port #port_number,
;;;                                where #port_number is 0-3 (0 the left-most
;;;                                port, 3 the right-most.)
;;;
;;; IGR is not enabled by default.
%define IGR
%define IGR_TO_DASHBOARD
; %define IGR_FULL_REBOOT
; %define IGR_MASTER_PORT 0
;;; Xbox Live blocking:
;;;
;;; XBL_BLOCK = Enable blocking
;;;
;;; Blocking not enabled by default.
%define XBL_BLOCK
;;; Virtual/Shadow C drive:
;;;
;;; VIRTUAL_C_DRIVE #part_num = Enable virtual C drive. Access to the partition
;;;                   number #part_num will be redirected to file VIRTUAL_C_PATH,
;;;                   which must be an image of a raw FATX file system. Note that
;;;                   the usual C drive partition number is 2. However, you
;;;                   could replace any drive with a virtual drive. The feature
;;;                   name "virtual C" is thus a bit misleading, although there's
;;;                   not much point virtualizing any other drive.
;;;
;;; ACTUAL_C_DRIVE #part_num = Make the actual C drive appear as
;;;                            partition #part_num. This requires LBA48 patches
;;;                            enabled.
;;;
;;; Not enabled by default.
;%define VIRTUAL_C_DRIVE 2
;%define ACTUAL_C_DRIVE 14
;%define VIRTUAL_C_PATH '\Device\Harddisk0\Partition0'
;;; Virtual CD/DVD drive:
;;;
;;; VIRTUAL_CDROM_DRIVE = Enable virtual C drive. This will create a virtual
;;;			device \Device\CdRom1 and link it to \Device\CdRom0
;;;			when an ISO image is attached to CdRom1 (through a system
;;;			call.)
;;;
;;; Not enabled by default.
;%define VIRTUAL_CDROM_DRIVE
;;; 48-bit Logical Block Addressing and partition table settings:
;;;
;;; LBA48 = Enable. Allows use of disk space beyond 137 GB. Only 32 bits are
;;;         actually used so maximum used disk size is now 2 TB.
;;;
;;; PARTITION_METHOD #method_num = Select partitioning scheme,
;;;         where #method_num is:
;;;         0, for only standard Xbox partitions i.e. no F or G drives
;;;         1, for partition 6 (F drive) takes rest of the drive (after standard)
;;;         2, for partition 6 takes up to 137 GB, partition 7 (G drive) rest
;;;         3, for partition 6 takes up to 137 GB, no partition 7
;;;
;;; Note that method 1 results in ".06 bios" and method 2 in ".67 bios".
;;;
;;; IGNORE_HD_PARTITION_TABLE = Don't use harddisk partition table even if it
;;;                             exists (on the first sector of the drive).
;;;
;;; LBA48 and partition table not enabled by default.
%define LBA48
%define PARTITION_METHOD 0
;%define PARTITION_METHOD 1
;%define PARTITION_METHOD 2
;%define PARTITION_METHOD 3
;%define IGNORE_HD_PARTITION_TABLE
;;; Virtual EEPROM settings:
;;;
;;; VIRTUAL_EEPROM = Enable. Access to the EEPROM through the kernel will be
;;;                  redirected to file VIRTUAL_EEPROM_PATH.
;;;
;;; ENABLE_REAL_EEPROM_WRITE = Enable writing to the real EEPROM.
;;;                  If the VIRTUAL_EEPROM_PATH file is not found nkpatcher (with
;;;                  VIRTUAL_EEPROM enabled) will default to using the real EEPROM,
;;;                  but writing disabled. If you want the writing enabled then
;;;                  use this option. The main purpose of having real writes
;;;                  disabled by default is that if you mistype the path in
;;;                  VIRTUAL_EEPROM_PATH then you might go messing with the real
;;;                  EEPROM thinking it was only the virtual.
;;;
;;; DISABLE_VIRTUAL_EEPROM_WRITE = Disable writing to the virtual EEPROM.
;;;
;;; Virtual EEPROM is disabled by default. Also note that normally you shouldn't
;;; define ENABLE_REAL_EEPROM_WRITE.
;%define VIRTUAL_EEPROM
;%define VIRTUAL_EEPROM_PATH ''
;%define DISABLE_VIRTUAL_EEPROM_WRITE
;%define ENABLE_REAL_EEPROM_WRITE
;;; nkpatcher Xbox application reboot behaviour after patching has been done:
;;; 
;;; BOOT_DASH_ONLY = Don't boot DVD.
;;; NORMAL_BOOT_ALWAYS = Always boot first DVD then dashboard.
;;; 
;;; default is to boot DVD first, unless doing IGR to dash.
%define BOOT_DASH_ONLY
;%define NORMAL_BOOT_ALWAYS
;;; Patches inside INIT section (boot unlocked HDs etc.)
;;;
;;; INIT_SEC_PATCHES = enable
;;;
;;; Do NOT enable these for nkpatcher.xbe executed on the Xbox (will crash
;;; and/or weird behaviour.)
;%define INIT_SEC_PATCHES
;;; --------------------------------------------------------------------------
;;; Configuration end
;;; --------------------------------------------------------------------------
%endif ; CONFIG_INC