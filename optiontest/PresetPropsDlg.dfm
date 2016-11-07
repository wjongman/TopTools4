object PresetPropsDlg: TPresetPropsDlg
  Left = 297
  Top = 418
  BorderStyle = bsDialog
  Caption = 'Preset Properties'
  ClientHeight = 203
  ClientWidth = 232
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object bnOk: TButton
    Left = 8
    Top = 171
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object bnCancel: TButton
    Left = 150
    Top = 171
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 217
    Height = 57
    Caption = '&Description'
    TabOrder = 0
    object edTitle: TEdit
      Left = 16
      Top = 24
      Width = 185
      Height = 21
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 120
    Top = 72
    Width = 105
    Height = 89
    Caption = 'Dimension'
    TabOrder = 2
    object Label4: TLabel
      Left = 14
      Top = 27
      Width = 14
      Height = 13
      Caption = '&W:'
      FocusControl = edW
    end
    object Label5: TLabel
      Left = 16
      Top = 59
      Width = 11
      Height = 13
      Caption = '&H:'
      FocusControl = edH
    end
    object edW: TEdit
      Left = 32
      Top = 24
      Width = 41
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object edH: TEdit
      Left = 32
      Top = 56
      Width = 41
      Height = 21
      TabOrder = 1
      Text = '0'
    end
    object udW: TUpDown
      Left = 73
      Top = 24
      Width = 15
      Height = 21
      Associate = edW
      Min = 0
      Max = 32767
      Position = 0
      TabOrder = 2
      Wrap = False
    end
    object udH: TUpDown
      Left = 73
      Top = 56
      Width = 15
      Height = 21
      Associate = edH
      Min = 0
      Max = 32767
      Position = 0
      TabOrder = 3
      Wrap = False
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 72
    Width = 105
    Height = 89
    Caption = 'Position'
    TabOrder = 1
    object Label2: TLabel
      Left = 16
      Top = 27
      Width = 10
      Height = 13
      Caption = '&X:'
      FocusControl = edX
    end
    object Label3: TLabel
      Left = 16
      Top = 59
      Width = 10
      Height = 13
      Caption = '&Y:'
      FocusControl = edY
    end
    object edX: TEdit
      Left = 32
      Top = 24
      Width = 41
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object udX: TUpDown
      Left = 73
      Top = 24
      Width = 15
      Height = 21
      Associate = edX
      Min = 0
      Max = 32767
      Position = 0
      TabOrder = 1
      Wrap = False
    end
    object edY: TEdit
      Left = 32
      Top = 56
      Width = 41
      Height = 21
      TabOrder = 2
      Text = '0'
    end
    object udY: TUpDown
      Left = 73
      Top = 56
      Width = 15
      Height = 21
      Associate = edY
      Min = 0
      Max = 32767
      Position = 0
      TabOrder = 3
      Wrap = False
    end
  end
end
