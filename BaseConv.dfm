inherited BaseConvForm: TBaseConvForm
  Left = 459
  Top = 309
  Caption = 'Base Converter'
  ClientHeight = 54
  ClientWidth = 237
  OldCreateOrder = True
  OnContextPopup = EditContextPopup
  PixelsPerInch = 96
  TextHeight = 13
  object plTop: TPanel
    Left = 0
    Top = 0
    Width = 237
    Height = 28
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object Label1: TLabel
      Left = 2
      Top = 6
      Width = 24
      Height = 13
      Caption = 'char:'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object Label2: TLabel
      Left = 56
      Top = 6
      Width = 20
      Height = 13
      Caption = 'hex:'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object Label3: TLabel
      Left = 139
      Top = 6
      Width = 21
      Height = 13
      Caption = 'dec:'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object AscEdit: TEdit
      Left = 29
      Top = 3
      Width = 20
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 1
      ParentFont = False
      TabOrder = 0
      Text = 'W'
      OnContextPopup = EditContextPopup
      OnKeyDown = EditKeyDown
      OnKeyPress = AscEditKeyPress
    end
    object HexEdit: TEdit
      Left = 79
      Top = 3
      Width = 55
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 8
      ParentFont = False
      TabOrder = 1
      Text = '1FFFFFFF'
      OnChange = HexEditChange
      OnContextPopup = EditContextPopup
      OnKeyDown = EditKeyDown
      OnKeyPress = HexEditKeyPress
    end
    object DecEdit: TEdit
      Left = 163
      Top = 3
      Width = 70
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 10
      ParentFont = False
      TabOrder = 2
      Text = '2147483647'
      OnChange = DecEditChange
      OnContextPopup = EditContextPopup
      OnKeyDown = EditKeyDown
      OnKeyPress = DecEditKeyPress
    end
  end
  object plBinary: TPanel
    Left = 0
    Top = 28
    Width = 237
    Height = 28
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object Label4: TLabel
      Left = 9
      Top = 6
      Width = 17
      Height = 13
      Caption = 'bin:'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object BinEdit: TEdit
      Left = 29
      Top = 3
      Width = 204
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 40
      ParentFont = False
      TabOrder = 0
      Text = '1111 1111 1111 1111 1111 1111 1111 1111'
      OnChange = BinEditChange
      OnContextPopup = EditContextPopup
      OnKeyDown = BinEditKeyDown
      OnKeyPress = BinEditKeyPress
    end
  end
  object BaseconvMenu: TPopupMenu
    Left = 176
    Top = 8
    object miHide: TMenuItem
      Caption = '&Hide Base Converter'
      Default = True
      OnClick = miHideClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object miCopy: TMenuItem
      Caption = '&Copy'
      OnClick = miCopyClick
    end
    object miPaste: TMenuItem
      Caption = '&Paste'
      OnClick = miPasteClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miBinary: TMenuItem
      Caption = '&Show binary field'
      OnClick = miBinaryClick
    end
    object miOptions: TMenuItem
      Caption = 'Options...'
      OnClick = miOptionsClick
    end
    object N2: TMenuItem
      Caption = '-'
      Visible = False
    end
    object miExit: TMenuItem
      Caption = 'E&xit TopTools'
      OnClick = miExitClick
    end
  end
end
