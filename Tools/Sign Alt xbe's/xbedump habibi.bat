@echo off

:main
echo;
echo                     ษออออออออออออออออออออออออออออออออออออป
echo                     บ                                    บ
echo                     บ        Sign edited xbe files       บ
echo                     บ                                    บ
echo                     ศออออออออออออออออออออออออออออออออออออผ
echo              ษออออออออออออออออออออออออออออออออออออออออออออออออออป
echo              บ                                                  บ
echo              บ      Put the edited xbe file in this folder      บ
echo              บ   and enter the name of the xbe file to Sign...  บ
echo              บ                                                  บ
echo              ศออออออออออออออออออออออออออออออออออออออออออออออออออผ
echo;
::timeout /t 2 >NUL

for /f "tokens=*" %%a in ('dir /b /s "disabled.xbe"') do (
xbedump "%%a" -habibi
echo:
echo  Disabled.xbe
echo  %%~pa
echo:
del "%%a"
ren out.xbe disabled.xbe >NUL
move "disabled.xbe" "%%~pa" >NUL
timeout /t 0 >NUL
)
for /f "tokens=*" %%a in ('dir /b /s "enabled.xbe"') do (
xbedump "%%a" -habibi
echo:
echo  Enabled.xbe
echo  %%~pa
echo:
del "%%a"
ren out.xbe enabled.xbe >NUL
move "enabled.xbe" "%%~pa" >NUL
timeout /t 0 >NUL
)