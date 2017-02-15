@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Release

Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

If not exist "..\Release" (
	MD "..\Release\Extras Disc"
	MD "..\Release\Other"
)
Call "Update Skin Version Number.bat"
Call "ReMove All Thumb.db.bat"
Call "Build Softmod zip.bat"
Call "Build Variants.bat"
Call "Build XISO.bat"
:: Xbox Softmodding Tool
Move "Xbox Softmodding Tool.zip" "..\Release"
Copy "Changes.txt" "..\Release\"
Copy "Read Me.txt" "..\Release\"
Copy "Free FTP Programs.txt" "..\Release\"
Copy "Troubleshooting.txt" "..\Release\"
Copy "Tools\thumb.jpg" "..\Release\"
:: Extras Disc
Move "*.iso" "..\Release\Extras Disc\"
Copy "UDDAE Instructions.txt" "..\Release\Extras Disc\"
Copy "Run XISO from HDD.txt" "..\Release\Extras Disc\"
Copy "Variants\Extras Disc Attacher.zip" "..\Release\Extras Disc\"
:: Variants
Move "Quick Update.zip" "..\Release\Other"
Move "Quick Upgrade.zip" "..\Release\Other"
Move "XBHDM Build.zip" "..\Release\Other"

Explorer "..\Release\"