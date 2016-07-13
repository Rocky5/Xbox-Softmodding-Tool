@Echo off
del /Q "*.xbe" 2>NUL

Echo:
Echo Building NKPatcher 11
Echo:
nasm -o default.xbe xboxapp.asm
Echo:
Echo Signing default.xbe
Echo:
xbedump default.xbe -habibi >NUL
del default.xbe
rename out.xbe default.xbe >NUL

Echo:

Echo Building NKPatcher 11
Echo:
nasm -o 6.xbe xboxapp.asm
Echo:
Echo Signing 6.xbe
Echo:
xbedump 6.xbe -habibi >NUL
del 6.xbe
rename out.xbe 6.xbe

Echo:

Echo Building NKPatcher 11
ren "config.inc" "config1.inc" >NUL
ren "config2.inc" "config.inc" >NUL
Echo:
nasm -o 67.xbe xboxapp.asm
Echo:
ren "config.inc" "config2.inc" >NUL
ren "config1.inc" "config.inc" >NUL
Echo Signing 67.xbe
Echo:
xbedump 67.xbe -habibi >NUL
del 67.xbe
rename out.xbe 67.xbe

Echo:

Echo:
Echo:
Echo Done
Echo:
Echo:

timeout /t 5 >NUL
