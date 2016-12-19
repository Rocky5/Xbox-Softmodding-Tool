@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Build Installation Guide.rar

::Build save Installation Guide for Xbox.
Set "Winrar=%CD%\Tools\Winrar\winrar.exe"

Del "Installation Guide.rar"
CD "Installation Guide"
"%winrar%" a -x*.db -m5 "..\Installation Guide.rar" "*"
"%winrar%" a -x*.db -m5 "..\Installation Guide.rar" "files\*"