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
rename out.xbe "default - DB Disabled F only.xbe" >NUL


Echo Building NKPatcher 11
ren "config.inc" "config1.inc" >NUL
ren "config2.inc" "config.inc" >NUL
Echo:
nasm -o default.xbe xboxapp.asm
Echo:
Echo Signing default.xbe
Echo:
xbedump default.xbe -habibi >NUL
del default.xbe
rename out.xbe "default - DB Enabled F only.xbe" >NUL


Echo Building NKPatcher 11
ren "config.inc" "config2.inc" >NUL
ren "config3.inc" "config.inc" >NUL
Echo:
nasm -o default.xbe xboxapp.asm
Echo:
Echo Signing default.xbe
Echo:
xbedump default.xbe -habibi >NUL
del default.xbe
rename out.xbe "default - DB Disabled F & G only.xbe" >NUL


Echo Building NKPatcher 11
ren "config.inc" "config3.inc" >NUL
ren "config4.inc" "config.inc" >NUL
Echo:
nasm -o default.xbe xboxapp.asm
Echo:
Echo Signing default.xbe
Echo:
xbedump default.xbe -habibi >NUL
del default.xbe
rename out.xbe "default - DB Enabled F & G only.xbe" >NUL

Echo:

Echo:
Echo:
Echo Done
Echo:
Echo:

ren "config.inc" "config4.inc" >NUL
ren "config1.inc" "config.inc" >NUL

timeout /t 5 >NUL
