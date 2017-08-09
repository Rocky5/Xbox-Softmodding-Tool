unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls, XPMan, xbeheader, CheckLst,strutils, ExtCtrls;

 type
  TPartFile = record
    fname:string;
    offset:int64;
    size:int64;
  end;
  PPartFile = ^TPartFile;

  TXisoEntry = record
    name:string;
    offset:int64;
    size:int64;
    childs:TList;
  end;
  PXisoEntry = ^TXIsoEntry;

  TForm1 = class(TForm)
    Ouvrir: TButton;
    tbId: TEdit;
    Label4: TLabel;
    tbName: TEdit;
    Label5: TLabel;
    isoname: TLabel;
    CheckListBox1: TCheckListBox;
    Button1: TButton;
    Button2: TButton;
    ProgressBar1: TProgressBar;
    Label1: TLabel;
    Panel1: TPanel;
    Label2: TLabel;
    Button3: TButton;
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure OuvrirClick(Sender: TObject);
  private
    CurrentIsoList:TStringList;
    TitleId:cardinal;
    TitleName:string;
    procedure CopySlice(fname: string; slice: PPartFile);
    procedure BuildPartList(fname, name: string);
    function OpenXIso(s: string):boolean;
    procedure ExtractEntry(f: TFileStream; Entry: PXIsoEntry; Output: TStream);
    procedure ExtractFile(f: TFileStream; Offset, Size: int64; Output: TStream);
    procedure UpdateLabels(Entry: PXIsoEntry);
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation
{$R *.dfm}

const
  DefaultSliceSize : cardinal = $ffe00000;

Function OpenXIso(filename:string;var f:TFileStream;var RootSec:int64;var RootSize:int64): Boolean;
var
  h:string;
begin
  f := TFileStream.Create(filename,fmOpenRead);
  f.Seek(65536,soBeginning);
  setlength(h,20);
  f.ReadBuffer(h[1],20);
  if h<>'MICROSOFT*XBOX*MEDIA' then
    result := false
  else begin
    rootsec:=0;
    rootsize:=0;
    f.Read(rootsec,4);
    f.Read(rootsize,4);
    f.Seek(2000,soCurrent);
    f.ReadBuffer(h[1],20);
    if h<>'MICROSOFT*XBOX*MEDIA' then
    begin
      f.Free;
      result := false;
    end
    else result:=true;
  end;
 end;

 function min(a,b:int64):int64;
 begin
   if a<b then result := a else result := b;
 end;

 Procedure TForm1.ExtractFile(f:TFileStream;Offset :Int64;Size :int64;Output :TStream);
 const
  BufferSize = 1048576;
 var
  c:int64;
  n:int64;
  i:int64;
 begin
  i := Size div BufferSize;
  ProgressBar1.Visible:=true;
  ProgressBar1.Min := 0;
  ProgressBar1.Max := i+1;
  ProgressBar1.Step := 1;
  ProgressBar1.Smooth := true;
  ProgressBar1.Position := 0;

  f.Seek(Offset, soBeginning);
  c:=0;
  while (c<Size) do
  begin
    n:=min(Size-c,BufferSize);
    output.CopyFrom(f,n);
    c:=c+n;
    ProgressBar1.StepIt;
    label2.Caption := FloatToStrF((c/size)*100,ffFixed,5,2) +' %';
    Application.ProcessMessages;
  end;
  ProgressBar1.Position := ProgressBar1.Max;
  ProgressBar1.Visible:=false;
 end;

 procedure TForm1.ExtractEntry(f:TFileStream;Entry:PXIsoEntry;Output:TStream);
 begin
   ExtractFile(f,Entry.offset,Entry.size,Output);
 end;

 procedure BuildNode(f:TFileStream;Offset:Int64;Table:Int64;list:TList);
 var
  LeftOffset : int64;
  RightOffset : int64;
  DataSec,DataSize : int64;
  DataAttr : byte;
  d2:int64;
  t2:int64;
  l:byte;
  fname:string;
  curEntry : Pxisoentry;
 begin
        f.Seek(Offset, soBeginning);
        leftoffset := 0;
        f.Read(LeftOffset,2);
        rightoffset := 0;
        f.Read(RightOffset,2);

        datasec := 0;
        f.Read(datasec,4);
        datasize := 0;
        f.Read(datasize,4);
        f.Read(dataattr,1);
        f.Read(l,1);
        setlength(fname,l);
        f.Read(fname[1],l);

        d2 := datasec * 2048;
        t2 := Table * 2048;

        If (l <> 0) Then
        begin
            new(curEntry);
            list.Add(curEntry);
            curEntry.name := fname;
            curEntry.offset := d2;
            curEntry.size := DataSize;
            curEntry.childs := nil;
            If ((fname<>'')) and ((DataAttr = 16) Or (DataAttr = 17)) Then
            begin
                curEntry.childs := TList.Create();
                BuildNode(f,d2, DataSec,curEntry.childs);
            end;

            If (RightOffset <> 0) Then
              BuildNode(f, t2 + (RightOffset * 4), Table,list);

            If (LeftOffset <> 0) Then
              BuildNode(f, t2 + (LeftOffset * 4), Table,list);
        End;
 end;

function BuildTree(s:string):TList;
var
  l:TList;
  f:TFileStream;
  RootSec,RootSize:Int64;
  r:boolean;
begin
  if OpenXIso(s,f,RootSec,RootSize)
  then begin
    l:=TList.Create();
    BuildNode(f,RootSec*2048,RootSec,l);
    f.Free;
    result := l;
  end
  else result := nil;
end;

procedure UpdateTreeView(Tree:TList;Tv:TTreeView;n:TTreeNode);
var
  n2:TTreeNode;
  i:integer;
  entry : PXIsoEntry;
begin
  if n=nil then
    n:=Tv.Items.AddChild(nil,'(root)');

  for i := 0 to Tree.Count - 1 do
  begin
    entry := PXIsoEntry(Tree.Items[i]);
    n2 := tv.Items.AddChild(n,entry.name);
    n2.Data := entry;
    if entry.childs<>nil then
      UpdateTreeView(entry.childs,tv,n2);
  end;
end;

procedure GetStringList(prefix:string;sl:TStringList;n:TTreeNode); overload;
var
  i:integer;
  s:string;
begin
  for I := 0 to n.Count - 1 do
  begin
    if n<>nil then
      s:=prefix+'/'+n.Item[i].Text
    else
      s:='/';
    sl.AddObject(s,n.Item[i].Data);
    if n.Item[i].Count>0 then
      GetStringList(s,sl,n.Item[i]);
  end;
end;

function GetStringList(tv:TTreeView):TStringList; overload;
begin
  result := TStringList.Create;
  GetStringList('',result,tv.Items[0]);
end;

procedure GetNode(Tree:TList;var list:TStringList;prefix:string);
var
  i:integer;
  entry : PXIsoEntry;
begin
  for i := 0 to Tree.Count - 1 do
  begin
    entry := PXIsoEntry(Tree.Items[i]);

    if entry.childs=nil then
      list.AddObject(prefix+'/'+entry.name,TObject(entry))
    else
      GetNode(entry.childs,list,prefix+'/'+entry.name);
  end;
end;

function GetStringList(t:TList):TStringList; overload;
begin
  result := TStringList.Create;
  GetNode(t,result,'');
end;

function FindXIsoEntry(list:TStringList;entry:string):PXIsoEntry;
var
  i:integer;
begin
  result := nil;
  // search directly (case sensitive)
  i:=list.IndexOf(entry);
  // couldn't find it ? search case insensitive
  if i=-1 then
  begin
    i:=0;
    entry:=lowercase(entry);
    while (i<list.Count) and (LowerCase(list[i])<>entry) do
      inc(i);
    if i=list.count then i:=-1;
  end;

  if i>-1 then
    result := PXIsoEntry(list.Objects[i]);
end;

function GetXbeCertificate(f:TStream):T_XBE_CERTIFICATE;
var
  header : T_XBE_HEADER;
  certificate : T_XBE_CERTIFICATE;
begin
  f.Seek(0,soBeginning);
  f.Read(header,sizeof(header));
  f.seek(cardinal(header.certificate) - cardinal(header.baseAddress),soBeginning);
  f.Read(certificate,sizeof(certificate));
  result := certificate;
end;

procedure SetXbeCertificate(f:TStream;c:T_XBE_CERTIFICATE);
var
  header : T_XBE_HEADER;
begin
  f.Seek(0,soBeginning);
  f.Read(header,sizeof(header));
  f.seek(cardinal(header.certificate) - cardinal(header.baseAddress),soBeginning);
  f.Write(c,sizeof(c));
end;

function GetXBoxTitleId(f:TStream):cardinal;
var
  c : T_XBE_CERTIFICATE;
begin
  c := GetXbeCertificate(f);
  result := c.TitleId;
end;

function GetXBoxTitleName(f:TStream):WideString;
var
  c : T_XBE_CERTIFICATE;
begin
  c := GetXbeCertificate(f);
  result := c.TitleName;
end;

procedure SetXBoxTitleId(f:TStream;t:cardinal);
var
  c : T_XBE_CERTIFICATE;
begin
  c := GetXbeCertificate(f);
  c.TitleId := t;
  SetXbeCertificate(f,c);
end;

procedure SetXBoxTitleName(f:TStream;t:Widestring);
var
  c : T_XBE_CERTIFICATE;
  I: Integer;
begin
  c := GetXbeCertificate(f);
  for i := 0 to 39 do
    c.TitleName[i] := #0;

  for I := 0 to min(length(t),40) - 1 do
    c.TitleName[i] := t[i+1];

  SetXbeCertificate(f,c);
end;

function TForm1.OpenXIso(s:string):boolean;
var
  Tree:TList;
  entry:PXisoEntry;
  f:TFileStream;
  f2:TStream;
  defaultStream : TStream;
begin
  Tree := BuildTree(s);
  result := tree<>nil;
  if result then
  begin
    CurrentIsoList := GetStringList(Tree);
    entry := FindXIsoEntry(CurrentIsoList,'/default.xbe');

    f:=TFileStream.Create(s,fmOpenRead);
    f2:=TMemoryStream.Create();
    ExtractEntry(f,entry,f2);
    TitleId := GetXboxTitleId(f2);
    TitleName := GetXboxTitleName(f2);
    f2.Free;
    f.free;
  end;
end;

Procedure TForm1.BuildPartList(fname,name:string);
var
  f:TFileStream;
  size:int64;
  part:PPartFile;
  offset:int64;
  i:integer;
begin
  f:=TFileStream.Create(fname,fmOpenRead);
  size := f.Size;
  f.free;
  i:=0;
  offset:=0;
  while offset<size do
  begin
    new(part);
    inc(i);
    part.fname := name+'.'+inttostr(i)+'.iso';
    part.offset := offset;
    part.size := min(DefaultSliceSize,size-offset);
    offset := offset+part.size;
    CheckListBox1.AddItem(part.fname,TObject(part));
    CheckListBox1.Checked[CheckListBox1.Count-1] := true;
  end;

end;

procedure TForm1.OuvrirClick(Sender: TObject);
begin
  with TOpenDialog.Create(nil) do
  begin
    Filter := 'Xbox Iso|*.iso|All files|*.*';
    DefaultExt := 'iso';
    CheckListBox1.Clear;
    if execute then
    begin
      if not OpenXIso(FileName) then
      begin
        TitleId := 0;
        TitleName:='';
        Button1.Enabled := false;
        Button2.Enabled := false;
        ShowMessage('Not an XBox Iso file !!!');
      end
      else begin
        Button1.Enabled := true;
        Button2.Enabled := true;
        BuildPartList(Filename,TitleName);
        isoname.Caption := Filename;
      end;
      tbId.Text := inttostr(TitleId);
      tbName.Text := TitleName;
    end;
  end;
end;

procedure TForm1.UpdateLabels(Entry:PXIsoEntry);
begin
{  if entry <> nil then
  begin
    label1.Caption := entry.name;
    if entry.childs=nil then
    begin
      label2.Caption := IntToStr(entry.offset);
      label3.Caption := IntToStr(entry.size);
    end
    else begin
      label2.Caption := '';
      label3.Caption := '';
    end;
  end;}
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  entry:PXisoEntry;
  f,f2:TFileStream;
begin
  f:=TFileStream.Create(extractfilepath(paramstr(0))+'default.data',fmOpenRead);
  f2:=TFileStream.Create(ExtractFilePath(isoname.Caption)+'default.xbe',fmCreate);
  f2.CopyFrom(f,f.Size);
  SetXBoxTitleId(f2,StrToIntDef(tbId.Text,0));
  SetXBoxTitleName(f2,tbName.Text);
  f.Free;
  f2.Free;
  label1.Caption := 'default.xbe créé pour '+tbName.Text;
  label1.Visible := true;
  button1.Enabled := false;
end;

procedure TForm1.CopySlice(fname:string;slice : PPartFile);
var
  f:TFileStream;
  f2:TFileStream;
begin
  f:=TFileStream.Create(fname,fmOpenRead);
  f2:=TFileStream.Create(extractfilepath(fname)+slice.fname,fmCreate);
  ExtractFile(f,slice.offset,slice.size,f2);
  f.Free;
  f2.Free;
end;

procedure TForm1.Button2Click(Sender: TObject);
var
  i:integer;
begin
  label1.Visible := true;
  label2.Visible := true;
  for I := 0 to checklistbox1.count - 1 do
    if CheckListBox1.Checked[i] then
    begin
      label1.Caption := 'Process '+PPartFile(CheckListBox1.Items.Objects[i]).fname+' ...';
      CopySlice(isoname.Caption, PPartFile(CheckListBox1.Items.Objects[i]));
    end;
  label1.Visible := true;
  label1.Caption := 'Complete !';
  label2.Visible := false;
end;


//  Exemple de code pour extraire un fichier :
// ExtractFileFromXIso('myIso.iso','/data/videos/intro.bik');
// ExtractFileFromXIso('myIso.iso','/default.xbe');

procedure ExtractFileFromXIso(isoFileName:string;FileToExtract:string);
var
  Tree:TList;
  entry:PXisoEntry;
  f:TFileStream;
  f2:TStream;
  defaultStream : TStream;
  FileList:TStringList;
begin
  Tree := BuildTree(isoFileName);
  if tree<>nil then
  begin
    FileList := GetStringList(Tree);
    entry := FindXIsoEntry(FileList,FileToExtract);
    if entry<>nil then
    begin
      f:=TFileStream.Create(isoFileName,fmOpenRead);
      f2:=TFileStream.Create(entry.name,fmCreate);
      f.Seek(entry.offset,soBeginning);
      f2.CopyFrom(f,entry.size);
      f2.Free;
      f.free;
    end;
  end;
end;


end.
