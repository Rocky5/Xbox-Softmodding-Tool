@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build XISO

Set "SourceDirectory=Extras Disc"
Set "OutputISOName=Xbox Softmodding Tool Extras Disc.iso"
Set "FolderToMove=Other"

Echo  About to create an Xbox ISO.
Echo:
Echo  Source Directory
Echo  %SourceDirectory%
Echo:
Echo  Output ISO Name.
Echo  %OutputISOName%
Timeout /t 3 >NUL
Call "Build Disc Save.bat"
Move "%SourceDirectory%\Softmod\%FolderToMove%" "Other\"
"Other\Tools\XDVDFS Tools\bin\windows\xdvdfs_maker.exe" "%SourceDirectory%" "%OutputISOName%"
Move "Other\%FolderToMove%" "%SourceDirectory%\Softmod\"
RD /Q /S "%SourceDirectory%\Softmod\softmod files\Softmod Files"