**********************************
**********************************
**      BROUGHT TO YOU BY:      **
**********************************
**********************************
**                              **
**        [TEAM ASSEMBLY]       **
**                              **
**     www.team-assembly.com    **
**                              **
**********************************

     ConfigMagic FINAL
 
     Release Date: 02/10/2003

**********************************


RELEASE NOTES:
--------------
1. Brand NEW Graphics and awsome new "look" with cool intro and credits movie !! 

2. Load ANY Version EEPROM Images; Automatically converts between V1.0 and V1.1 

3. More Safety features to ONLY allow valid Video and XBE Regions. 

4. Backup feature will create .CFG file as well as the .txt file with XBOX Info. 
   .txt backup file:   
    *XBOX VERSION
    *KERNEL VERSION
    *RAM SIZE
    *XBOX SERIAL
    *MAC ADDRESS
    *ONLINE KEY
    *VIDEO STANDARD
    *XBE REGION
    *HDD KEY
    *CONFOUNDER
    *HDD MODEL
    *HDD SERIAL
    *HDD PASSWORD
    *XBOX DVD MODEL

5. Now Using SHA1 Middle Message to detect XBOX Version; Better compatibility with XEcuter2 Bios. 

6. Allow you to FORCE XBOX Version if there are compatibility problems (ONLY ADVANCED USERS!).

7. Huge changes in XKUtils libary that is shared between LiveInfo and ConfigMagic. 

8. NEW Feature: Blind Restore EEPROM; Boot from DVD and hold in a button sequence to "blind" restore your EEPROM. 

9. Valid Blank V1.0 and V1.1 EEPROM images supplied (USA Region, NTSC) 

10. Auto create backups before locking HDD..



Features: (Both V1.0 and V1.1)
------------------------------
(Please check the configmagic.ini for all file locations and config options)

1. You can update your XBOX EEPROM in one of four ways
    * using a valid EEPROM.BIN file 
    * You can "build" a new eeprom image from a configuration file
    * Edit your current, loaded (.bin) or built (.cfg) eeprom data "On-The-Fly" 
    * use our BLIND restore feature to restore a .bin file if you have no visuals..

2. Blind Restore EEPROM if you have no Video image..
   Usage: (MAKE SURE YOU HAVE A VALID EEPROM.BIN in \DATA)
    1) Boot ConfigMagic from DVD Drive
    2) Press A button to Skip Video (press A untill you see the orange light flash)
    3) Wait untill eject light flashes orange (flashing orange means ConfigMagic is ready)
    4) To Start Hold BOTH triggers and press WHITE button !
    5) Orange light stops flashing and stays steady (Steady orange means busy)
    6) If successfull Eject light will Flash GREEN (Flash Green means success!)
	*IF there was a problem loading/decrypting eeprom, Eject light will turn RED (Red means Error !)

2. You Can Lock and Unlock your HDD with the password created from your XBOX EEPROM
  * Simply Press Start to bring up the Menu, Then select Lock/Unlock HDD..

3. You can create backup of your current eeprom, HDD password and various other settings.
  * Make 100% SURE that the path in the configmagic.ini file is valid and exists !!

4. Security features will not allow a "bad" eeprom update, or update eeprom if HDD is still locked..

5. You can skip the Intro and Credits Movie by pressing A while movie is playing

6. ConfigMAgic should Auto Detect your XBOX Version, but if you have problems we suggest using the "Force" mode
   by specifying your XBOX Version in the configmagic.ini - Use this if you flashed previously bad eeprom image...
   We also suggest using this setting when using the "Blind Restore"



Feedback:
---------
Pleaese send all feedback to info@team-assembly.com


Credits & Greets
****************

backup-source.com:
Our very generous Web Hosting Sponsor, Thanks a bunch !! You guys are the best !

Pr0nstar (team-assembly):
He was the brains behind our Graphics and awesome Intro/Credits Videos !!
thanks Dude !! you rule with all things Gfx ! If you need gfx for your Apps, give him a shout!

Evil1 (team-assembly):
Thanks man !! This is the guy who makes everything on our website so very functional and pretty !!

undead (team-assembly):
This is me, of course credits to me for writing the (now) 6000+ lines of c++ code that makes up
ConfgMagic ! Need any infor on how we do things or need some help.. just shout !!!!

GAMEFREAX.DE:
Our official ConfigMagic FINAL Sponsor !!  You guys help keep us alive in the scene !! THANK YOU !
This is an awsome site you can get everything related to XBOX, PS2, Sega, etc.etc... 
GO Check them out !!!

XBINS:
This is our official distribution channel !!  You guys provide the scene a extremely valueable service !
Keep up the good work ! 

SPEEDBUMP:
Our utmost greatfullness and admiration goes towards SpeedBump for all his hard work..
We learnt a lot from his HDD Password generate code.. converted to C++ objects etc.. 

MRUEL:
Thanks for the pointing us to the Pascall Code that was used for Locking/Unlocking the disk..  
Great work on UnlockX.. keep it up !

MARTIN GERDES:
An editor of the german "c't" magazine. The article we referenced was published in 
c't Magazine 11/91 "Platten-Auslese - Konfiguration von AT-Bus-Platten lesen", 
We ported this Pascall and ASM stuff to CPP to access the IDE Ports directly 


Greets Goes to:
GameFreax, XBINS, xbox-scene.com, Team XEcuter, CXBX, Team Evox, xbox-linux team, Andy, Aspect, Caustik, 
CHERRY, xboxhacker.net, XanTium, Opjose, BenJeremy, extremegaming.net,Fuzzy
