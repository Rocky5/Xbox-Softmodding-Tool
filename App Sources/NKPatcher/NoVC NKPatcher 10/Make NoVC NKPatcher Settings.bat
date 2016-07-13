@Echo off
del /Q "*.xbe" 2>NUL


:: Rename all ASM files.
rename "header.asm" "header1.asm" >NUL
rename "NoVC NKPatcher Settings.asm" "header.asm" >NUL
rename "NoVC NKPatcher Settings.inc" "config.inc" >NUL


:: Build ASM files & sign the xbe.
Echo:
Echo Building NKPatcher 10
Echo:
nasm -o default.xbe xboxapp.asm
Echo:
Echo Signing default.xbe
Echo:
xbedump default.xbe -habibi >NUL

:: Remove old default.xbe, as its now signed then rename out.xbe.
del default.xbe
rename out.xbe default.xbe >NUL
copy default.xbe install.xbe >NUL


:: Move XBE to its own folder.
md "NoVC NKPatcher Settings" >NUL
move "*.xbe" "NoVC NKPatcher Settings" >NUL


:: Rename all ASM files back.
rename "config.inc" "NoVC NKPatcher Settings.inc" >NUL
rename "header.asm" "NoVC NKPatcher Settings.asm" >NUL
rename "header1.asm" "header.asm" >NUL

pause