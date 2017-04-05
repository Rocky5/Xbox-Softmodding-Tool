=========================================
	Read Me
	Xbox Softmodding Tool v1.1
	Created by Rocky5
=========================================


=========================================
	Background:
=========================================
		I set out to try to make the softmodding process as pain free and user friendly as possible and to be honest I think I nailed it.
		You don't need to have a specific MS dashboard version or mess about in menus trying to backup your EEPROM.bin or even working out what version to install.
		I take all that and do it for you, I backup the EEPROM.bin I give you an easy to use settings menu ( NKPatcher Settings ) that has all the info you need
		to get the just of things. I have also made it as safe as I possibly can, by having recovery dashboards and fail safes in place in case you remove your
		dashboard or you format your E partition eg... you can always recover.
		
		I even added support for 15 dashboard locations, so every other softmod dashboard install location is covered.
		
		Note: Making changes in the unprotected C partition is not advised unless you know what you are doing, you will know you are in this mode if you see
		a folder named "Do not touch anything in here" lol

	
=========================================
	Main Features:
=========================================
		Supports all Xbox versions
		Supports all currently exploitable games + Tony Hawks Pro Skater 4
		Obviously runs unsigned code
		Doesn't require a certain Microsoft Dashboard version
		User friendly interface
		Information buttons on all menus
		Mandatory ShadowC partition
		Increased ShadowC partition size (485mb)
		Small footprint *
		Dualbooting support
		Simple and easy to use settings menu
		Repair missing dashboards, right from the Xbox
		Repair Softmod right from the Xbox
		Extremely simple to install
		Extremely difficult to break the softmod
		Supports the Xbox HDD partition table
		Clock auto set to 01/01/2017 (No more clock loops)
		Multiple dashboard locations built in
			> C:\XBMC\default.xbe
			> E:\XBMC\default.xbe
			> F:\XBMC\default.xbe
			> C:\dashboard\default.xbe
			> E:\dashboard\default.xbe
			> F:\dashboard\default.xbe
			> C:\dash\default.xbe
			> E:\dash\default.xbe
			> F:\dash\default.xbe
			> E:\default.xbe
			> E:\dashboard.xbe
			> C:\evoxdash.xbe
			> E:\evoxdash.xbe
			> C:\XBMC.xbe
			> E:\XBMC.xbe

		
=========================================
	NKPatcher Settings:
=========================================
	NKPatcher, this is the second stage of the Softmod. This is what patches the kernel after the NDURE exploit is executed and gives you all the lovely unsigned code and fancy features.
	NKPatcher Settings allows you to modify certain aspects of NKPatcher in a safe manor.
		
		At the top of each menus there is an "Information" button, this will give you a description of the menu you are in.

			Dashboard Settings
				> Backup, Restore or Move
					This is used to move or restore a clean copy of UnleashX, as well as backup your dashboards.
				> Dualbooting
					Enable:
						This option is used to enabled dualbooting, you get asked if you would like a dashboard setup for you when you select this option.
					Disable:
						This option is used to disabled the dualbooting process. You are also asked if you would like the alt dashboard to be removed.
					Update:
						This option is to be used if you want to use your own custom dualbooting path, you can use the "OpenDash Dualboot.settings" tool to do this, it explains things.
					Restore:
						This option will restore the stock dualboot.settings file.
			
			Fan speed
				Self explanatory.
			
			Hard Drive
				> Partition Types
					Default:
						This is the standard/default option, this allows Partition 6 ( F ) to use all available extra space.
					Partition F & G:
						This option allows a F and G partition. Where F takes upto 137GB and G takes the remaining space of the drive.
					XBPartitioner:
						This option is a placebo option, all above options will allow the partition table, I just added this for simplistic reasons.
				> Populate partitions
					E Partition:
						Creates "Applications, Emulators, Games and Homebrew" folders.
					F Partition:
						Creates "Applications, Emulators, Games and Homebrew" folders.
					G Partition:
						Creates "Applications, Emulators, Games and Homebrew" folders.
			
			Kernel Fonts
				> Advanced Features
					> Kernel Font Options
						Install Kernel Font:
							This option will install a kernel specific font. This isn't really needed but if there are boot issues this can fix the issue.	
						Restore Generic Font:
							This option will restore the generic font to the Xbox. This font works on all Xbox kernel versions and is the default font used.
					> XTF Font Delay Options
						Remove:
							Removes these delay files.
						Fix 1:
							Installs 1 delay file, in hope it fixes your boot issue.
						Fix 2:
							Installs 2 delay files, in hope that fixes your boot issue.
			
			LED colour
				Self explanatory.
				
			Video modes
				Default:
					This is the default mode, supports all video mode.
				Force Progressive:
					This option will force 480p video mode even if your game doest support it. Note: This will make the image distorted in PAL video mode.
				Flicker Filter:
					Used to change the blur amount when using 480i and 576i video signals.

			Virtual EEPROM
				Enable:
					This option will enable the virtual eeprom. This will install a copy of your Xbox eeprom.bin to the HDD and use it instead of the onboard one.
				Disable:
					This option will disable the virtual eeprom. Allowing you to edit the onboard one. ( not recommended to go modifying the eeprom if you don't know what you're doing )
				Backup EEPROM Tool:
					This option will load my "Xbox Softmod Tool Kit" and backup the eeprom. You will be taken back to the NKPatcher Settings when complete.

			Modules & Settings
				> Modules
					> Built in Modules
						These are what I use to do specific tasks.
					> User Modules
						If you make your own, you will see them here. ( you add your own to the modules folder inside the NKPatcher Settings App )
				> UnleashX Settings
					This is the settings menu for UnleashX, change network settings video settings eg...


=========================================
	Xbox Softmodding Tool Extras Disc:
=========================================
		
		Install alternative dashboards
			> Avalaunch
			> EvolutionX
			> UnleashX
			> XBMC 3.5.3
			> XBMC4Kids
			
		Install useful applications
			> DVD2Xbox
			> DVDX v2
			> EEPROM Backup Tool
			> Enigmah-X
			> NKPatcher Settings
			> Xored Mini launcher with 882 trainers
			> Note Alt Dashboards can be installed as applications.
			
		Install game mods & homebrew
			-- Homebrew --
			> Aliens versus Predator Gold
			> DoomX
			> Super Mario War 1.8
			> Super Mario War 1.8 Halloween Edition
			> xDuke
			-- Mods --
			> FIFA 07 - real gameplay mod
			> Operation Flashpoint Elite - Addons
			> Richard Burns Rally - Textures & realistic damage & physics.
			> WWE Wrestlemania XXI - CrazyChris ModPack v1, crap-loads of changes.
			
		Install Microsoft dashboards
			> Official 5960
			> My hacked 4920 version
			
		Clear the Xbox Cache
			This will clear the E:\Cache folder and the X, Y & Z partitions.
		
		Upgrade the 2014 Softmod to my Xbox Softmodding Tool
			Used to upgrade from my 2014 Softmod.
			
		Update the Xbox Softmodding Tool
			This is used to update to a newer version of the softmod. Your dashboards are not touched, I only update the backend.
		
		Upgrade Older Softmods to my Xbox Softmodding Tool	
			Used to upgrade from any other softmod to mine, or install a clean softmod.
		
		Restore your Xbox to an un-softmodded state
			I install all softmod saves and the UDDAE exploit so you can resoftmod without the need for a exploitable game.
		
		Advanced Apps
			> Chimp 261812
			> XBPartitioner
			
		NKPatcher Settings Disc version
			Has limited options, use the HDD version.
		
		As well loads of other features.			


=========================================
	Update, Upgrade & Restore info:
=========================================
		1. Updating the Xbox Softmodding Tool to the latest version:
			You can use the "Xbox Softmodding Tool Extras Disc" to update the Softmod.
			All you need to do is burn the new ISO & select the "Update Xbox Softmodding Tool" option, located in the "Advanced Features" menu, just follow the on-screen instructions.

		2. Updating the 2014 Softmod to the Xbox Softmodding Tool:
			You can use the "Xbox Softmodding Tool Extras Disc" to update the Softmod.
			All you need to do is burn the new ISO & select the "Update 2014 Softmod" option, located in the "Advanced Features" menu, just follow the on-screen instructions.

		3. Upgrade Older Softmods:
			You can use the "Xbox Softmodding Tool Extras Disc" to update older Softmods to my new softmod.
			All you need to do is burn the new ISO & select the "Upgrade Old Softmods" option, located in the "Advanced Features" menu, just follow the on-screen instructions.

		4. Restoring to a clean install of my Xbox Softmodding Tool:
			You can use the "Xbox Softmodding Tool Extras Disc" to install a clean install of my Xbox Softmodding Tool.
			All you need to do is burn the new ISO & select the "Upgrade Old Softmods" option, located in the "Advanced Features" menu, just follow the on-screen instructions.

		The great thing about the above, is it requires no FTP access or knowledge on how to update/restore softmods, great for people who are new to the Xbox scene.


=========================================
	Source code:
=========================================
		This softmod is fully open source, you can find the full source code on my GitHub with everything I have used/created in a nice organised manner.
		If you are a windows user, I have created batch files to do most of the heavy lifting, so it makes my life and yours simple when building stuff.
		
		Note: The Xbox XDK and Microsoft Visual Studio .NET 2003 is required to build some of the tools.
		
		GitHub = https://github.com/Rocky5/2016-Softmodding-Tool

		
=========================================
	Youtube Channel:
=========================================
	Here is my Youtube Channel = https://www.youtube.com/JCRocky5
	There you can find videos on "How to setup your USB Pen and install a Softmod", "How to clone your Hard drive using Chimp 261812 from start to finish." and many other crappy videos I make for the Xbox.
	
		
=========================================
	Credits:
=========================================
		I would like to thank the following, without there contributions to the Xbox scene we would not have what we have today.
		
		Original NKPatcher:
			Rmenhal without this guy we wouldn't have NKP or the advanced fonts we use today.
		NKPatcher Unofficial Edits:
			Xman954 & kingroach added some cracking features to NKPatcher.
		PBLoader - Metoo Edition:
			Team Phoenix, ldotsfan, & Rmenhal
		UnleashX:
			James for such an amazing dashboard, without his work on UnleashX there would be no Softmod installers as we know it.
		The original XBMC team
			For XBMC Shortcut XBE Source
		MS Source leak:
			& without this person we wouldn't have the bios files or NKPatchers we have today.
		Background Music
			http://www.looperman.com/users/profile/1795296 - vibes piano 3
		New save game exploit:
			Grimdoomer
		To all the tester that helped me squash bugs and improve features.
		& to anyone else I may have forgot.
