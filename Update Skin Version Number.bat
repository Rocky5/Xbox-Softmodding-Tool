@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title Update Skin Version Number

Set "Skin1=%CD%\Save Folder\nkpatcher Settings\skins\Dashboard"
Set "Skin2=%CD%\Save Folder\nkpatcher Settings\skins\NKPatcher Settings"
Set "Skin3=%CD%\Save Folder\nkpatcher Settings\skins\RescueDash_C"
Set "Skin4=%CD%\Save Folder\nkpatcher Settings\skins\RescueDash_E"
Set "Skin5=%CD%\Save Folder\softmod files\skins\Softmod"
Set "Skin6=%CD%\Save Folder\nkpatcher Settings\modules\resoftmod dash\skins\ReSoftmod Dash"
Set "Skin7=%CD%\Extras Disc\skins\Factory ReSet"
Set "Skin8=%CD%\Extras Disc\skins\ShadowC"
Set "Skin9=%CD%\Extras Disc\skins\Softmod"
Set "Skin10=%CD%\Extras Disc\skins\Update Softmod"
Set "Skin11=%CD%\Extras Disc\skins\Upgrade Softmod"
Set "Skin12=%CD%\Variants\XBHDM Build\linux\E\Prep\skins\Softmod"
Set "Skin13=%CD%\Variants\Quick Upgrade\Prep\skins\Softmod"
Set "Skin14=%CD%\Variants\Quick Update\Prep\skins\Softmod"

For /f "tokens=2,* Delims==" %%a in ('findstr /b /i /l "Version" "Build Version.txt"') do Set "old_version=%%a"

:loop
CLS
Echo:
Echo Current Build Version = %old_version%
Echo:
Echo Enter New version number
Set /p "replace=|= "
If "%replace%"=="" CLS & Echo: & Echo Current Build Version = %old_version% & Echo: & Echo Enter New version number & Echo ^|= Using %old_version% as the version number. & Timeout /t 3 >NUL & Set "replace=%old_version%"

Call Tools\repl.bat "%old_version%" "%replace%" L < "%CD%\Build Version.txt" >"%CD%\tmp.txt" & Del "%CD%\Build Version.txt" & rename "%CD%\tmp.txt" "Build Version.txt"

Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin1%\skin.xml" >"%Skin1%\tmp.xml" & Del "%Skin1%\skin.xml" & rename "%Skin1%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin1%\skinwide.xml" >"%Skin1%\tmp.xml" & Del "%Skin1%\skinwide.xml" & rename "%Skin1%\tmp.xml" "skinwide.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin1%\skinevox.xml" >"%Skin1%\tmp.xml" & Del "%Skin1%\skinevox.xml" & rename "%Skin1%\tmp.xml" "skinevox.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin2%\skin.xml" >"%Skin2%\tmp.xml" & Del "%Skin2%\skin.xml" & rename "%Skin2%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin3%\skin.xml" >"%Skin3%\tmp.xml" & Del "%Skin3%\skin.xml" & rename "%Skin3%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin4%\skin.xml" >"%Skin4%\tmp.xml" & Del "%Skin4%\skin.xml" & rename "%Skin4%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin5%\skin.xml" >"%Skin5%\tmp.xml" & Del "%Skin5%\skin.xml" & rename "%Skin5%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin6%\skin.xml" >"%Skin6%\tmp.xml" & Del "%Skin6%\skin.xml" & rename "%Skin6%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin7%\skin.xml" >"%Skin7%\tmp.xml" & Del "%Skin7%\skin.xml" & rename "%Skin7%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin8%\skin.xml" >"%Skin8%\tmp.xml" & Del "%Skin8%\skin.xml" & rename "%Skin8%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin9%\skin.xml" >"%Skin9%\tmp.xml" & Del "%Skin9%\skin.xml" & rename "%Skin9%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin10%\skin.xml" >"%Skin10%\tmp.xml" & Del "%Skin10%\skin.xml" & rename "%Skin10%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin11%\skin.xml" >"%Skin11%\tmp.xml" & Del "%Skin11%\skin.xml" & rename "%Skin11%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin12%\skin.xml" >"%Skin12%\tmp.xml" & Del "%Skin12%\skin.xml" & rename "%Skin12%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin13%\skin.xml" >"%Skin13%\tmp.xml" & Del "%Skin13%\skin.xml" & rename "%Skin13%\tmp.xml" "skin.xml"
Call Tools\repl.bat "<Version>%old_version%</Version>" "<Version>%replace%</Version>" L < "%Skin14%\skin.xml" >"%Skin14%\tmp.xml" & Del "%Skin14%\skin.xml" & rename "%Skin14%\tmp.xml" "skin.xml"

Call Tools\repl.bat "v%old_version%" "v%replace%" L < "Read Me.txt" >"tmp.txt" & Del "Read Me.txt" & rename "tmp.txt" "Read Me.txt"
Copy /y "Read Me.txt" "README.md" >NUL
Call Tools\repl.bat "v%old_version%" "v%replace%" L < "Changes.txt" >"tmp.txt" & Del "Changes.txt" & rename "tmp.txt" "Changes.txt"