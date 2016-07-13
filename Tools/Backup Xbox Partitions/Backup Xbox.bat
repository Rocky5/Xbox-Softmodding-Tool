@Echo off & mode con:cols=86 lines=15 & Title Backup Xbox Partitions

CD %~dp0

if not exist "Config.ini" Goto Config_ini_IP

::::::::::::::::::::::::::::::::::::::
:: Version Number & Colour
::::::::::::::::::::::::::::::
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "IP Address" "Config.ini"') do Set "IP=%%a"
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "Mirror Mode" "Config.ini"') do Set "Mirror=%%a"
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "Version" "Config.ini"') do Set "Version=%%a"
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "Colour" "Config.ini"') do Color %%a
::For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "LFTP Path" "Config.ini"') do Set "lftp=%%a"
Set "lftp=Tools\lftp.exe"

:: Directories
Set "Location1=C"
Set "Location2=E"
Set "Location3=F"
Set "Location4=G"
Set "Output_Folder1=C"
Set "Output_Folder2=E"
Set "Output_Folder3=F"
Set "Output_Folder4=G"
Set "Backup_All="
Set "Backup_C="
Set "Backup_E="
Set "Backup_F="
Set "Backup_G="

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: GUI Elements.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Set "GUI_Element_1=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴"
Set "GUI_Element_2=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 Created by Rocky5"
Set "GUI_Element_3=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 Version %Version%"
Set "GUI_Element_4=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 Error"


if "%Mirror%"=="Yes" (
set "vars=-c -v -e"
set "var2=Mirror"
set "var3=Mirroring"
set "var4=Checking for changes, please wait..."
Goto Splash :D
)

set "vars=-c -v"
set "var2=Backup"
set "var3=Backing up"
set "var4=This may take some time."

:Splash :D
CLS & Echo: & Echo: & Echo: & Echo:
%GUI_Element_1%
Echo  %var2% Xbox Partitions
Echo:
%GUI_Element_2%
%GUI_Element_3%
timeout /t 2 >NUL

:menu
Set "pick="
CLS
Echo:
Echo:
%GUI_Element_1%
if "%Backup_C%"==""  Echo  1) %var2% C Partition
if "%Backup_C%"=="1" Echo  1) %var2% C Partition o
if "%Backup_E%"==""  Echo  2) %var2% E Partition
if "%Backup_E%"=="1" Echo  2) %var2% E Partition o
if "%Backup_F%"==""  Echo  3) %var2% F Partition
if "%Backup_F%"=="1" Echo  3) %var2% F Partition o
if "%Backup_G%"==""  Echo  4) %var2% G Partition
if "%Backup_G%"=="1" Echo  4) %var2% G Partition o
Echo:
if "%Backup_All%"=="" Echo  5) %var2% All Partitions
if "%Backup_All%"=="1" Echo  5) %var2% All Partitions o
Echo:
Echo  Type - S - to start the %var2% process
Echo:
%GUI_Element_1%
Set /p "pick="
:: 1 = true / 0 = false
if "%Backup_C%"=="" if "%pick%"=="1" Set "Backup_C=1" & Goto menu
if "%Backup_E%"=="" if "%pick%"=="2" Set "Backup_E=1" & Goto menu
if "%Backup_F%"=="" if "%pick%"=="3" Set "Backup_F=1" & Goto menu
if "%Backup_G%"=="" if "%pick%"=="4" Set "Backup_G=1" & Goto menu
if "%Backup_All%"=="" if "%pick%"=="5" Set "Backup_All=1" & Set "Backup_C=1" & Set "Backup_E=1" & Set "Backup_F=1" & Set "Backup_G=1" & Goto menu
if "%Backup_C%"=="1" if "%pick%"=="1" Set "Backup_C=" & Goto menu
if "%Backup_E%"=="1" if "%pick%"=="2" Set "Backup_E=" & Goto menu
if "%Backup_F%"=="1" if "%pick%"=="3" Set "Backup_F=" & Goto menu
if "%Backup_G%"=="1" if "%pick%"=="4" Set "Backup_G=" & Goto menu
if "%Backup_All%"=="1" if "%pick%"=="5" Call:reset_vars
if "%pick%"=="s" goto start
if "%pick%"=="S" goto start

:reset_vars
Set "Backup_All="
Set "Backup_C="
Set "Backup_E="
Set "Backup_F="
Set "Backup_G="
del /q Tools\tmp
Goto menu

:start
:: Code
cls
Echo:
Echo:
%GUI_Element_1%
Echo  About to %var2%
Echo:
if "%Backup_C%"=="1" Echo  %Output_Folder1% Partition
if "%Backup_E%"=="1" Echo  %Output_Folder2% Partition
if "%Backup_F%"=="1" Echo  %Output_Folder3% Partition
if "%Backup_G%"=="1" Echo  %Output_Folder4% Partition
Echo:
Echo  This can take some time.
Echo:
%GUI_Element_1%
timeout /t 5 >NUL & cls

if "%Backup_C%"=="1" (
cls
Echo:
Echo  %var3% %Output_Folder1% Partition.
Echo:
Echo  %var4%
Echo:
(
Echo open -u xbox,xbox -p 21 %IP%
Echo mirror %vars% "/%Output_Folder1%" "/%var2%/%Output_Folder1%"
Echo exit
)>Tools\tmp
%lftp% -f Tools/tmp
Echo:
)

if "%Backup_E%"=="1" (
cls
Echo:
Echo  %var3% %Output_Folder2% Partition.
Echo:
Echo  %var4%
Echo:
(
Echo open -u xbox,xbox -p 21 %IP%
Echo mirror %vars% "/%Output_Folder2%" "/%var2%/%Output_Folder2%"
Echo exit
)>Tools\tmp
%lftp% -f Tools/tmp
Echo:
)

if "%Backup_F%"=="1" (
cls
Echo:
Echo  %var3% %Output_Folder3% Partition.
Echo:
Echo  %var4%
Echo:
(
Echo open -u xbox,xbox -p 21 %IP%
Echo mirror %vars% "/%Output_Folder3%" "/%var2%/%Output_Folder3%"
Echo exit
)>Tools\tmp
%lftp% -f Tools/tmp
Echo:
)

if "%Backup_G%"=="1" (
cls
Echo:
Echo  %var3% %Output_Folder4% Partition.
Echo:
Echo  %var4%
Echo:
(
Echo open -u xbox,xbox -p 21 %IP%
Echo mirror %vars% "/%Output_Folder4%" "/%var2%/%Output_Folder4%"
Echo exit
)>Tools\tmp
%lftp% -f Tools/tmp
Echo:
)

if not "%Backup_E%"=="1" Call:reset
:: Remove empty folders from TDAtA & UDATA
CLS & Echo: & Echo: & Echo: & Echo:
%GUI_Element_1%
Echo  Removing unused directories from TDATA
Echo:
%GUI_Element_1%
CD "%var2%\E\TDATA\"
for /f "delims=" %%d in ('dir /s /b /ad ^| sort /r') do rd /q "%%d" 2>NUL
CD %~dp0 & CD "%var2%\E\"
MD Check
for /f "delims=" %%d in ('dir /b "UDATA\*"') do Echo F | xcopy /S /I /Y "UDATA\%%d" "Check\%%d" >NUL
CLS & Echo: & Echo: & Echo: & Echo:
%GUI_Element_1%
Echo  Removing unused directories from UDATA
Echo:
%GUI_Element_1%
CD %~dp0 & CD "%var2%\E\Check\"
(
del /q /s "*.xbx" >NUL
for /f "delims=" %%d in ('dir /s /b /ad ^| sort /r') do rd /q "%%d"
for /f "delims=" %%d in ('dir /b /ad "*"') do Echo %%d>>List.txt
for /f "delims=" %%d in ('dir /b "*.profile"') do Echo %%d>>List.txt
for /f "delims=" %%d in ('dir /b "*.XBN"') do Echo %%d>>List.txt
)2>NUL
CD %~dp0 & CD "%var2%\E\"
md "Not Empty"
for /f "delims=" %%d in (Check\list.txt) do move "UDATA\%%d" "Not Empty" >NUL
rd /q /s "Check" 2>NUL
rd /q /s "UDATA" 2>NUL
ren "Not Empty" "UDATA" >NUL
timeout /t 3 >NUL

:reset
Goto reset_vars

:Config_ini_IP
CLS
Echo:
Echo:
%GUI_Element_1%
Echo  Enter your Xbox IP address
Echo  Example: 192.168.0.6
Echo:
%GUI_Element_1%
Set /p "IP="
if not "%IP%"=="" Goto Config_ini_Mirror
Echo  Error, cannot be blank.
timeout /t 3 >NUL
goto Config_ini_IP

:Config_ini_Mirror
CLS
Echo:
Echo:
%GUI_Element_1%
Echo  Would you like to mirror your Xbox?
Echo  This will keep track of files on the Xbox.
Echo  So if you remove something it will remove it from your backup.
Echo:
%GUI_Element_1%
Set /p "Mirror=[Y/N]"
if "%Mirror%"=="Y" Set "Mirror=Yes" & Goto Config_ini_Write
if "%Mirror%"=="y" Set "Mirror=Yes" & Goto Config_ini_Write
if "%Mirror%"=="N" Set "Mirror=No" & Goto Config_ini_Write
if "%Mirror%"=="n" Set "Mirror=No" & Goto Config_ini_Write
Echo  Error, cannot be blank.
timeout /t 3 >NUL
goto Config_ini_Mirror

:Config_ini_Write
(
Echo [settings]
Echo IP Address=%IP%
Echo Mirror Mode=%mirror%
Echo [other]
Echo Version=1.0
Echo Colour=0B
)>Config.ini