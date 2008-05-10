object AboutBox: TAboutBox
  Left = 313
  Top = 399
  BorderIcons = [biSystemMenu]
  BorderStyle = bsNone
  Caption = 'About TopTools'
  ClientHeight = 299
  ClientWidth = 411
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClick = CloseClick
  OnDeactivate = CloseClick
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object lbVersion: TLabel
    Left = 8
    Top = 196
    Width = 401
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'v.2.4'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    Transparent = True
    OnClick = CloseClick
  end
  object lbCopy: TLabel
    Left = 7
    Top = 216
    Width = 402
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = '© 1998-2008 Willem Jongman'
    Color = clBlack
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Transparent = True
    OnClick = CloseClick
  end
  object lbUrl: TLabel
    Left = 80
    Top = 236
    Width = 248
    Height = 16
    Cursor = crHandPoint
    Caption = 'Visit the TopTools homepage for updates'
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentColor = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    OnClick = UrlClick
  end
  object lbClose: TLabel
    Left = 192
    Top = 266
    Width = 35
    Height = 16
    Cursor = crHandPoint
    Caption = 'Close'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = CloseClick
  end
  object Logo: TImage
    Left = 89
    Top = 40
    Width = 233
    Height = 140
    AutoSize = True
    ParentShowHint = False
    ShowHint = False
    OnClick = CloseClick
  end
end
