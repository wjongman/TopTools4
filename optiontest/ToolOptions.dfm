object TopToolsOptions: TTopToolsOptions
  Left = 267
  Top = 122
  BorderStyle = bsDialog
  Caption = 'TopTools Options'
  ClientHeight = 600
  ClientWidth = 981
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object lvSelectOption: TListView
    Left = 10
    Top = 14
    Width = 105
    Height = 172
    Columns = <>
    HideSelection = False
    Items.Data = {
      D30000000700000000000000FFFFFFFFFFFFFFFF00000000000000000747656E
      6572616C00000000FFFFFFFFFFFFFFFF00000000000000000552756C65720000
      0000FFFFFFFFFFFFFFFF0000000000000000054C6F75706500000000FFFFFFFF
      FFFFFFFF000000000000000004496E666F00000000FFFFFFFFFFFFFFFF000000
      00000000000E4261736520436F6E766572746F7200000000FFFFFFFFFFFFFFFF
      00000000000000000E53637265656E204772616262657200000000FFFFFFFFFF
      FFFFFF000000000000000007486F746B657973}
    ReadOnly = True
    RowSelect = True
    ShowColumnHeaders = False
    TabOrder = 0
    ViewStyle = vsList
    OnChange = lvSelectOptionChange
  end
  object bnOk: TButton
    Left = 224
    Top = 200
    Width = 75
    Height = 25
    Caption = '&OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object bnCancel: TButton
    Left = 317
    Top = 200
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object plGeneral: TPanel
    Left = 406
    Top = 2
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 3
    object gbGeneral: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 177
      Caption = 'General'
      TabOrder = 0
      object ckStart: TCheckBox
        Left = 16
        Top = 55
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
        Top = 27
        Width = 98
        Height = 14
        Caption = '&Always on top'
        TabOrder = 0
      end
      object ckSaveToolstate: TCheckBox
        Left = 16
        Top = 83
        Width = 177
        Height = 17
        Caption = 'Preserve tool states on exit'
        TabOrder = 2
      end
      object ckOpenToolbar: TCheckBox
        Left = 16
        Top = 141
        Width = 225
        Height = 17
        Caption = 'Double-clicking tray icon opens toolbar'
        TabOrder = 3
      end
      object ckHotkeyDoubleclick: TCheckBox
        Left = 16
        Top = 112
        Width = 241
        Height = 17
        Caption = 'Hotkey emulates trayicon double-click'
        TabOrder = 4
      end
    end
  end
  object plRuler: TPanel
    Left = 694
    Top = 2
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 4
    object gbRuler: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 177
      Caption = 'Ruler'
      TabOrder = 0
      object lbLength: TLabel
        Left = 16
        Top = 86
        Width = 36
        Height = 13
        Caption = '&Length:'
        FocusControl = edLength
      end
      object ckOpenRuler: TCheckBox
        Left = 16
        Top = 141
        Width = 241
        Height = 17
        Caption = 'Double-clicking tray icon opens this tool'
        TabOrder = 0
      end
      object edLength: TEdit
        Left = 61
        Top = 83
        Width = 52
        Height = 21
        TabOrder = 1
        Text = '50'
      end
      object udLength: TUpDown
        Left = 113
        Top = 83
        Width = 15
        Height = 21
        Associate = edLength
        Min = 50
        Max = 2000
        Position = 50
        TabOrder = 2
        Thousands = False
        Wrap = False
      end
      object bnHotkeyRulerToggle: TButton
        Tag = 4
        Left = 168
        Top = 22
        Width = 75
        Height = 25
        Caption = 'Hotkey...'
        TabOrder = 3
        OnClick = bnHotkeyClick
      end
      object ckHotkeyRuler: TCheckBox
        Left = 16
        Top = 27
        Width = 145
        Height = 14
        Caption = '&Hotkey toggles orientation'
        TabOrder = 4
      end
    end
  end
  object plLoupe: TPanel
    Left = 406
    Top = 202
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 5
    object gbLoupe: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 177
      Caption = 'Loupe'
      TabOrder = 0
      object ckOpenLoupe: TCheckBox
        Left = 16
        Top = 141
        Width = 241
        Height = 17
        Caption = 'Double-clicking tray icon opens this tool'
        TabOrder = 0
      end
      object bnHotkeyZoomIn: TButton
        Tag = 2
        Left = 168
        Top = 22
        Width = 75
        Height = 25
        Caption = 'Hotkey...'
        TabOrder = 1
        OnClick = bnHotkeyClick
      end
      object ckHotkeyZoomIn: TCheckBox
        Left = 16
        Top = 27
        Width = 137
        Height = 14
        Caption = '&Hotkey zooms in'
        TabOrder = 2
      end
      object bnHotkeyZoomOut: TButton
        Tag = 3
        Left = 168
        Top = 62
        Width = 75
        Height = 25
        Caption = 'Hotkey...'
        TabOrder = 3
        OnClick = bnHotkeyClick
      end
      object ckHotkeyZoomOut: TCheckBox
        Left = 16
        Top = 59
        Width = 137
        Height = 14
        Caption = '&Hotkey zooms out'
        TabOrder = 4
      end
    end
  end
  object plInfo: TPanel
    Left = 694
    Top = 202
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 6
    object gbInfo: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 177
      Caption = 'Info'
      TabOrder = 0
      object ckOpenInfo: TCheckBox
        Left = 16
        Top = 141
        Width = 241
        Height = 17
        Caption = 'Double-clicking tray icon opens this tool'
        TabOrder = 0
      end
      object ckHotkeyColorCopy: TCheckBox
        Left = 16
        Top = 27
        Width = 225
        Height = 17
        Caption = 'Hotkey copies pixel color'
        TabOrder = 1
      end
      object gbAttributes: TGroupBox
        Left = 16
        Top = 59
        Width = 233
        Height = 57
        Hint = 'Specify attributes to add to the webcolor string.'
        Caption = 'When copying:'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
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
      object bnHotkeyColorCopy: TButton
        Left = 168
        Top = 22
        Width = 75
        Height = 25
        Caption = 'Hotkey...'
        TabOrder = 3
        OnClick = bnHotkeyClick
      end
    end
  end
  object plBaseconv: TPanel
    Left = 406
    Top = 402
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 7
    object gbBaseConv: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 177
      Caption = 'Base Converter'
      TabOrder = 0
      object ckOpenBaseconv: TCheckBox
        Left = 16
        Top = 141
        Width = 241
        Height = 17
        Caption = 'Double-clicking tray icon opens this tool'
        TabOrder = 0
      end
      object ckBinary: TCheckBox
        Left = 16
        Top = 27
        Width = 113
        Height = 17
        Caption = 'Hide binary field'
        TabOrder = 1
      end
    end
  end
  object plRef: TPanel
    Left = 118
    Top = 1
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 8
    object gbRef: TGroupBox
      Left = 8
      Top = 9
      Width = 265
      Height = 177
      Caption = 'Reference'
      TabOrder = 0
    end
  end
  object plGrabber: TPanel
    Left = 695
    Top = 402
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 9
    object gbGrab: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 177
      Caption = 'Screen Grabber'
      TabOrder = 0
      object ckHotkeyCaptureStart: TCheckBox
        Left = 16
        Top = 27
        Width = 138
        Height = 14
        Caption = '&Hotkey starts capture'
        TabOrder = 0
      end
      object bnHotkeyCaptureStart: TButton
        Tag = 5
        Left = 168
        Top = 22
        Width = 75
        Height = 25
        Caption = 'Hotkey...'
        TabOrder = 1
        OnClick = bnHotkeyClick
      end
      object ckAutosave: TCheckBox
        Left = 16
        Top = 55
        Width = 113
        Height = 13
        Hint = 'Adds TopTools to the run section in the registry'
        Caption = '&Autosave to file'
        ParentShowHint = False
        ShowHint = False
        TabOrder = 2
      end
      object gbAutosave: TGroupBox
        Left = 16
        Top = 79
        Width = 233
        Height = 82
        Hint = 'Specify attributes to add to the webcolor string.'
        Caption = 'Autosave Options'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        object lbDirectory: TLabel
          Left = 16
          Top = 24
          Width = 45
          Height = 13
          Caption = 'Directory:'
        end
        object lbBaseFilename: TLabel
          Left = 16
          Top = 48
          Width = 45
          Height = 13
          Caption = 'Filename:'
        end
      end
    end
  end
  object plHotkeys: TPanel
    Left = 118
    Top = 402
    Width = 281
    Height = 193
    BevelOuter = bvNone
    TabOrder = 10
    object gbHotkeys: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 177
      Caption = 'Define Hotkeys'
      TabOrder = 0
      object lbAction: TLabel
        Left = 16
        Top = 75
        Width = 33
        Height = 13
        Caption = '&Action:'
      end
      object lbHotkey: TLabel
        Left = 16
        Top = 111
        Width = 37
        Height = 13
        Caption = '&Hotkey:'
        FocusControl = edHotkey
      end
      object cbHotkeyAction: TComboBox
        Left = 64
        Top = 71
        Width = 177
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
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
        Left = 16
        Top = 27
        Width = 233
        Height = 33
        BorderStyle = bsNone
        Color = clBtnFace
        Lines.Strings = (
          'Define key combinations for actions to occur no '
          'matter which application has keyboard focus.')
        ReadOnly = True
        TabOrder = 1
      end
      object edHotkey: TEdit
        Left = 64
        Top = 108
        Width = 177
        Height = 21
        ReadOnly = True
        TabOrder = 2
      end
    end
  end
end
