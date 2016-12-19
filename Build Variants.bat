@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Variants

Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

Call "Build Disc Save.bat"

Copy "%CD%\Save Folder\nkpatcher settings\unleashx.xbe" "%CD%\Variants\Quick Update\Prep\unleashx.xbe"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\Quick Update\Prep\Softmod Files\"
Copy "%CD%\Save Folder\nkpatcher settings\unleashx.xbe" "%CD%\Variants\Quick Upgrade\Prep\unleashx.xbe"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\Quick Upgrade\Prep\Softmod Files\"
Copy "%CD%\Save Folder\nkpatcher settings\unleashx.xbe" "%CD%\Variants\XBHDM Build\linux\E\Prep\default.xbe"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\XBHDM Build\linux\E\Prep\Softmod Files\"

CD "Variants"
"%Winrar%" a -x*.db -afzip "..\Quick Update" "Quick Update"
"%Winrar%" a -x*.db -afzip "..\Quick Upgrade" "Quick Upgrade"
"%Winrar%" a -x*.db -afzip "..\XBHDM Build" "XBHDM Build"
RD /q /s "Quick Update\Prep\Softmod Files"
Del /q /s "Quick Update\Prep\unleashx.xbe"
RD /q /s "Quick Upgrade\Prep\Softmod Files"
Del /q /s "Quick Upgrade\Prep\unleashx.xbe"
RD /q /s "XBHDM Build\linux\E\Prep\Softmod Files"
Del /q /s "XBHDM Build\linux\E\Prep\default.xbe"

CD "Attacher, Detacher"
"%Winrar%" a -x*.db -afzip "..\Extras Disc Attacher" "2016 Softmodding Tool Extras Disc"
"%Winrar%" a -x*.db -afzip "..\Extras Disc Attacher" "Detacher"