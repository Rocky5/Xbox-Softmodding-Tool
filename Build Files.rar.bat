@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Files.rar

::Build save files for Xbox.
Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"

Del "Files.rar" 2>NUL
CD "C Partition\"
"%Winrar%" a -r -x*.db -x"*empty" -m5 "nkpatcher.rar" "nkpatcher"
CD "..\Save Folder"
"%Winrar%" a -r -x*.db -x"*empty" -m5 "..\C Partition\nkp settings.rar" "nkpatcher settings"
CD "..\C Partition\"
"%Winrar%" a -r -x*.db -x"*empty" -m5 "..\Files.rar" "*.rar"
del /Q "nkpatcher.rar"
del /Q "nkp settings.rar"