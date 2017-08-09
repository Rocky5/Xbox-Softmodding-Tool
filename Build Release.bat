@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Release

Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"

If not exist "..\Release" (
	MD "..\Release\Extras Disc\Documents"
	MD "..\Release\Extras Disc\Extras"
	MD "..\Release\Installer Variants"
	MD "..\Release\Utilites"
	MD "..\Release\Documents"
)
Call "Update Skin Version Number.bat"
Call "ReMove All Thumb.db.bat"
Call "Build Softmod zip.bat"
Call "Build Installer Variants.bat"
Call "Build XISO.bat"
:: Xbox Softmodding Tool
Move "Xbox Softmodding Tool.zip" "..\Release"
Copy "Changes.txt" "..\Release\"
Copy "Documents\ReadMe.url" "..\Release\Read Me.url"
Copy "Documents\Free FTP Programs.txt" "..\Release\Documents\"
Copy "Documents\How to run the exploit save.txt"" "..\Release\Documents\"
Copy "Documents\Troubleshooting.txt" "..\Release\Documents\"
Copy "Other\Graphics\thumb.jpg" "..\Release\"
:: Extras Disc
Move "*.iso" "..\Release\Extras Disc\"
Copy "Documents\UDDAE Instructions.txt" "..\Release\Extras Disc\Documents"
Copy "Documents\Run XISO from HDD.txt" "..\Release\Extras Disc\Documents"
Copy "Utilites\Xbox\Attacher, Detacher\Extras Disc Attacher.zip" "..\Release\Extras Disc\Extras"
:: Variants
Move "Quick Update.zip" "..\Release\Installer Variants"
Move "Quick Upgrade.zip" "..\Release\Installer Variants"
Move "XBHDM Build.zip" "..\Release\Installer Variants"
:: Tools
Copy "Utilites\PC\zipped\*" "..\Release\Utilites"
Explorer "..\Release\"
