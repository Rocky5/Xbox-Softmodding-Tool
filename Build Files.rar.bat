@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Files.rar

::Build save files for Xbox.
Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"

Del "Files.rar" 2>NUL
CD "C Partition\"
"%Winrar%" a -r -x*.db -x"*empty" "nkpatcher.rar" "nkpatcher"
CD "xboxdash"
"%Winrar%" a -r -x*.db -x"*empty" "..\xboxdash.rar" ""
CD "..\..\Save Folder"
"%Winrar%" a -r -x*.db -x"*empty" "..\C Partition\nkp settings.rar" "nkpatcher settings"
CD "..\C Partition\"
"%Winrar%" a -r -x*.db -x"*empty" "..\Files.rar" "*.rar"
del /Q "xboxdash.rar"
del /Q "nkpatcher.rar"
del /Q "nkp settings.rar"