XDVDFS Tools v2.1
-----------------

Changes :-

+ LayoutDumper.xbe now detects the presence of
  security place holders to determine if the disc
  being dumped is an original or backup. This had
  to be done because some backups were being detected
  as originals.

+ LayoutDumper.xbe can now be run from within
  a sub directory on any drive on the XBOX and
  .lyt files will be created in the same location
  as the tool itself.

+ LayoutDumper.xbe now reports the correct location
  of the .lyt file on screen when running from a
  sub directory.

+ LayoutDumper.xbe no longer locks up when you
  dont have a controller plugged into port 1.

+ LayoutDumper.xbe now truncates filenames bigger
  than that supported by the XBOX file system.

+ Added -OEIBuf (Out Edge Integrity Buffer) flag to
  xdvdfs_maker.exe which can be used with the -pad
  option to ensure the shitty bit of cheap discs near
  the outside edge is not written to.

+ Few more bug fixes which I cant remember