object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 303
  ClientWidth = 496
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    496
    303)
  PixelsPerInch = 96
  TextHeight = 13
  object Label4: TLabel
    Left = 8
    Top = 38
    Width = 40
    Height = 13
    Caption = 'Title Id :'
  end
  object Label5: TLabel
    Left = 152
    Top = 38
    Width = 57
    Height = 13
    Caption = 'Title Name :'
  end
  object isoname: TLabel
    Left = 8
    Top = 8
    Width = 480
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Ouvrir: TButton
    Left = 7
    Top = 62
    Width = 130
    Height = 25
    Caption = 'Ouvrir Iso'
    TabOrder = 0
    OnClick = OuvrirClick
  end
  object tbId: TEdit
    Left = 54
    Top = 35
    Width = 83
    Height = 21
    TabOrder = 1
  end
  object tbName: TEdit
    Left = 215
    Top = 35
    Width = 274
    Height = 21
    TabOrder = 2
  end
  object CheckListBox1: TCheckListBox
    Left = 8
    Top = 93
    Width = 481
    Height = 132
    ItemHeight = 13
    TabOrder = 3
  end
  object Button1: TButton
    Left = 343
    Top = 62
    Width = 145
    Height = 25
    Caption = 'G'#233'n'#233'rer loader nkpatcher'
    Enabled = False
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 231
    Width = 201
    Height = 25
    Caption = 'Sauvegarde les fichiers'
    Enabled = False
    TabOrder = 5
    OnClick = Button2Click
  end
  object ProgressBar1: TProgressBar
    Left = 0
    Top = 286
    Width = 496
    Height = 17
    Align = alBottom
    TabOrder = 6
    Visible = False
    ExplicitLeft = -7
    ExplicitTop = 262
  end
  object Panel1: TPanel
    Left = 0
    Top = 264
    Width = 496
    Height = 22
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 7
    ExplicitTop = 258
    object Label1: TLabel
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 407
      Height = 16
      Align = alClient
      AutoSize = False
      Caption = 'test'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      Visible = False
      ExplicitLeft = 0
      ExplicitTop = -5
      ExplicitWidth = 465
      ExplicitHeight = 19
    end
    object Label2: TLabel
      AlignWithMargins = True
      Left = 416
      Top = 3
      Width = 77
      Height = 16
      Align = alRight
      Alignment = taRightJustify
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      Visible = False
    end
  end
  object Button3: TButton
    Left = 232
    Top = 233
    Width = 75
    Height = 25
    Caption = 'Button3'
    TabOrder = 8
  end
end
