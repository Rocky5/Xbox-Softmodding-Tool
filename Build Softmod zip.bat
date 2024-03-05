@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Softmod zip
Set "root=%~dp0"

if not exist "..\Other Stuff\dev.bin" Call :Download & Exit

::Build Release build.
Set "ZipName=Xbox Softmodding Tool.zip"
Del /Q "%ZipName%" 2>NUL
Del "Files.rar" 2>NUL
Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"
::Build Files.rar
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL
::Build Softmod Save.zip
md "temp\Softmod Package\Legacy"
md "temp\Prep"
CD "temp"

::Copy
XCopy /s /y /e "..\Endgame\Endgame exploit\*" "Endgame\"
Copy "..\Endgame\payload.xbe" "Endgame\"
::Move
Move "payload.zip" "Endgame\"
CD "Endgame"
::Zip softmod save
"%Winrar%" a -x*.db -afzip "..\Softmod Package\Endgame.zip" "payload.xbe"
"%Winrar%" a -x*.db -afzip "..\Softmod Package\Endgame.zip" "helper"
"%Winrar%" a -x*.db -afzip "..\Softmod Package\Endgame.zip" "trigger"
CD "..\"

::Copy
XCopy /s /y /e "..\Save Folder\*" "UDATA\21585554\000000000000\"
XCopy /s /y "..\Other\Game Saves\Softmod\*" "UDATA\21585554\"
XCopy /s /y "..\Other\Game Saves\Zipped\*.zip" "Softmod Package\Legacy\"
XCopy /s /y "..\Documents\Installation Guide" "Installation Guide\"
::Move
Move "..\Files.rar" "UDATA\21585554\000000000000\softmod files\"

::Delete
RD /Q /S "Prep"
RD /Q /S "UDATA\21585554\000000000000\nkpatcher settings"
::Zip softmod save
"%Winrar%" a -x*.db -afzip "..\temp\Softmod Package\Legacy\Softmod Save.zip" "UDATA\21585554"
::Build Xbox Softmodding Tool.zip
::Copy
Copy "..\Documents\ReadMe.url" "ReadMe.url"
::Zip Xbox Softmodding Tool.zip
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "Installation Guide
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "Softmod Package"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Documents\Legacy"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Documents\Endgame Information.url"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Documents\ReadMe.url"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Changes.txt"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Utilites\PC\Open FTP Window.bat"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Utilites\PC\OpenDash Dualboot.Settings"

CD "..\"
RD /Q /S "temp" 2>NUL
CD "Installer Variants\UDDAE\"
"%Winrar%" a -x*.db -afzip "..\..\Extras Disc\Softmod\Dashboards\msdash\other\UDDAE-C.zip" "resoftmod dash"

CD %root%
:Download
if not exist "..\Other Stuff\dev.bin" (
	Echo:
	Echo  This is intended for people that know what they are doing.
	Echo  If you do not please hit enter to be taken to the Pre-Build versions of the files.
	Set /p "tmp="
	start "" https://drive.google.com/file/d/1_7Ra-2cKSYBjT1pVWuCRTHvDGYuxKhKk/view?usp=sharing
)