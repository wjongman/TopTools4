object ScreenForm: TScreenForm
  Left = 1318
  Top = 431
  Width = 309
  Height = 172
  Cursor = crSizeAll
  Hint = 
    'Click right mouse-button to capture selected area\r\nleft to set' +
    ' position'
  Caption = 'ScreenForm'
  Color = clBtnFace
  Constraints.MinHeight = 16
  Constraints.MinWidth = 16
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnKeyDown = FormKeyDown
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object InfoLabel: TLabel
    Left = 19
    Top = 1
    Width = 63
    Height = 14
    Caption = 'InfoLabel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
end
