@Echo off
if exist "default.xbe" del /Q "default.xbe"
nasm -o default.xbe xboxapp.asm
xbedump default.xbe -habibi
del default.xbe
rename out.xbe install.xbe
pause
