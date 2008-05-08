object Form1: TForm1
  Left = 1358
  Top = 267
  Width = 114
  Height = 141
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object bnAutosave: TButton
    Left = 16
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Autosave...'
    TabOrder = 0
    OnClick = bnAutosaveClick
  end
  object bnQuit: TButton
    Left = 16
    Top = 72
    Width = 75
    Height = 25
    Caption = 'Quit'
    TabOrder = 1
    OnClick = bnQuitClick
  end
  object bnTopTools: TButton
    Left = 16
    Top = 40
    Width = 75
    Height = 25
    Caption = 'TopTools...'
    TabOrder = 2
    OnClick = bnTopToolsClick
  end
end
