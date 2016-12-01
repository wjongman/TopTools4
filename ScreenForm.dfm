object ScreenForm: TScreenForm
  Left = 1319
  Top = 432
  Width = 309
  Height = 172
  Cursor = crSizeAll
  Hint = 
    'Click right mouse-button for menu.\r\nDrag and resize with left ' +
    'button.'
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
  Position = poDefault
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
end
