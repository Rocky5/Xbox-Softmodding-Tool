@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build HDD Save

::Build save files for Xbox.
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL
RD /S /Q "..\Test Build\UDATA"
XCopy /s /y /e "Save Folder\*" "..\Test Build\UDATA\21585554\000000000000\"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings\configs"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings\modules"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings\Skins\Dashboard"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings\Skins\NKPatcher Settings"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings\Skins\RescueDash_C"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings\Skins\RescueDash_E"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings\toggles"
Move "Files.rar" "..\Test Build\UDATA\21585554\000000000000\softmod files\"
XCopy /s /y "Game Saves\Softmod\*" "..\Test Build\UDATA\21585554\"
Explorer "..\Test Build\UDATA\21585554\000000000000\"