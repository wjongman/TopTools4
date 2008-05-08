object ImageViewer: TImageViewer
  Left = 1024
  Top = 726
  BorderStyle = bsToolWindow
  Caption = 'TopTools Snapshot'
  ClientHeight = 215
  ClientWidth = 329
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object Image: TImage
    Left = 0
    Top = 0
    Width = 329
    Height = 215
    Align = alClient
    OnMouseDown = FormMouseDown
  end
end
