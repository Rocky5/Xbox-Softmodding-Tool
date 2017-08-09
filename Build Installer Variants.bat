@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Installer Variants

Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"

Call "Build Disc Save.bat"

Copy "%CD%\Save Folder\softmod files\UX.xbe" "%CD%\Installer Variants\Quick Update\Quick Update\unleashx.xbe"
Copy "%CD%\Extras Disc\softmod files\boot.cfg" "%CD%\Installer Variants\Quick Update\Quick Update\"
Copy "%CD%\Extras Disc\softmod files\default.xbe" "%CD%\Installer Variants\Quick Update\Quick Update\"
Copy "%CD%\Extras Disc\softmod files\NoVC.bin" "%CD%\Installer Variants\Quick Update\Quick Update\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Installer Variants\Quick Update\Quick Update\Softmod Files\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files\21585554\000000000000\softmod files\skin files" "%CD%\Installer Variants\Quick Update\Quick Update\skins\skin files\"
RD /q /s "%CD%\Installer Variants\Quick Update\Quick Update\Softmod Files\4d530017"
RD /q /s "%CD%\Installer Variants\Quick Update\Quick Update\Softmod Files\4541000d"
RD /q /s "%CD%\Installer Variants\Quick Update\Quick Update\Softmod Files\SC-NTSC"
RD /q /s "%CD%\Installer Variants\Quick Update\Quick Update\Softmod Files\SC-PAL"
RD /q /s "%CD%\Installer Variants\Quick Update\Quick Update\Softmod Files\TH4-NTSC"
RD /q /s "%CD%\Installer Variants\Quick Update\Quick Update\Softmod Files\TH4-PAL"

Copy "%CD%\Save Folder\softmod files\UX.xbe" "%CD%\Installer Variants\Quick Upgrade\Quick Upgrade\unleashx.xbe"
Copy "%CD%\Extras Disc\softmod files\boot.cfg" "%CD%\Installer Variants\Quick Upgrade\Quick Upgrade\"
Copy "%CD%\Extras Disc\softmod files\default.xbe" "%CD%\Installer Variants\Quick Upgrade\Quick Upgrade\"
Copy "%CD%\Extras Disc\softmod files\NoVC.bin" "%CD%\Installer Variants\Quick Upgrade\Quick Upgrade\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Installer Variants\Quick Upgrade\Quick Upgrade\Softmod Files\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files\21585554\000000000000\softmod files\skin files" "%CD%\Installer Variants\Quick Upgrade\Quick Upgrade\skins\skin files\"

Copy "%CD%\Save Folder\softmod files\UX.xbe" "%CD%\Installer Variants\XBHDM Build\C\installer\unleashx.xbe"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files" "%CD%\Installer Variants\XBHDM Build\C\installer\Softmod Files\"
XCopy /S /Y "%CD%\Extras Disc\softmod files\Softmod Files\21585554\000000000000\softmod files\skin files" "%CD%\Installer Variants\XBHDM Build\C\installer\skins\skin files\"

CD "Installer Variants\Quick Update"
"%Winrar%" a -x*.db -afzip "..\..\Quick Update" "Quick Update"
"%Winrar%" a -x*.db -afzip "..\..\Quick Update" "*"
CD "..\Quick Upgrade"
"%Winrar%" a -x*.db -afzip "..\..\Quick Upgrade" "Quick Upgrade"
"%Winrar%" a -x*.db -afzip "..\..\Quick Upgrade" "*"
CD "..\"
"%Winrar%" a -x*.db -afzip "..\XBHDM Build" "XBHDM Build"
RD /q /s "Quick Update\Quick Update\Softmod Files"
RD /q /s "Quick Update\Quick Update\skins\skin files"
Del /q /s "Quick Update\Quick Update\unleashx.xbe"
Del /q /s "Quick Update\Quick Update\default.xbe"
Del /q /s "Quick Update\Quick Update\boot.cfg"
Del /q /s "Quick Update\Quick Update\NoVC.bin"
RD /q /s "Quick Upgrade\Quick Upgrade\Softmod Files"
RD /q /s "Quick Upgrade\Quick Upgrade\skins\skin files"
Del /q /s "Quick Upgrade\Quick Upgrade\unleashx.xbe"
Del /q /s "Quick Upgrade\Quick Upgrade\default.xbe"
Del /q /s "Quick Upgrade\Quick Upgrade\boot.cfg"
Del /q /s "Quick Upgrade\Quick Upgrade\NoVC.bin"
RD /q /s "XBHDM Build\C\installer\Softmod Files"
RD /q /s "XBHDM Build\C\installer\skins\skin files"
Del /q /s "XBHDM Build\C\installer\unleashx.xbe"

CD "..\Utilites\Xbox\Attacher, Detacher"
If not exist "Extras Disc Attacher.zip" (
"%Winrar%" a -x*.db -afzip "..\Extras Disc Attacher" "Xbox Softmodding Tool Extras Disc"
"%Winrar%" a -x*.db -afzip "..\Extras Disc Attacher" "Detacher"
)

RD /Q /S "..\..\..\Extras Disc\softmod files\Softmod Files"