@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build XISO

Echo  About to create an Xbox ISO.
Echo:
Echo  Source Directory
Echo  Extras Disc
Echo:
Echo  Output ISO Name.
Echo  Xbox Softmodding Tool Extras Disc.iso
Timeout /t 3 >NUL
Call "Build Disc Save.bat"
Move "Extras Disc\Games" ""
"Tools\XDVDFS Tools\bin\windows\xdvdfs_maker.exe" "Extras Disc" "Xbox Softmodding Tool Extras Disc.iso"
Move "Games" "Extras Disc"
RD /Q /S "Extras Disc\softmod files\Softmod Files"