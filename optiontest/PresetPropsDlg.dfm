object PresetPropsDlg: TPresetPropsDlg
  Left = 1144
  Top = 301
  BorderStyle = bsDialog
  Caption = 'Preset Properties'
  ClientHeight = 138
  ClientWidth = 265
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lbX: TLabel
    Left = 8
    Top = 48
    Width = 10
    Height = 13
    Caption = '&X:'
    FocusControl = edX
  end
  object lbY: TLabel
    Left = 72
    Top = 48
    Width = 10
    Height = 13
    Caption = '&Y:'
    FocusControl = edY
  end
  object lbW: TLabel
    Left = 136
    Top = 48
    Width = 14
    Height = 13
    Caption = '&W:'
    FocusControl = edW
  end
  object lbH: TLabel
    Left = 200
    Top = 48
    Width = 11
    Height = 13
    Caption = '&H:'
    FocusControl = edH
  end
  object lbName: TLabel
    Left = 8
    Top = 4
    Width = 31
    Height = 13
    Caption = '&Name:'
    FocusControl = edTitle
  end
  object bnOk: TButton
    Left = 88
    Top = 103
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 5
  end
  object bnCancel: TButton
    Left = 182
    Top = 103
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 6
  end
  object edTitle: TEdit
    Left = 8
    Top = 20
    Width = 249
    Height = 21
    TabOrder = 0
    OnChange = edTitleChange
  end
  object edX: TEdit
    Left = 8
    Top = 64
    Width = 41
    Height = 21
    TabOrder = 1
    Text = '0'
  end
  object udX: TUpDown
    Left = 48
    Top = 64
    Width = 15
    Height = 21
    Associate = edX
    Min = 0
    Max = 32767
    Position = 0
    TabOrder = 7
    Wrap = False
  end
  object edY: TEdit
    Left = 72
    Top = 64
    Width = 41
    Height = 21
    TabOrder = 2
    Text = '0'
  end
  object udY: TUpDown
    Left = 112
    Top = 64
    Width = 15
    Height = 21
    Associate = edY
    Min = 0
    Max = 32767
    Position = 0
    TabOrder = 8
    Wrap = False
  end
  object edW: TEdit
    Left = 136
    Top = 64
    Width = 41
    Height = 21
    TabOrder = 3
    Text = '0'
  end
  object udW: TUpDown
    Left = 176
    Top = 64
    Width = 15
    Height = 21
    Associate = edW
    Min = 0
    Max = 32767
    Position = 0
    TabOrder = 9
    Wrap = False
  end
  object edH: TEdit
    Left = 200
    Top = 64
    Width = 41
    Height = 21
    TabOrder = 4
    Text = '0'
  end
  object udH: TUpDown
    Left = 240
    Top = 64
    Width = 15
    Height = 21
    Associate = edH
    Min = 0
    Max = 32767
    Position = 0
    TabOrder = 10
    Wrap = False
  end
end
