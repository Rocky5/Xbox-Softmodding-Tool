program xisosplit;

// DISCLAIMER
// Cet outil contient du code provenant ou inspiré de :
// XisoE
// nkpatcher DriveImageUtils 1.0
// xbox-linux project's imagebld

uses
  Forms,
  Unit1 in 'Unit1.pas' {Form1},
  xbeheader in 'xbeheader.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
