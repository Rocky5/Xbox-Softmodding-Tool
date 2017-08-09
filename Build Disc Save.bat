@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build Disc Save
::Build save files for Xbox.
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL
::Build Softmod Save
Echo %CD%
RD /Q /S "Extras Disc\softmod files\Softmod Files\4d530017"
RD /Q /S "Extras Disc\softmod files\Softmod Files\4541000d"
RD /Q /S "Extras Disc\softmod files\Softmod Files\21585554"
XCopy /s /y /e "Save Folder\*" "Extras Disc\softmod files\Softmod Files\21585554\000000000000\"
RD /Q /S "Extras Disc\softmod files\Softmod Files\21585554\000000000000\nkpatcher settings"
Move "Files.rar" "Extras Disc\softmod files\Softmod Files\21585554\000000000000\softmod files\"
XCopy /s /y "Other\Game Saves\Softmod\*" "Extras Disc\softmod files\Softmod Files\21585554\"
XCopy /s /y "Other\Game Saves\Extracted\MA\Udata\4d530017" "Extras Disc\softmod files\Softmod Files\4d530017\"
XCopy /s /y "Other\Game Saves\Extracted\007\Udata\4541000d" "Extras Disc\softmod files\Softmod Files\4541000d\"
XCopy /s /y "Other\Game Saves\Extracted\SC-NTSC\UDATA\5553000c" "Extras Disc\softmod files\Softmod Files\SC-NTSC\5553000c\"
XCopy /s /y "Other\Game Saves\Extracted\SC-PAL\UDATA\5553000c" "Extras Disc\softmod files\Softmod Files\SC-PAL\5553000c\"
XCopy /s /y "Other\Game Saves\Extracted\TH4-NTSC\UDATA\41560017" "Extras Disc\softmod files\Softmod Files\TH4-NTSC\41560017\"
XCopy /s /y "Other\Game Saves\Extracted\TH4-PAL\UDATA\41560017" "Extras Disc\softmod files\Softmod Files\TH4-PAL\41560017\"