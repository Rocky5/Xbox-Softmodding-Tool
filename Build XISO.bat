@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build XISO

Echo  About to create an Xbox ISO.
Echo:
Echo  Source Directory
Echo  Extras Disc
Echo:
Echo  Output ISO Name.
Echo  2016 Softmodding Tool Extras Disc.iso
Timeout /t 3 >NUL
"Tools\XDVDFS Tools\bin\windows\xdvdfs_maker.exe" -oRDer "Tools\2016 Disc layout.txt" "Extras Disc" "2016 Softmodding Tool Extras Disc.iso"