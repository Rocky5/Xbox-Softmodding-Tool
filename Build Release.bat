@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Release
Set "root=%~dp0"

if not exist "..\Other Stuff\dev.bin" Call :Download & Exit

Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"
Call "Update Skin Version Number.bat"
For /f "tokens=2,* Delims==" %%a in ('findstr /b /i /l "Version" "%root%\Other\Build Version.txt"') do Set "version=%%a"
MD "Release\Build v%version%\Extras Disc\Documents"
MD "Release\Build v%version%\Extras Disc\Extras"
MD "Release\Build v%version%\Installer Variants"
MD "Release\Build v%version%\Utilites"
MD "Release\Build v%version%\Documents"
Call "ReMove All Thumb.db.bat"
Call "Build Softmod zip.bat"
Call "Build Installer Variants.bat"
Call "Build XISO.bat"
:: Xbox Softmodding Tool
Move "Xbox Softmodding Tool.zip" "Release\Build v%version%\"
Copy "Changes.txt" "Release\Build v%version%\"
Copy "Documents\ReadMe.url" "Release\Build v%version%\Read Me.url"
Copy "Documents\Free FTP Programs.txt" "Release\Build v%version%\Documents\"
Copy "Documents\How to run the exploit save.txt"" "Release\Build v%version%\Documents\"
Copy "Documents\Troubleshooting.txt" "Release\Build v%version%\Documents\"
Copy "Other\Graphics\thumb.jpg" "Release\Build v%version%\"
:: Extras Disc
Move "*.iso" "Release\Build v%version%\Extras Disc\"
Copy "Documents\UDDAE Instructions.txt" "Release\Build v%version%\Extras Disc\Documents"
Copy "Documents\Run XISO from HDD.txt" "Release\Build v%version%\Extras Disc\Documents"
Copy "Utilites\Xbox\Attacher, Detacher\Extras Disc Attacher.zip" "Release\Build v%version%\Extras Disc\Extras"
:: Variants
Move "Quick Update.zip" "Release\Build v%version%\Installer Variants"
Move "Quick Upgrade.zip" "Release\Build v%version%\Installer Variants"
Move "XBHDM Build.zip" "Release\Build v%version%\Installer Variants"
:: Tools
Copy "Utilites\PC\zipped\*" "Release\Build v%version%\Utilites"
Explorer "Release\Build v%version%\"

CD %root%
:Download
if not exist "..\Other Stuff\dev.bin" (
	Echo:
	Echo  This is intended for people that know what they are doing.
	Echo  If you do not please hit enter to be taken to the Pre-Build versions of the files.
	Set /p "tmp="
	start "" https://drive.google.com/drive/folders/0BzRN8P835YijRU94cVNNWFA1Z28?usp=sharing
)