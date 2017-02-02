@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Variants

Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

Call "Build Disc Save.bat"

Copy "%CD%\Save Folder\softmod files\UX.xbe" "%CD%\Variants\Quick Update\Prep\unleashx.xbe"
Copy "%CD%\Extras Disc\softmod files\boot.cfg" "%CD%\Variants\Quick Update\Prep\"
Copy "%CD%\Extras Disc\softmod files\default.xbe" "%CD%\Variants\Quick Update\Prep\"
Copy "%CD%\Extras Disc\softmod files\NoVC.bin" "%CD%\Variants\Quick Update\Prep\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\Quick Update\Prep\Softmod Files\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files\21585554\000000000000\softmod files\skin files" "%CD%\Variants\Quick Update\Prep\skins\skin files\"
RD /q /s "%CD%\Variants\Quick Update\Prep\Softmod Files\4d530017"
RD /q /s "%CD%\Variants\Quick Update\Prep\Softmod Files\4541000d"
RD /q /s "%CD%\Variants\Quick Update\Prep\Softmod Files\SC-NTSC"
RD /q /s "%CD%\Variants\Quick Update\Prep\Softmod Files\SC-PAL"

Copy "%CD%\Save Folder\softmod files\UX.xbe" "%CD%\Variants\Quick Upgrade\Prep\unleashx.xbe"
Copy "%CD%\Extras Disc\softmod files\boot.cfg" "%CD%\Variants\Quick Upgrade\Prep\"
Copy "%CD%\Extras Disc\softmod files\default.xbe" "%CD%\Variants\Quick Upgrade\Prep\"
Copy "%CD%\Extras Disc\softmod files\NoVC.bin" "%CD%\Variants\Quick Upgrade\Prep\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\Quick Upgrade\Prep\Softmod Files\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files\21585554\000000000000\softmod files\skin files" "%CD%\Variants\Quick Upgrade\Prep\skins\skin files\"

Copy "%CD%\Save Folder\softmod files\UX.xbe" "%CD%\Variants\XBHDM Build\linux\E\Prep\default.xbe"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Variants\XBHDM Build\linux\E\Prep\Softmod Files\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files\21585554\000000000000\softmod files\skin files" "%CD%\Variants\XBHDM Build\linux\E\Prep\skins\skin files\"

CD "Variants"

"%Winrar%" a -x*.db -afzip "..\Quick Update" "Quick Update"
"%Winrar%" a -x*.db -afzip "..\Quick Upgrade" "Quick Upgrade"
"%Winrar%" a -x*.db -afzip "..\XBHDM Build" "XBHDM Build"
RD /q /s "Quick Update\Prep\Softmod Files"
RD /q /s "Quick Update\Prep\skins\skin files"
Del /q /s "Quick Update\Prep\unleashx.xbe"
Del /q /s "Quick Update\Prep\default.xbe"
Del /q /s "Quick Update\Prep\boot.cfg"
Del /q /s "Quick Update\Prep\NoVC.bin"
RD /q /s "Quick Upgrade\Prep\Softmod Files"
RD /q /s "Quick Upgrade\Prep\skins\skin files"
Del /q /s "Quick Upgrade\Prep\unleashx.xbe"
Del /q /s "Quick Upgrade\Prep\default.xbe"
Del /q /s "Quick Upgrade\Prep\boot.cfg"
Del /q /s "Quick Upgrade\Prep\NoVC.bin"
RD /q /s "XBHDM Build\linux\E\Prep\Softmod Files"
RD /q /s "XBHDM Build\linux\E\Prep\skins\skin files"
Del /q /s "XBHDM Build\linux\E\Prep\default.xbe"

If not exist "Extras Disc Attacher.zip" (
CD "Attacher, Detacher"
"%Winrar%" a -x*.db -afzip "..\Extras Disc Attacher" "2016 Softmodding Tool Extras Disc"
"%Winrar%" a -x*.db -afzip "..\Extras Disc Attacher" "Detacher"
)

RD /Q /S "..\Extras Disc\softmod files\Softmod Files"