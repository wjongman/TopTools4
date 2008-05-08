object HotkeyDlg: THotkeyDlg
  Left = 533
  Top = 388
  BorderStyle = bsDialog
  Caption = 'Select Hotkey'
  ClientHeight = 187
  ClientWidth = 272
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 257
    Height = 137
    Shape = bsFrame
  end
  object lbAction: TLabel
    Left = 24
    Top = 75
    Width = 33
    Height = 13
    Caption = '&Action:'
  end
  object lbHotkey: TLabel
    Left = 24
    Top = 111
    Width = 37
    Height = 13
    Caption = '&Hotkey:'
    FocusControl = edHotkey
  end
  object OKBtn: TButton
    Left = 103
    Top = 156
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 191
    Top = 156
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object cbHotkeyAction: TComboBox
    Left = 72
    Top = 71
    Width = 177
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
    Items.Strings = (
      'Copy Current Pixel Color'
      'Double-click Trayicon'
      'Zoom Loupe In'
      'Zoom Loupe Out'
      'Toggle Ruler Orientation'
      'Grab Screen'
      'Display Options Dialog')
  end
  object memoExplainHotkeys: TMemo
    Left = 24
    Top = 27
    Width = 233
    Height = 33
    BorderStyle = bsNone
    Color = clBtnFace
    Lines.Strings = (
      'Define key combinations for actions to occur no '
      'matter which application has keyboard focus.')
    ReadOnly = True
    TabOrder = 3
  end
  object edHotkey: TEdit
    Left = 72
    Top = 108
    Width = 177
    Height = 21
    ReadOnly = True
    TabOrder = 4
  end
end
