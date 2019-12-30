@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title Update Skin Version Number
Set "Winrar=%CD%\Other\Tools\Winrar\winrar.exe"

Set "Skin1=%CD%\Save Folder\nkpatcher Settings\skins\Dashboard"
Set "Skin2=%CD%\Save Folder\nkpatcher Settings\skins\NKPatcher Settings"
Set "Skin3=%CD%\Save Folder\nkpatcher Settings\skins\RescueDash_C"
Set "Skin4=%CD%\Save Folder\nkpatcher Settings\skins\RescueDash_E"
Set "Skin5=%CD%\Save Folder\softmod files\skins\Softmod"
Set "Skin6=%CD%\Save Folder\nkpatcher Settings\modules\resoftmod dash\skins\ReSoftmod Dash"
Set "Skin7=%CD%\Extras Disc\Softmod\skins\Factory ReSet"
Set "Skin8=%CD%\Extras Disc\Softmod\skins\ShadowC"
Set "Skin9=%CD%\Extras Disc\Softmod\skins\Softmod"
Set "Skin10=%CD%\Extras Disc\Softmod\skins\Update Softmod"
Set "Skin11=%CD%\Extras Disc\Softmod\skins\Upgrade Softmod"
Set "Skin12=%CD%\Extras Disc\Hardmod\skins\Hardmod"
Set "Skin13=%CD%\Extras Disc\FlashSM\skins\Flasher"
Set "Skin14=%CD%\Extras Disc\FlashSM\cleanup\skins\CleanupHM"
Set "Skin15=%CD%\Extras Disc\FlashSM\cleanup\skins\CleanupSM"
Set "Skin16=%CD%\Installer Variants\XBHDM Build\C\installer\skins\Softmod"
Set "Skin17=%CD%\Installer Variants\Quick Upgrade\Quick Upgrade\skins\Softmod"
Set "Skin18=%CD%\Installer Variants\Quick Update\Quick Update\skins\Softmod"
Set "Skin19=%CD%\Installer Variants\UDDAE\resoftmod dash\skins\ReSoftmod Dash"

Set "BuildFileLocation=%CD%\Other"

For /f "tokens=2,* Delims==" %%a in ('findstr /b /i /l "Version" "%BuildFileLocation%\Build Version.txt"') do Set "old_version=%%a"

:loop
CLS
Echo:
Echo Current Build Version = %old_version%
Echo:
Echo Enter New version number
Set /p "replace=|= "
If "%replace%"=="" CLS & Echo: & Echo Current Build Version = %old_version% & Echo: & Echo Enter New version number & Echo ^|= Using %old_version% as the version number. & Timeout /t 1 >NUL & goto EOF

if not exist "Other\Tools\repl.bat" exit
Call Other\Tools\repl.bat "%old_version%" "%replace%" L < "%BuildFileLocation%\Build Version.txt" >"%BuildFileLocation%\tmp.txt" & Del "%BuildFileLocation%\Build Version.txt" & rename "%BuildFileLocation%\tmp.txt" "Build Version.txt"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin1%\skin.xml" >"%Skin1%\tmp.xml" & Del "%Skin1%\skin.xml" & rename "%Skin1%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin1%\skinwide.xml" >"%Skin1%\tmp.xml" & Del "%Skin1%\skinwide.xml" & rename "%Skin1%\tmp.xml" "skinwide.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin1%\skinevox.xml" >"%Skin1%\tmp.xml" & Del "%Skin1%\skinevox.xml" & rename "%Skin1%\tmp.xml" "skinevox.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin2%\skin.xml" >"%Skin2%\tmp.xml" & Del "%Skin2%\skin.xml" & rename "%Skin2%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin3%\skin.xml" >"%Skin3%\tmp.xml" & Del "%Skin3%\skin.xml" & rename "%Skin3%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin4%\skin.xml" >"%Skin4%\tmp.xml" & Del "%Skin4%\skin.xml" & rename "%Skin4%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin5%\skin.xml" >"%Skin5%\tmp.xml" & Del "%Skin5%\skin.xml" & rename "%Skin5%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin6%\skin.xml" >"%Skin6%\tmp.xml" & Del "%Skin6%\skin.xml" & rename "%Skin6%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin7%\skin.xml" >"%Skin7%\tmp.xml" & Del "%Skin7%\skin.xml" & rename "%Skin7%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin8%\skin.xml" >"%Skin8%\tmp.xml" & Del "%Skin8%\skin.xml" & rename "%Skin8%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin9%\skin.xml" >"%Skin9%\tmp.xml" & Del "%Skin9%\skin.xml" & rename "%Skin9%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin10%\skin.xml" >"%Skin10%\tmp.xml" & Del "%Skin10%\skin.xml" & rename "%Skin10%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin11%\skin.xml" >"%Skin11%\tmp.xml" & Del "%Skin11%\skin.xml" & rename "%Skin11%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin12%\skin.xml" >"%Skin12%\tmp.xml" & Del "%Skin12%\skin.xml" & rename "%Skin12%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin13%\skin.xml" >"%Skin13%\tmp.xml" & Del "%Skin13%\skin.xml" & rename "%Skin13%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin14%\skin.xml" >"%Skin14%\tmp.xml" & Del "%Skin14%\skin.xml" & rename "%Skin14%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin15%\skin.xml" >"%Skin15%\tmp.xml" & Del "%Skin15%\skin.xml" & rename "%Skin15%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin16%\skin.xml" >"%Skin16%\tmp.xml" & Del "%Skin16%\skin.xml" & rename "%Skin16%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin17%\skin.xml" >"%Skin17%\tmp.xml" & Del "%Skin17%\skin.xml" & rename "%Skin17%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin18%\skin.xml" >"%Skin18%\tmp.xml" & Del "%Skin18%\skin.xml" & rename "%Skin18%\tmp.xml" "skin.xml"
Call Other\Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin19%\skin.xml" >"%Skin19%\tmp.xml" & Del "%Skin19%\skin.xml" & rename "%Skin19%\tmp.xml" "skin.xml"

Call Other\Tools\repl.bat "Xbox Softmodding Tool v%old_version%" "Xbox Softmodding Tool v%replace%" L < "README.md" >"tmp.txt" & Del "README.md" & rename "tmp.txt" "README.md"
Call Other\Tools\repl.bat "Xbox Softmodding Tool v%old_version%" "Xbox Softmodding Tool v%replace%" L < "Changes.txt" >"tmp.txt" & Del "Changes.txt" & rename "tmp.txt" "Changes.txt"

:EOF
CD "Installer Variants\UDDAE\"
"%Winrar%" a -x*.db -afzip "..\..\Extras Disc\Softmod\Dashboards\msdash\other\UDDAE-C.zip" "resoftmod dash"