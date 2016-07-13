@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Softmod zip

::Build Release build.

Set "ZipName=2016 Softmodding Tool.zip"

Del /Q "%ZipName%" 2>NUL
Del "Files.rar" 2>NUL
Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

CD "Files"
Echo %CD%
"%Winrar%" a -x*.db "..\Files.rar" "*.rar"
CD ..\

Echo %CD%
RD /S /Q "UDATA"
XCopy /s /y /e "Save Folder\*" "UDATA\21585554\000000000000\"
XCopy /s /y "Installation Guide" "temp\Installation Guide\"
XCopy /s /y "Game Saves\Zipped\*.zip" "temp\Softmod Package\"
Move "Files.rar" "UDATA\21585554\000000000000\softmod files\"
Move "Save Folder\nkpatcher Settings\skins\Softmod\softmod.zip" "UDATA\21585554\000000000000\nkpatcher Settings\skins\Softmod\"
XCopy /s /y "Game Saves\Softmod\*" "UDATA\21585554\"

"%Winrar%" a -x*.db -afzip "Softmod Save.zip" "UDATA\21585554"

Move "Softmod Save.zip" "temp\Softmod Package\"

CD Temp
Echo %CD%
"%Winrar%" a -x*.db -afzip "%ZipName%" "Installation Guide
"%Winrar%" a -x*.db -afzip "%ZipName%" "Softmod Package"
Move "%ZipName%" "..\"

CD ..\
Echo %CD%
"%Winrar%" a -x*.db -afzip "%ZipName%" "Read Me.txt"
"%Winrar%" a -x*.db -afzip "%ZipName%" "Changes.txt"
"%Winrar%" a -x*.db -afzip "%ZipName%" "Tools\OpenDash Dualboot.Settings"
::"%Winrar%" a -x*.db -afzip "%ZipName%" "Tools\Backup Xbox Partitions"

RD /Q /S "UDATA"
RD /Q /S "temp"