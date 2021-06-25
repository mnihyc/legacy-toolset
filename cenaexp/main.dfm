object FMain: TFMain
  Left = 191
  Top = 104
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'CenaExp'
  ClientHeight = 460
  ClientWidth = 471
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object LName: TLabel
    Left = 352
    Top = 0
    Width = 48
    Height = 16
    Caption = 'Name: '
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LWorkDir: TLabel
    Left = 352
    Top = 24
    Width = 121
    Height = 57
    AutoSize = False
    Caption = 'Dir:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object LStatus: TLabel
    Left = 352
    Top = 88
    Width = 64
    Height = 16
    Caption = 'Status: '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LSystem: TLabel
    Left = 352
    Top = 120
    Width = 121
    Height = 73
    AutoSize = False
    Caption = 'System: '
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object LUCName: TLabel
    Left = 352
    Top = 200
    Width = 108
    Height = 12
    AutoSize = False
    Caption = 'UsesComputerName: '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object List1: TListBox
    Left = 0
    Top = 0
    Width = 353
    Height = 465
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ItemHeight = 16
    ParentFont = False
    TabOrder = 0
    OnClick = List1Click
  end
  object BSTMSG: TButton
    Left = 352
    Top = 216
    Width = 121
    Height = 25
    Caption = 'SendTestMessage'
    Enabled = False
    TabOrder = 1
    OnClick = BSTMSGClick
  end
  object BEditConfig: TButton
    Left = 352
    Top = 240
    Width = 121
    Height = 25
    Caption = 'EditConfig'
    Enabled = False
    TabOrder = 2
    OnClick = BEditConfigClick
  end
  object Button1: TButton
    Left = 352
    Top = 264
    Width = 121
    Height = 25
    Caption = 'Others'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = Button1Click
  end
  object BUE: TButton
    Left = 352
    Top = 320
    Width = 121
    Height = 25
    Caption = 'UpdateExploit'
    Enabled = False
    TabOrder = 4
    OnClick = BUEClick
  end
  object btn_co: TButton
    Left = 424
    Top = 432
    Width = 41
    Height = 25
    Caption = 'Close'
    TabOrder = 5
    OnClick = btn_coClick
  end
  object UDPC: TIdUDPClient
    BroadcastEnabled = True
    Port = 0
    Top = 312
  end
  object UDPS: TIdUDPServer
    BroadcastEnabled = True
    Bindings = <>
    DefaultPort = 0
    OnUDPRead = UDPSUDPRead
    Left = 64
    Top = 312
  end
  object TCPC: TIdTCPClient
    MaxLineAction = maException
    ReadTimeout = 0
    Port = 0
    Left = 96
    Top = 312
  end
end
