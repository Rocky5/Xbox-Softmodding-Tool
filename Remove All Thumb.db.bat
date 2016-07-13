@Echo off & SetLocal EnableDelayedExpansion & Mode con:cols=56 lines=7 & Color 0B
Title ReMove All Thumb.db Files

Attrib /s -r -h -s "Thumbs.db"
Del /Q /S "Thumbs.db"