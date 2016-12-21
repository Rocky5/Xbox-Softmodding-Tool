@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Files.rar

::Build save files for Xbox.
Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

Del "Files.rar" 2>NUL
CD "C Partition\"
"%Winrar%" a -x*.db -x"*empty" -m5 "nkpatcher.rar" "nkpatcher"
CD "..\Save Folder"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings\configs"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings\modules"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings\skins\Dashboard"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings\skins\NKPatcher Settings"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings\skins\RescueDash_C"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings\skins\RescueDash_E"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings\toggles"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "softmod files\Splash.jpg"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "softmod files\videoset.dat"
CD "..\C Partition\"
"%Winrar%" a -x*.db -x"*empty" -m5 "..\Files.rar" "*.rar"
del /Q "nkpatcher.rar"
del /Q "nkp settings.rar"