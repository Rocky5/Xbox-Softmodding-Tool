@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Files.rar

::Build save files for Xbox.
Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

Del "Files.rar"
CD "Files"
"%winrar%" a "..\Files.rar" "*.rar"