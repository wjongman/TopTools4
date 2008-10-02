inherited LoupeForm: TLoupeForm
  Left = 1353
  Top = 319
  Width = 241
  Height = 249
  BorderStyle = bsSizeToolWin
  Caption = 'Loupe'
  Constraints.MinHeight = 160
  Constraints.MinWidth = 180
  OldCreateOrder = True
  PopupMenu = LoupeMenu
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnMouseWheelDown = FormMouseWheelDown
  OnMouseWheelUp = FormMouseWheelUp
  PixelsPerInch = 96
  TextHeight = 13
  object LoupeToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 233
    Height = 26
    Caption = 'LoupeToolBar'
    EdgeBorders = [ebTop, ebBottom]
    Flat = True
    TabOrder = 0
    object bnZoomOut: TToolButton
      Left = 0
      Top = 0
      Hint = 'Zoom Out'
      Caption = 'Zoom Out'
      ImageIndex = 0
      ParentShowHint = False
      ShowHint = True
      OnClick = miZoomOutClick
    end
    object bnZoomIn: TToolButton
      Left = 23
      Top = 0
      Hint = 'Zoom In'
      ImageIndex = 1
      ParentShowHint = False
      ShowHint = True
      OnClick = miZoomInClick
    end
    object Separator1: TToolButton
      Left = 46
      Top = 0
      Width = 8
      ImageIndex = 4
      Style = tbsSeparator
    end
    object bnCrosshair: TToolButton
      Left = 54
      Top = 0
      Hint = 'Crosshair'
      Caption = 'bnCrosshair'
      ImageIndex = 2
      ParentShowHint = False
      ShowHint = True
      Style = tbsCheck
      OnClick = miCrosshairClick
    end
    object bnCenterbox: TToolButton
      Left = 77
      Top = 0
      Hint = 'Centerbox'
      Caption = 'bnCenterbox'
      ImageIndex = 3
      ParentShowHint = False
      ShowHint = True
      Style = tbsCheck
      OnClick = miCenterboxClick
    end
    object bnGrid: TToolButton
      Left = 100
      Top = 0
      Hint = 'Grid'
      Caption = 'bnGrid'
      ImageIndex = 4
      ParentShowHint = False
      ShowHint = True
      Style = tbsCheck
      OnClick = miGridClick
    end
    object Separator2: TToolButton
      Left = 123
      Top = 0
      Width = 8
      Caption = 'Separator2'
      ImageIndex = 8
      Style = tbsSeparator
    end
    object bnLockPos: TToolButton
      Left = 131
      Top = 0
      Hint = 'Lock Position'
      ImageIndex = 5
      ParentShowHint = False
      ShowHint = True
      Style = tbsCheck
      OnClick = miLockPosClick
    end
  end
  object LoupeMenu: TPopupMenu
    OnPopup = LoupeMenuPopup
    Left = 24
    Top = 128
    object miHide: TMenuItem
      Caption = 'Hide &Loupe'
      Default = True
      OnClick = miHideClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miZoomIn: TMenuItem
      Caption = 'Zoom &In'
      ImageIndex = 1
      ShortCut = 16571
      OnClick = miZoomInClick
    end
    object miZoomOut: TMenuItem
      Caption = 'Zoom &Out'
      ImageIndex = 0
      ShortCut = 16573
      OnClick = miZoomOutClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miCrosshair: TMenuItem
      Caption = 'Cross &Hair'
      ImageIndex = 2
      ShortCut = 16456
      OnClick = miCrosshairClick
    end
    object miCenterbox: TMenuItem
      Caption = 'Center &Box'
      ImageIndex = 3
      ShortCut = 16450
      OnClick = miCenterboxClick
    end
    object miGrid: TMenuItem
      Caption = '&Grid'
      ImageIndex = 4
      ShortCut = 16455
      OnClick = miGridClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object miLockPos: TMenuItem
      Caption = '&Lock Position'
      ImageIndex = 5
      ShortCut = 32
      OnClick = miLockPosClick
    end
    object miFreezeView: TMenuItem
      Caption = 'Freeze View'
      Enabled = False
      ShortCut = 16416
      Visible = False
      OnClick = miFreezeViewClick
    end
    object N4: TMenuItem
      Caption = '-'
      Visible = False
    end
    object miMagnifySelf: TMenuItem
      Caption = '&Magnify Self'
      Visible = False
      OnClick = miMagnifySelfClick
    end
    object N6: TMenuItem
      Caption = '-'
    end
    object miSaveView: TMenuItem
      Caption = '&Save View'
      object miCopyToClipboard: TMenuItem
        Caption = '&Copy To Clipboard'
        Hint = 'Copy'
        ShortCut = 16451
        OnClick = miSaveViewClick
      end
      object miSaveToFile: TMenuItem
        Caption = '&Save To File...'
        Hint = 'Save'
        ShortCut = 16467
        OnClick = miSaveViewClick
      end
      object miPrint: TMenuItem
        Caption = '&Print...'
        Hint = 'Print'
        ShortCut = 16464
        OnClick = miSaveViewClick
      end
    end
    object N5: TMenuItem
      Caption = '-'
    end
    object miExit: TMenuItem
      Caption = 'E&xit TopTools'
      OnClick = miExitClick
    end
  end
end
