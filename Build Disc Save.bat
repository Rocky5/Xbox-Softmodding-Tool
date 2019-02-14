@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Disc Save
::Build save files for Xbox.
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL
::Build Softmod Save
Set "ZipName=Xbox Softmodding Tool.zip"
Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"
Echo %CD%
RD /Q /S "Extras Disc\softmod files"
MD "Extras Disc\softmod files"
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
Copy "Save Folder\softmod files\UX.xbe" "Other\tmp\unleashx.xbe"
CD "Other\tmp"
::Rar Xbox Softmodding Tool.zip
"%Winrar%" a -r -y -x*.db -afzip "..\..\Extras Disc\softmod files\%ZipName%" "*"
CD ..\..\
RD /Q /S "Other\tmp"