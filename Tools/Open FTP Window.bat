@Echo off
mode con:cols=86 lines=15
Title Explorer FTP Window

if not exist "Config.ini" Goto Config_ini_Write
:start
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "IPAddress" "Config.ini"') do Set "IPAddress=%%a"
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "Version" "Config.ini"') do Set "Version=%%a"
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "Colour" "Config.ini"') do Color %%a
Echo:
Echo  Version: %Version%
Echo  Xbox IP: %IPAddress%
%windir%\explorer.exe ftp://xbox:xbox@%IPAddress%:21
timeout /t 2 >NUL
Exit

:Config_ini_Write
Echo:
Echo  Enter your Xbox IP address and press enter to finish.
Echo:
Echo  Note: You can edit the IP adress after setting it here.
Echo        By opening the Config.ini file with notepad.exe
Echo:
Set /p "IPAddress=IP address ="
if "%IPAddress%"=="" Echo The IP address cannot be blank. & cls & Goto Config_ini_Write
(
Echo [settings]
Echo IPAddress=%IPAddress%
Echo [other]
Echo Version=1.0
Echo Colour=0B
)>Config.ini
Goto start