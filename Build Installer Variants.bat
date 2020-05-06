@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Installer Variants
Set "root=%~dp0"

if not exist "..\Other Stuff\dev.bin" Call :Download & Exit

Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"
::Build save files for Xbox.
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL

MD "Other\tmp"
XCopy /s /y /e "Save Folder\*" "Other\tmp\softmod files\21585554\000000000000\"
RD /Q /S "Other\tmp\softmod files\21585554\000000000000\nkpatcher settings"
Move "Files.rar" "Other\tmp\softmod files\21585554\000000000000\softmod files\"
XCopy /s /y "Other\Game Saves\Softmod\*" "Other\tmp\softmod files\21585554\"
XCopy /s /y "Other\Game Saves\Extracted\MA\Udata\4d530017" "Other\tmp\softmod files\4d530017\"
XCopy /s /y "Other\Game Saves\Extracted\007\Udata\4541000d" "Other\tmp\softmod files\4541000d\"
XCopy /s /y "Other\Game Saves\Extracted\SC-NTSC\UDATA\5553000c" "Other\tmp\softmod files\SC-NTSC\5553000c\"
XCopy /s /y "Other\Game Saves\Extracted\SC-PAL\UDATA\5553000c" "Other\tmp\softmod files\SC-PAL\5553000c\"
XCopy /s /y "Other\Game Saves\Extracted\TH4-NTSC\UDATA\41560017" "Other\tmp\softmod files\TH4-NTSC\41560017\"
XCopy /s /y "Other\Game Saves\Extracted\TH4-PAL\UDATA\41560017" "Other\tmp\softmod files\TH4-PAL\41560017\"
XCopy /s /y "Other\Game Saves\BFM" "Other\tmp\"

Copy "Save Folder\softmod files\UX.xbe" "Installer Variants\Quick Update\Quick Update\unleashx.xbe"
Copy "Other\tmp\boot.cfg" "Installer Variants\Quick Update\Quick Update\"
Copy "Other\tmp\default.xbe" "Installer Variants\Quick Update\Quick Update\"
Copy "Other\tmp\NoVC.bin" "Installer Variants\Quick Update\Quick Update\"
XCopy /S /Y "Other\tmp\Softmod Files" "Installer Variants\Quick Update\Quick Update\Softmod Files\"
XCopy /S /Y "Other\tmp\Softmod Files\21585554\000000000000\softmod files\skin files" "Installer Variants\Quick Update\Quick Update\skins\skin files\"
RD /q /s "Installer Variants\Quick Update\Quick Update\Softmod Files\4d530017"
RD /q /s "Installer Variants\Quick Update\Quick Update\Softmod Files\4541000d"
RD /q /s "Installer Variants\Quick Update\Quick Update\Softmod Files\SC-NTSC"
RD /q /s "Installer Variants\Quick Update\Quick Update\Softmod Files\SC-PAL"
RD /q /s "Installer Variants\Quick Update\Quick Update\Softmod Files\TH4-NTSC"
RD /q /s "Installer Variants\Quick Update\Quick Update\Softmod Files\TH4-PAL"

Copy "Save Folder\softmod files\UX.xbe" "Installer Variants\Quick Upgrade\Quick Upgrade\unleashx.xbe"
Copy "Other\tmp\boot.cfg" "Installer Variants\Quick Upgrade\Quick Upgrade\"
Copy "Other\tmp\default.xbe" "Installer Variants\Quick Upgrade\Quick Upgrade\"
Copy "Other\tmp\NoVC.bin" "Installer Variants\Quick Upgrade\Quick Upgrade\"
XCopy /S /Y "Other\tmp\Softmod Files" "Installer Variants\Quick Upgrade\Quick Upgrade\Softmod Files\"
XCopy /S /Y "Other\tmp\Softmod Files\21585554\000000000000\softmod files\skin files" "Installer Variants\Quick Upgrade\Quick Upgrade\skins\skin files\"

Copy "Save Folder\softmod files\UX.xbe" "Installer Variants\XBHDM Build\C\installer\unleashx.xbe"
XCopy /S /Y "Other\tmp\Softmod Files" "Installer Variants\XBHDM Build\C\installer\Softmod Files\"
XCopy /S /Y "Other\tmp\Softmod Files\21585554\000000000000\softmod files\skin files" "Installer Variants\XBHDM Build\C\installer\skins\skin files\"

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

RD /Q /S "..\..\..\Other\Game Saves\BFM\Softmod Files"
RD /Q /S "..\..\..\Other\tmp"

CD %root%
:Download
if not exist "..\Other Stuff\dev.bin" (
	Echo:
	Echo  This is intended for people that know what they are doing.
	Echo  If you do not please hit enter to be taken to the Pre-Build versions of the files.
	Set /p "tmp="
	start "" https://drive.google.com/drive/folders/1GO8e61Urz0Ck8110A5C5X0qxnGpLjTI8?usp=sharing
)