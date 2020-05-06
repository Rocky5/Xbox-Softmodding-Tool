@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Build HDD Save
Set "root=%~dp0"

if not exist "..\Other Stuff\dev.bin" Call :Download & Exit

::Build save files for Xbox.
If not exist "Files.rar" Call "Build Files.rar.bat" 2>NUL
RD /S /Q "..\Test Build\UDATA"
XCopy /s /y /e "Save Folder\*" "..\Test Build\UDATA\21585554\000000000000\"
RD /Q /S "..\Test Build\UDATA\21585554\000000000000\nkpatcher settings"
Move "Files.rar" "..\Test Build\UDATA\21585554\000000000000\softmod files\"
XCopy /s /y "Other\Game Saves\Softmod\*" "..\Test Build\UDATA\21585554\"
Explorer "..\Test Build\UDATA\21585554\000000000000\"

CD %root%
:Download
if not exist "..\Other Stuff\dev.bin" (
	Echo:
	Echo  This is intended for people that know what they are doing.
	Echo  If you do not please hit enter to be taken to the Pre-Build versions of the files.
	Set /p "tmp="
	start "" https://drive.google.com/drive/folders/0BzRN8P835YijRU94cVNNWFA1Z28?usp=sharing
)