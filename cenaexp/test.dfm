object FTest: TFTest
  Left = 202
  Top = 512
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'TestUnits'
  ClientHeight = 54
  ClientWidth = 317
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
  object LSHints: TLabel
    Left = 136
    Top = 0
    Width = 177
    Height = 25
    AutoSize = False
    Caption = 'Hints: '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object LBSshow: TLabel
    Left = 0
    Top = 24
    Width = 72
    Height = 16
    AutoSize = False
    Caption = 'Boardcast'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object BLSM: TButton
    Left = 0
    Top = 0
    Width = 129
    Height = 25
    Caption = 'LoopSendTestMessage'
    TabOrder = 0
    OnClick = BLSMClick
  end
  object CBBC: TCheckBox
    Left = 72
    Top = 24
    Width = 17
    Height = 17
    TabOrder = 1
  end
  object TLSend: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TLSendTimer
    Left = 288
  end
end
