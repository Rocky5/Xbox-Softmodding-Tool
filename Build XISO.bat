@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build XISO
Set "root=%~dp0"

if not exist "..\Other Stuff\dev.bin" Call :Download & Exit

Set "SourceDirectory=Extras Disc"
Set "OutputISOName=Xbox Softmodding Tool Extras Disc.iso"

Echo  About to create an Xbox ISO.
Echo:
Echo  Source Directory
Echo  %SourceDirectory%
Echo:
Echo  Output ISO Name.
Echo  %OutputISOName%
Timeout /t 3 >NUL
Call "Build Disc Save.bat"
"Other\Tools\XDVDFS Tools\bin\windows\xdvdfs_maker.exe" "%SourceDirectory%" "%OutputISOName%"
RD /Q /S "%SourceDirectory%\Softmod\softmod files\Softmod Files"

CD %root%
:Download
if not exist "..\Other Stuff\dev.bin" (
	Echo:
	Echo  This is intended for people that know what they are doing.
	Echo  If you do not please hit enter to be taken to the Pre-Build versions of the files.
	Set /p "tmp="
	start "" https://drive.google.com/drive/folders/1xTpK-VoZX_uzyztk3zdnDaOdEcfOqPJb?usp=sharing
)