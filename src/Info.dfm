inherited InfoForm: TInfoForm
  Left = 275
  Top = 202
  Caption = 'Info'
  ClientHeight = 130
  ClientWidth = 130
  OldCreateOrder = True
  PopupMenu = InfoPopupMenu
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  PixelsPerInch = 96
  TextHeight = 13
  object RGBPanel: TPanel
    Left = 0
    Top = 23
    Width = 130
    Height = 59
    Align = alTop
    BevelInner = bvLowered
    BevelOuter = bvNone
    TabOrder = 0
    OnMouseDown = FormMouseDown
    OnMouseMove = FormMouseMove
    OnMouseUp = FormMouseUp
    object Red: TLabel
      Left = 24
      Top = 8
      Width = 24
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = '255'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object RLabel: TLabel
      Left = 8
      Top = 8
      Width = 11
      Height = 13
      Caption = 'R:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object GLabel: TLabel
      Left = 8
      Top = 24
      Width = 11
      Height = 13
      Caption = 'G:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object Green: TLabel
      Left = 24
      Top = 24
      Width = 24
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = '255'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object BLabel: TLabel
      Left = 8
      Top = 41
      Width = 10
      Height = 13
      Caption = 'B:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object Blue: TLabel
      Left = 24
      Top = 41
      Width = 24
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = '255'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object Hue: TLabel
      Left = 85
      Top = 8
      Width = 29
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = '100°'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object Sat: TLabel
      Left = 85
      Top = 24
      Width = 29
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = '100%'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object Val: TLabel
      Left = 85
      Top = 41
      Width = 29
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = '100%'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object VLabel: TLabel
      Left = 69
      Top = 41
      Width = 10
      Height = 13
      Caption = 'V:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object SLabel: TLabel
      Left = 69
      Top = 24
      Width = 10
      Height = 13
      Caption = 'S:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object HLabel: TLabel
      Left = 69
      Top = 8
      Width = 11
      Height = 13
      Caption = 'H:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
  end
  object XYPanel: TPanel
    Left = 0
    Top = 0
    Width = 130
    Height = 23
    Align = alTop
    BevelInner = bvLowered
    BevelOuter = bvNone
    TabOrder = 1
    OnMouseDown = FormMouseDown
    OnMouseMove = FormMouseMove
    OnMouseUp = FormMouseUp
    object YValue: TLabel
      Left = 85
      Top = 5
      Width = 24
      Height = 13
      Alignment = taCenter
      Caption = '1024'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object YLabel: TLabel
      Left = 69
      Top = 5
      Width = 10
      Height = 13
      Caption = 'Y:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object XValue: TLabel
      Left = 24
      Top = 5
      Width = 24
      Height = 13
      Alignment = taCenter
      Caption = '1024'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object XLabel: TLabel
      Left = 8
      Top = 5
      Width = 10
      Height = 13
      Caption = 'X:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
  end
  object WebPanel: TPanel
    Left = 0
    Top = 82
    Width = 130
    Height = 23
    Align = alTop
    BevelInner = bvLowered
    BevelOuter = bvNone
    TabOrder = 2
    OnMouseDown = FormMouseDown
    OnMouseMove = FormMouseMove
    OnMouseUp = FormMouseUp
    object RGB: TLabel
      Left = 61
      Top = 5
      Width = 60
      Height = 13
      AutoSize = False
      Caption = '# C0 C0 C0'
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
    object ColorPanel: TPanel
      Left = 7
      Top = 4
      Width = 39
      Height = 15
      BevelOuter = bvLowered
      TabOrder = 0
      OnMouseDown = FormMouseDown
      OnMouseMove = FormMouseMove
      OnMouseUp = FormMouseUp
    end
  end
  object InfoPopupMenu: TPopupMenu
    Left = 8
    Top = 88
    object miHide: TMenuItem
      Caption = 'Hide Info'
      Default = True
      OnClick = miHideClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miOptions: TMenuItem
      Caption = 'Options...'
      OnClick = miOptionsClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miExit: TMenuItem
      Caption = 'Exit TopTools'
      OnClick = miExitClick
    end
  end
end
