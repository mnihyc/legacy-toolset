object FUE: TFUE
  Left = 202
  Top = 380
  Width = 398
  Height = 331
  BorderIcons = [biSystemMenu]
  Caption = 'UpdateExploit'
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
  object LLog: TLabel
    Left = 8
    Top = 0
    Width = 377
    Height = 257
    AutoSize = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object BD: TButton
    Left = 24
    Top = 264
    Width = 121
    Height = 33
    Caption = 'Do'
    TabOrder = 0
    OnClick = BDClick
  end
  object TCPS: TIdTCPServer
    Bindings = <>
    CommandHandlers = <>
    DefaultPort = 0
    Greeting.NumericCode = 0
    MaxConnectionReply.NumericCode = 0
    OnConnect = TCPSConnect
    ReplyExceptionCode = 0
    ReplyTexts = <>
    ReplyUnknownCommand.NumericCode = 0
    Top = 240
  end
end
