object FEditConfig: TFEditConfig
  Left = 226
  Top = 416
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'EditConfig'
  ClientHeight = 244
  ClientWidth = 283
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object LNshow: TLabel
    Left = 0
    Top = 8
    Width = 48
    Height = 16
    AutoSize = False
    Caption = 'Name: '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LWDshow: TLabel
    Left = 0
    Top = 48
    Width = 65
    Height = 17
    AutoSize = False
    Caption = 'WorkDir:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LASUshow: TLabel
    Left = 0
    Top = 88
    Width = 97
    Height = 17
    AutoSize = False
    Caption = 'AutoStartUp:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LUCN: TLabel
    Left = 120
    Top = 88
    Width = 136
    Height = 16
    AutoSize = False
    Caption = 'UsesComputerName:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LStatus: TLabel
    Left = 0
    Top = 120
    Width = 273
    Height = 25
    AutoSize = False
    Caption = 'Status:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LP: TLabel
    Left = 0
    Top = 152
    Width = 97
    Height = 25
    AutoSize = False
    Caption = 'Permissions: '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LV: TLabel
    Left = 0
    Top = 184
    Width = 273
    Height = 25
    AutoSize = False
    Caption = 'Version: '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object EName: TEdit
    Left = 48
    Top = 8
    Width = 225
    Height = 24
    AutoSize = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    MaxLength = 64
    ParentFont = False
    TabOrder = 0
  end
  object EWorkDir: TEdit
    Left = 64
    Top = 48
    Width = 209
    Height = 21
    AutoSize = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    MaxLength = 256
    ParentFont = False
    TabOrder = 1
  end
  object CBASU: TCheckBox
    Left = 96
    Top = 88
    Width = 17
    Height = 25
    TabOrder = 2
  end
  object CBUCN: TCheckBox
    Left = 256
    Top = 88
    Width = 17
    Height = 17
    TabOrder = 3
  end
  object BS: TButton
    Left = 56
    Top = 216
    Width = 153
    Height = 25
    Caption = 'huy'
    TabOrder = 4
    OnClick = BSClick
  end
  object EP: TEdit
    Left = 96
    Top = 152
    Width = 177
    Height = 21
    AutoSize = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    MaxLength = 15
    ParentFont = False
    TabOrder = 5
  end
end
