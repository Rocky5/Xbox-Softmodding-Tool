@Echo off
::ren "NKP_top.asm" "NKP_top.asm1"
::ren "NKP_top after softmod.asm" "NKP_top.asm"
if exist "default.xbe" del /Q "default.xbe"
nasm -o default.xbe xboxapp.asm
xbedump default.xbe -habibi
del default.xbe
rename out.xbe default.xbe
::ren "NKP_top.asm" "NKP_top after softmod.asm"
::ren "NKP_top.asm1" "NKP_top.asm"
pause
