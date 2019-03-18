@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Softmod zip

::Build Release build.
Set "ZipName=Xbox Softmodding Tool.zip"
Del /Q "%ZipName%" 2>NUL
Del "Files.rar" 2>NUL
Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"
::Build Files.rar
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL
::Build Softmod Save.zip
md "temp"
CD "temp"
::Copy
XCopy /s /y /e "..\Save Folder\*" "UDATA\21585554\000000000000\"
XCopy /s /y "..\Other\Game Saves\Softmod\*" "UDATA\21585554\"
XCopy /s /y "..\Other\Game Saves\Zipped\*.zip" "Softmod Package\"
XCopy /s /y "..\Documents\Installation Guide" "Installation Guide\"
::Move
Move "..\Files.rar" "UDATA\21585554\000000000000\softmod files\"
::Delete
RD /Q /S "UDATA\21585554\000000000000\nkpatcher settings"
::Rar softmod save
"%Winrar%" a -x*.db -afzip "..\temp\Softmod Package\Softmod Save.zip" "UDATA\21585554"
::Build Xbox Softmodding Tool.zip
::Copy
Copy "..\Documents\ReadMe.url" "ReadMe.url"
::Rar Xbox Softmodding Tool.zip
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "Installation Guide
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "Softmod Package"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Documents\Troubleshooting.txt"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Documents\How to run the exploit save.txt"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "ReadMe.url"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Changes.txt"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Utilites\PC\Open FTP Window.bat"
"%Winrar%" a -x*.db -afzip "..\%ZipName%" "..\Utilites\PC\OpenDash Dualboot.Settings"
CD "..\"
RD /Q /S "temp" 2>NUL
CD "Installer Variants\UDDAE\"
"%Winrar%" a -x*.db -afzip "..\..\Extras Disc\Softmod\Dashboards\msdash\other\UDDAE-C.zip" "resoftmod dash"