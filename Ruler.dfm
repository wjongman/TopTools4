object RulerForm: TRulerForm
  Left = 315
  Top = 189
  Cursor = crArrow
  Hint = 'Right click for option menu'
  Anchors = []
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'RulerForm'
  ClientHeight = 50
  ClientWidth = 644
  Color = clNone
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = RulerMenu
  Position = poDefaultPosOnly
  Scaled = False
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object RulerMenu: TPopupMenu
    Alignment = paCenter
    AutoPopup = False
    OnPopup = RulerMenuPopup
    Left = 8
    Top = 8
    object miHide: TMenuItem
      Caption = 'Hide Ruler'
      Default = True
      OnClick = miHideClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miOrientation: TMenuItem
      Caption = 'Vertical'
      OnClick = miOrientationClick
    end
    object miTransparent: TMenuItem
      Caption = 'Transparent'
      OnClick = miTransparentClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object miZero: TMenuItem
      Caption = 'Slide to zero'
      OnClick = miSlidetoZeroClick
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
