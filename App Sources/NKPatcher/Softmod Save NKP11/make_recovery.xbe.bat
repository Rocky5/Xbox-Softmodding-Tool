@Echo off
ren "config.inc" "config.inc1"
ren "config2.inc" "config.inc"
if exist "default.xbe" del /Q "default.xbe"
nasm -o default.xbe xboxapp.asm
xbedump default.xbe -habibi
del default.xbe
rename out.xbe default.xbe
ren "config.inc" "config2.inc"
ren "config.inc1" "config.inc"
pause
