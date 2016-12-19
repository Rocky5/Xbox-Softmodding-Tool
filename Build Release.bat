@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Release

Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

If not exist "..\Release" MD "..\Release"

Call "Update Skin Version Number.bat"
Call "ReMove All Thumb.db.bat"
Call "Build Softmod zip.bat"
Call "Build Variants.bat"
Call "Build XISO.bat"

Copy "Changes.txt" "..\Release\"
Copy "Read Me.txt" "..\Release\"
Copy "UDDAE Instructions.txt" "..\Release\"
Copy "Run XISO from HDD.txt" "..\Release\"
Copy "Tools\thumb.jpg" "..\Release\"
Move "*.iso" "..\Release\"
Move "*.zip" "..\Release\"

Explorer "..\Release\"