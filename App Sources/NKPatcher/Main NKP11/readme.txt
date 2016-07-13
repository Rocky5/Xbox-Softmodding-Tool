Nkpatcher 11 (Unofficial) 
with-vga6 patches

--------------------
Installation
--------------------

 * Just put all the files in any directory of your choice
   edit the config.inc and config_runtime.inc to your liking and run the make file's
   the output will be default.xbe (habibi signed)
    Note;
	There is a config_runtime.inc,
	 that is the file you edit to get a runtime config file for NKP11.
	 then you run the make_runtime.bat to get the NKP11_config.bin that you FTP to E:\NKP11\
	 now if that file is there NKPatcher will use it's options .

----------------
Software License
----------------

This software is released under GNU General Public License version 2. See
the file COPYING for details.
 -rmenhal  < orignal author and creator of this Excellent Kernal patcher

Notes
-----

 * Nkpatcher now has IGR and automatic game region setting. Remember to
   disable these features from your (Evox) dashboard so that they won't
   interfere.

Files
-----

 * config.inc: kernel patcher configuration (all user setting are there)

 * config_runtime.inc: runtime patcher configuration (some user setting are there)

 * xboxapp.asm: source code for the Xbox application (nkpatcher.xbe).
                contains also some configuration options at the beginning of
                the file.

 * nkpatcher.asm: source code for the core kernel patcher

 * diskimagedrv.asm: source code for virtual/shadow C drive

 * cdromimagedrv.asm: source code for virtual CD/DVD device driver

 * xa_lba48.asm: LBA48 and partition table, gather feature parameters.

 * lba48.asm: LBA48 kernel patch

 * virteeprom.asm: virtual EEPROM kernel patch

 * NKP_top.asm: code added by xman954

 * rtc.asm: runtime config asm


-------
--------------------
changes
--------------------
From version version 11-U03 to version 11-U04 (Unofficial)

 * UPDATED to Dr oldschool's VGA 10 patches

 * ADDED 3 different config files E:\NKP11\ video.bin, led.bin, fan.bin that are read (if used)
	after the runtime_config file (if used)
		
 * ADDED DUS's Flicker Filter patch

 * ADDED fan and led reset to IGR

 * UPDATED clock check time to 2007/3/3 use with updated ernie.xtf (included)

---------------------

From version version 11-U02 to version 11-U03 (Unofficial)

 * NOTE most options can be now turned on/off when using the runtime config file (NKP11_config_open.bin
    use the normal runtime config file and rename )

 * ADDED option for king boot mode NDURE 3.X

 * ADDED option for a second runtime config file, one that is used for tray open 

 * CHANGED option for boot only to dash so that it can be configured in the runtime config (if used)

 * ADDED VGA 7 patch (Dr oldschool)

 * FIXED LED mode for Green, it will now blink when open/close tray

 * CHANGED location of config files to E:\NKP11\ shadowC_off.bin, eeprom_off.bin, NKP11_config.bin
       NKP11_config_open.bin, and boot1.bin (using king boot mode) 

 * CHANGED clock check date to more current 7-7-2006 (use included ernie.xtf for NDURE only)
       it test the clock in tray open state now and if not set it will load E:\NKP11\clock.xbe (if found)
       or the normal dash and set the LED to G,O,R,off to let you know the clock need to be set
       and it's in a modded state (set clock, do IGR and/or reboot to use normaly)

 * REMOVED clock check from config file options
     if you want it, you must put a copy of the MS xboxdash.xbe or other
      in E:\NKP11\ as clock.xbe (E:\NKP11\clock.xbe) 

--------------------

From version version 11-U01 to version 11-U02 (Unofficial)

 * ADDED two global override functions
	now there is a easy way to disable the shadowC and virtual EEPROM for making system changes.
	just put any file named shadowC_off.bin and/or EEPROM_off.bin in the root of E:\
	reboot and do what you need then delete or rename it.

 * ADDED A FEW MORE LED COLOR OPTIONS

 * ADDED dash testing for habibi and patch back paths when using tray boot
	if not there it will default to tray open NKP options (no way to get error 21)	
	a invalid path will display a "fast" frag on the eject led

 * CHANGED the fan setting to only be used when NKPatching is done 
	that way nothing will be changed if using tri-boot,habibi or patch back for some XBL trick

 * ADDED a option for tri-boot led color for the same XBL reason
	also be sure to set the ALTDASH_LED_COLOR to GREEN if using habibi tray boot for XBL

 * FIXED the tray booting not being detected corrrectly after playing a orignal game (ROE)
	without any boot time increase 
	(see topic here http://forums.xbox-scene.com/index.php?showtopic=455188&view=findpost&p=3018013)

;----------------
From version 10-vga6-ts.01 to version 11-U01 (Unofficial)

 * ADDED a runtime config file E:\NKP11_config.bin
	that you can change some options at runtime

 * Added set the clock using the M$ dash (if it was set by the fonts)
 * Added path error indicators  (using eject led)

 * ADDED a 3 dash boot order
 	it test the path's in order
 	and if none found it will boot C:\rescuedash\default.xbe (on the real C)

 * Now has expanded tray booting options 
	1. boot habibi only (for testing other hacks)
	2. boot with a patched back M$ key
	3. boot a dash from any drive (habibi not needed and if not found 
	   it will start looking from the 3 boot order dashes)
	4. shadow C off (when tray booting)
	5. close tray after boot (for faster tray booting without tri-boot )

 * Added small code fragment to xboxapps.asm, diskimagedrv.asm and nkpatcher.asm
	 to enable these new features

;----------------
From version 10-vga6 to version 10-vga6-ts.01
 * Added small code fragment to xboxapps.asm to enable these new features
 * Added user settable fan speed
 * Added user settable led color for all boot options 
 * Added user configuration of tray state booting options (power or eject button)
    user now has options to set a alt_dash and led color for tray open boot 
    there is also a option to patch back the MS signature KEY to it's original	
    value and boot a MS signed xbe (for the NDURE exploit)
 * Added tri-boot a unique twist on dual-boot that will close the tray after 4 sec
    and check for a disk in the DVD drive and if there is a habibi signed disk
    it will boot it instead of the alt_dash (good for testing and recovery)
     note: it MUST be habibi signed or the xbox will error #21
           also in all tray open boots it will bypass NKpatcher completely, so all xbe's
           need to be habibi signed with xbedump (included in this package)
   Author: X-S member xman954

From version 10 to version 10-vga6:
 * Added force RGB video feature. This will patch kernel system call
   AvSetDisplayMode to configure GPU for RGsB signal if HDTV AV pack
   is detected. Intended for connecting the Xbox to VGA monitor.
   Connect AV pack Y output to monitor green input, Cr to monitor red
   input and Cb to monitor blue input. The monitor must support sync
   on green. If your monitor does not support sync on green, special
   sync separator circuit is required.

 * Added feature to convert 480i video modes to 480p. This will patch
   kernel system call AvSetDisplayMode to convert 480i video modes to
   480p if HDTV AV pack is detected. Enhances graphics quality for
   applications that run in 480i video mode only. Note that when using
   HDTV AV pack, progressive scan video output will be forced even if
   480p video support is disabled in msdash. 
   
   Author: Krists Krilovs <pow@pow.za.net>

From version 9 to version 10:
 * Added virtual EEPROM feature. This will patch kernel system calls
   HalReadSMBusValue and HalWriteSMBusValue such that EEPROM reads and writes
   are redirected to a user-specified file.

   It is possible to disable writing to the virtual EEPROM and it is also
   possible to continue using the real EEPROM but with writing disabled.
   Since the kernel keeps a cached copy of the EEPROM contents in RAM, you
   can modify EEPROM settings in software even when writing is disabled and
   the modifications will persist across quick reboots. The modifications
   are not actually written to the disk or actual EEPROM so they will
   disappear when you cold reboot. Therefore you can experiment with EEPROM
   settings and if you mess up, just do cold reboot. :)

   Note that some (homebrew) applications may access the EEPROM directly
   without going through the kernel, especially those based on Xbox-Linux
   project's code. The present feature does not protect against this.

   The virtual EEPROM feature is not enabled in the default compilation so go
   turn it on in config.inc and recompile.

From version 8.1 to version 9:
 * oz_paulb LBA48 and partition table support

 * Virtual C image may now be of any size. You can make a virtual drive from
   scratch by first creating a file (or copying any old), say 200 MB, and then
   formatting it. You can for example virtualize the F with the unformatted image
   first and then format F using Evox dashboard. Then copy stuff to F and then
   use the done image file to virtualize C. The number at the end of
   VIRTUAL_C_DRIVE option can be used to select any existing drive for
   virtualizing. I'm sure there are also Linux tools for creating a FATX file
   system inside a given file.

 * ACTUAL_C_DRIVE option makes the real C drive appear in the specified partition.
   You can now put a shadow image file into the real C drive. Use this option
   to make the real C drive appear for example as Partition7 or Partition8.
   Then make VIRTUAL_C_PATH point to blahblah\Partition8\blaablaa.img.

 * Moved the few configuration options in xboxapp.asm to config.inc. They're all
   in config.inc now.


From version 8 to version 8.1:
 * Fixed the version 8 problem (version 7 was fine) with kernels 5530 and above


From version 7 to version 8:
 * Added virtual/shadow C drive feature
 * Added virtual CD/DVD drive feature


 -rmenhal  
