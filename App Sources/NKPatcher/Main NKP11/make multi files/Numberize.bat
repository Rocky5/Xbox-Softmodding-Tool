@Echo off

Set "Total=12"
Set "Value=0"

:Loop

Copy "%~nx1" "%~n1%Value%.bin"

Set /a "Value+=1"

if not "%Value%"=="%Total%" goto Loop

pause