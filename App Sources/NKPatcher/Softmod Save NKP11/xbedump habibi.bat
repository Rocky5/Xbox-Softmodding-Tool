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
timeout /t 2 >NUL

:doit
xbedump %1 -habibi
echo;
ren default.xbe Original-default.xbe
ren out.xbe default.xbe
pause
cls
exit

:error
echo;
echo                      %what%.xbe file does not exist!
echo;
pause
cls
exit