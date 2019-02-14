@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=100 lines=10 & Color 0B
Title ReMove All Thumb.db Files

Attrib /s -r -h -s "desktop.ini" >NUL
Attrib /s -r -h -s "Thumbs.db" >NUL
Del /Q /S "desktop.ini" 2>NUL
Del /Q /S "Thumbs.db" 2>NUL