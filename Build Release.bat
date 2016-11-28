@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Release

Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

If not exist "..\Release" MD "..\Release"

Call "Update Skin Version Number.bat"
Call "ReMove All Thumb.db.bat"
Call "Build Disc Save.bat"
Call "Build Softmod zip.bat"
Call "Build XISO.bat"

Copy "Changes.txt" "..\Release\"
Copy "Read Me.txt" "..\Release\"
Copy "UDDAE Instructions.txt" "..\Release\"
Copy "Run XISO from HDD.txt" "..\Release\"
Copy "Tools\thumb.jpg" "..\Release\"
Move "*.iso" "..\Release\"
Move "*.zip" "..\Release\"

Copy "%CD%\Save Folder\nkpatcher settings\unleashx.xbe" "%CD%\Variants\Quick Upgrade\Prep\unleashx.xbe"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\Quick Upgrade\Prep\Softmod Files\"
Copy "%CD%\Save Folder\nkpatcher settings\unleashx.xbe" "%CD%\Variants\XBHDM Build\linux\E\Prep\default.xbe"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\XBHDM Build\linux\E\Prep\Softmod Files\"

CD "Variants"
"%Winrar%" a -x*.db -afzip "Quick Upgrade" "Quick Upgrade"
"%Winrar%" a -x*.db -afzip "XBHDM Build" "XBHDM Build"
RD /q /s "Quick Upgrade\Prep\Softmod Files"
Del /q /s "Quick Upgrade\Prep\unleashx.xbe"
RD /q /s "XBHDM Build\linux\E\Prep\Softmod Files"
Del /q /s "XBHDM Build\linux\E\Prep\default.xbe"

Move "*.zip" "..\..\Release\"

CD "Attacher, Detacher"
"%Winrar%" a -x*.db -afzip "Extras Disc Attacher" "2016 Softmodding Tool Extras Disc"
"%Winrar%" a -x*.db -afzip "Extras Disc Attacher" "Detacher"

Move "*.zip" "..\..\..\Release\"

Explorer "..\..\..\Release\"