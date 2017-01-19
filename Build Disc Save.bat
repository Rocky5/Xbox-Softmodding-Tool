@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Disc Save

::Build save files for Xbox.
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL
RD /S /Q "tmp" >NUL
XCopy /s /y /e "Save Folder\*" "tmp\21585554\000000000000\"
RD /Q /S "tmp\21585554\000000000000\nkpatcher settings"
Move "Files.rar" "tmp\21585554\000000000000\softmod files\"
XCopy /s /y "Game Saves\Softmod\*" "tmp\21585554\"

RD /Q /S "Extras Disc\softmod files\Softmod Files\4d530017"
RD /Q /S "Extras Disc\softmod files\Softmod Files\4541000d"
RD /Q /S "Extras Disc\softmod files\Softmod Files\21585554"

XCopy /s /y "Game Saves\Extracted\MA\Udata\4d530017" "Extras Disc\softmod files\Softmod Files\4d530017\"
XCopy /s /y "Game Saves\Extracted\007\Udata\4541000d" "Extras Disc\softmod files\Softmod Files\4541000d\"
XCopy /s /y "Game Saves\Extracted\SC-NTSC\UDATA\5553000c" "Extras Disc\softmod files\Softmod Files\SC-NTSC\5553000c\"
XCopy /s /y "Game Saves\Extracted\SC-PAL\UDATA\5553000c" "Extras Disc\softmod files\Softmod Files\SC-PAL\5553000c\"
XCopy /s /y /e "tmp\21585554" "Extras Disc\softmod files\Softmod Files\21585554\"

RD /q /s "tmp"