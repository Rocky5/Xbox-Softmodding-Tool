@Echo off & SetLocal EnableDelayedExpansion & mode con:cols=56 lines=15 & Color 0B & Title Dualboot.Settings Maker
del /Q "*.settings" 2>NUL

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: GUI Elements.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Set "GUI_Element_1=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴"
Set "GUI_Element_2=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 Created by Rocky5"
Set "GUI_Element_3=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 Version 1.0"
Set "GUI_Element_4=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 Error"

:Splash :D
CLS & Echo: & Echo: & Echo: & Echo:
%GUI_Element_1%
Echo  Dualboot.Settings Maker
Echo:
%GUI_Element_2%
%GUI_Element_3%
timeout /t 2 >NUL

:menu
Set "Partition="
CLS
Echo:
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  1) E Partition
Echo  2) F Partition
Echo  3) G Partition
Echo:
%GUI_Element_1%
Set /p "Partition=- "
if "%Partition%"=="1" Set "Partition=E" & Goto menu2
if "%Partition%"=="2" Set "Partition=F" & Goto menu2
if "%Partition%"=="3" Set "Partition=G" & Goto menu2
if "%Partition%"=="SC" Set "Partition=Unprotected" & Goto menu2
Echo  Error: Not a valid Option.
Timeout /t 3 >NUL
Goto menu

:menu2
Set "Folder="
CLS
Echo:
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  Enter the Dashboard Folder name/path.
Echo  Example ^= E:\Applications\XBMC4Xbox\
Echo:
%GUI_Element_1%
Set /p "Folder=%Partition%:\"
if not "%Folder%"=="" Goto PathCheck
Echo  Error: Not a valid Option.
Timeout /t 3 >NUL
Goto menu2

:pathcheck
Set "Foldercheck=%Folder:~-1%"
if "%FolderCheck%"=="\" Set "Folder=%Folder:~0,-1%"
if "%FolderCheck%"=="/" Set "Folder=%Folder:~0,-1%"

:menu3
Set "XBE="
CLS
Echo:
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  Enter the XBE name.
Echo  Blank ^= Default.xbe
Echo:
%GUI_Element_1%
Set /p "XBE=%Partition%:\%Folder%\"
if "%XBE%"=="%XBE:~0,-4%.xbe" Goto review
if "%XBE%"=="" Set "XBE=Default.xbe" & Goto review
Echo  Error: Not a valid Option.
Timeout /t 3 >NUL
Goto menu3

:review
Set "OK="
CLS
Echo:
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  Is this path correct?
Echo  %Partition%:\%Folder%\%XBE%
Echo:
%GUI_Element_1%
Set /p "OK=[Y/N] "
if "%OK%"=="y" Goto FTP
if "%OK%"=="Y" Goto FTP
if "%OK%"=="n" Goto menu
if "%OK%"=="N" Goto menu
Echo  Error: Not a valid Option.
Timeout /t 3 >NUL
Goto review


:FTP
Set "FTP="
CLS
Echo:
Echo:
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  Would you like me to FTP the file over^?
Echo:
%GUI_Element_1%
Set /p "FTP=[Y/N] "
if "%FTP%"=="y" Goto ip
if "%FTP%"=="Y" Goto ip
if "%FTP%"=="n" Call :build
if "%FTP%"=="N" Call :build
Echo  Error: Not a valid Option.
Timeout /t 3 >NUL
Goto FTP

:ip
Set "IP="
CLS
Echo:
Echo:
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  Enter your Xbox IP address
Echo:
%GUI_Element_1%
Set /p "ip=- "


:ipconfirm
Set "yup= "
CLS
Echo:
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  Is this the correct IP?
Echo  %IP%
Echo:
%GUI_Element_1%
Set /p "yup=[Y/N] "
if "%yup%"=="y" Goto FTPfile
if "%yup%"=="Y" Goto FTPfile
if "%yup%"=="n" Goto ip
if "%yup%"=="N" Goto ip
Echo  Error: Not a valid Option.
Timeout /t 3 >NUL
Goto ipconfirm

:FTPfile
Call :tmp
Call :build
:buildFTP
FTP -i -s:tools\tmp2 -v
del /Q "tools\tmp2"
del /Q "tools\Dualboot.settings"
Goto Done


:Done
mode con:cols=58 lines=15 & Set "GUI_Element_1=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴"
CLS
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo    Transfered "Dualboot.settings" to the root of the E
Echo    partition.
Echo:
Echo    Now load NKPatcher Setting on the Xbox ^& select
Echo    Dashboards ^> Dualbooting ^> Update Dualboot.settings
Echo:
%GUI_Element_1%
Call :Exit

:nup
mode con:cols=59 lines=15 & Set "GUI_Element_1=Echo 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�"
CLS
Echo:
Echo:
Echo:
%GUI_Element_1%
Echo  You will need to manually transfer "Dualboot.settings" to
Echo  to the root of the E partition.
Echo:
Echo  Afterwards load NKPatcher Setting on the Xbox ^& select
Echo  Dashboards ^> Dualbooting ^> Update Dualboot.settings
Echo:
%GUI_Element_1%
Call :Exit

:Exit
Echo:
Timeout /t 10 >NUL
exit

:build
(
Echo %%define Partition '%Partition%'
Echo %%define XBE_Name '%XBE%'
Echo %%define Dash_Folder '%Folder%'
)>tools\tmp
Tools\nasm -o "tools\Dualboot.settings" "Tools\default_Settings"
del /Q "tools\tmp"
if "%FTP%"=="y" Goto buildFTP
if "%FTP%"=="Y" Goto buildFTP
if "%FTP%"=="n" Move "tools\Dualboot.settings" ".\" & Goto nup
if "%FTP%"=="N" Move "tools\Dualboot.settings" ".\" & Goto nup

:tmp
(
Echo open %IP%
Echo xbox
Echo xbox
Echo put tools\Dualboot.settings /E/dualboot.settings
Echo bye
)>tools\tmp2
