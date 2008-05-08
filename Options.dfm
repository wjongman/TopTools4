object OptionForm: TOptionForm
  Left = 343
  Top = 200
  BorderStyle = bsDialog
  Caption = 'TopTools Options'
  ClientHeight = 361
  ClientWidth = 438
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 438
    Height = 361
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object gbRuler: TGroupBox
      Left = 8
      Top = 293
      Width = 145
      Height = 57
      Caption = 'Ruler'
      TabOrder = 1
      object lbLength: TLabel
        Left = 16
        Top = 22
        Width = 36
        Height = 13
        Caption = '&Length:'
        FocusControl = edLength
      end
      object edLength: TEdit
        Left = 61
        Top = 19
        Width = 52
        Height = 21
        TabOrder = 0
        Text = '50'
      end
      object udLength: TUpDown
        Left = 113
        Top = 19
        Width = 16
        Height = 21
        Associate = edLength
        Min = 50
        Max = 2000
        Position = 50
        TabOrder = 1
        Thousands = False
        Wrap = False
      end
    end
    object gbGeneral: TGroupBox
      Left = 8
      Top = 8
      Width = 145
      Height = 85
      Caption = 'General'
      TabOrder = 0
      object ckStart: TCheckBox
        Left = 16
        Top = 53
        Width = 113
        Height = 13
        Hint = 'Adds TopTools to the run section in the registry'
        Caption = '&Start with Windows'
        ParentShowHint = False
        ShowHint = False
        TabOrder = 1
      end
      object ckOnTop: TCheckBox
        Left = 16
        Top = 25
        Width = 98
        Height = 14
        Caption = '&Always On Top'
        TabOrder = 0
      end
    end
    object gbColorCopy: TGroupBox
      Left = 168
      Top = 8
      Width = 257
      Height = 273
      Caption = 'Color Copy'
      ParentShowHint = False
      ShowHint = False
      TabOrder = 2
      object gbHotkey: TGroupBox
        Left = 16
        Top = 95
        Width = 225
        Height = 89
        Hint =
          'Specify a key combination to perform the copy action ~regardless' +
          ' of which window has keyboard focus.'
        Caption = 'Hotkey'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        object lbHotkey: TLabel
          Left = 16
          Top = 54
          Width = 37
          Height = 13
          Caption = '&Hotkey:'
          FocusControl = HotkeyEdit
        end
        object ckEnableHK: TCheckBox
          Left = 16
          Top = 24
          Width = 97
          Height = 17
          Caption = '&Enable Hotkey'
          TabOrder = 0
          OnClick = ckEnableHKClick
          OnMouseUp = ckEnableHKMouseUp
        end
        object HotkeyEdit: TEdit
          Left = 64
          Top = 51
          Width = 145
          Height = 21
          ReadOnly = True
          TabOrder = 1
          Text = 'Ctrl + Shift + Alt + Q'
          OnKeyDown = HotkeyEditKeyDown
        end
      end
      object Memo1: TMemo
        Left = 16
        Top = 24
        Width = 225
        Height = 41
        BorderStyle = bsNone
        Color = clBtnFace
        Lines.Strings = (
          'Specify a hotkey to copy the pixel color under '
          'the mousepointer in webcolor format to the '
          'clipboard.')
        ReadOnly = True
        TabOrder = 2
      end
      object gbAttributes: TGroupBox
        Left = 16
        Top = 197
        Width = 225
        Height = 57
        Hint = 'Specify attributes to add to the webcolor string.'
        Caption = 'Format Attributes'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        object ckPrefix: TCheckBox
          Left = 16
          Top = 24
          Width = 89
          Height = 17
          Caption = 'Add # &Prefix'
          TabOrder = 0
        end
        object ckQuotes: TCheckBox
          Left = 128
          Top = 24
          Width = 81
          Height = 17
          Caption = 'Add &Quotes'
          TabOrder = 1
        end
      end
    end
    object gbIcon: TGroupBox
      Left = 8
      Top = 104
      Width = 145
      Height = 177
      Caption = 'Tray Icon'
      TabOrder = 3
      object Label1: TLabel
        Left = 16
        Top = 24
        Width = 94
        Height = 13
        Caption = 'Double-click opens:'
      end
      object ckRuler: TCheckBox
        Left = 16
        Top = 73
        Width = 113
        Height = 13
        Hint = 'Adds TopTools to the run section in the registry'
        Caption = 'Ruler'
        ParentShowHint = False
        ShowHint = False
        TabOrder = 1
      end
      object ckToolbar: TCheckBox
        Left = 16
        Top = 49
        Width = 98
        Height = 14
        Caption = '&Toolbar'
        TabOrder = 0
      end
      object ckLoupe: TCheckBox
        Left = 16
        Top = 97
        Width = 113
        Height = 13
        Hint = 'Adds TopTools to the run section in the registry'
        Caption = 'Loupe'
        ParentShowHint = False
        ShowHint = False
        TabOrder = 2
      end
      object ckInfo: TCheckBox
        Left = 16
        Top = 121
        Width = 113
        Height = 13
        Hint = 'Adds TopTools to the run section in the registry'
        Caption = 'Info'
        ParentShowHint = False
        ShowHint = False
        TabOrder = 3
      end
      object ckBaseconv: TCheckBox
        Left = 16
        Top = 145
        Width = 113
        Height = 13
        Hint = 'Adds TopTools to the run section in the registry'
        Caption = 'Base Converter'
        ParentShowHint = False
        ShowHint = False
        TabOrder = 4
      end
    end
    object bnCancel: TButton
      Left = 351
      Top = 320
      Width = 75
      Height = 25
      Cancel = True
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 4
    end
    object bnOk: TButton
      Left = 248
      Top = 320
      Width = 75
      Height = 25
      Caption = '&OK'
      Default = True
      ModalResult = 1
      TabOrder = 5
      OnClick = bnOkClick
    end
  end
end
