object ToolOptionsDialog: TToolOptionsDialog
  Left = 170
  Top = 121
  BorderStyle = bsDialog
  Caption = 'TopTools Options'
  ClientHeight = 761
  ClientWidth = 974
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lvSelectOption: TListView
    Left = 10
    Top = 12
    Width = 105
    Height = 205
    Columns = <>
    ColumnClick = False
    HideSelection = False
    HotTrackStyles = [htUnderlineHot]
    Items.Data = {
      D50000000700000000000000FFFFFFFFFFFFFFFF00000000000000000747656E
      6572616C00000000FFFFFFFFFFFFFFFF00000000000000000552756C65720000
      0000FFFFFFFFFFFFFFFF0000000000000000054C6F75706500000000FFFFFFFF
      FFFFFFFF000000000000000004496E666F00000000FFFFFFFFFFFFFFFF000000
      00000000000E4261736520436F6E76657274657200000000FFFFFFFFFFFFFFFF
      00000000000000000E53637265656E204772616262657200000000FFFFFFFFFF
      FFFFFF000000000000000009547261792049636F6E}
    ReadOnly = True
    RowSelect = True
    ShowColumnHeaders = False
    TabOrder = 0
    ViewStyle = vsList
    OnChange = lvSelectOptionChange
  end
  object plGeneral: TPanel
    Left = 406
    Top = 2
    Width = 281
    Height = 230
    BevelOuter = bvNone
    TabOrder = 1
    object gbGeneral: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 210
      Caption = 'General'
      TabOrder = 0
      object ckAutoStart: TCheckBox
        Left = 16
        Top = 55
        Width = 113
        Height = 17
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
        Height = 17
        Caption = '&Always on top'
        TabOrder = 0
      end
      object ckSaveToolstate: TCheckBox
        Left = 16
        Top = 83
        Width = 177
        Height = 17
        Caption = '&Preserve tool states on exit'
        TabOrder = 2
      end
      object ckTrayApp: TCheckBox
        Left = 16
        Top = 176
        Width = 241
        Height = 17
        Caption = '&Reside in sytem tray (like older versions did)'
        TabOrder = 3
      end
      object ckSingleton: TCheckBox
        Left = 16
        Top = 112
        Width = 225
        Height = 17
        Caption = 'Allow only &one instance to run at a time'
        TabOrder = 4
      end
    end
  end
  object plRuler: TPanel
    Left = 694
    Top = 2
    Width = 281
    Height = 230
    BevelOuter = bvNone
    TabOrder = 2
    object gbRuler: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 210
      Caption = 'Ruler'
      TabOrder = 0
      object lbLength: TLabel
        Left = 16
        Top = 28
        Width = 36
        Height = 13
        Caption = '&Length:'
        FocusControl = edLength
      end
      object lbPixels: TLabel
        Left = 139
        Top = 28
        Width = 26
        Height = 13
        Caption = 'pixels'
      end
      object lbTransparency: TLabel
        Left = 34
        Top = 139
        Width = 68
        Height = 13
        Caption = 'Transparency:'
        FocusControl = edTransparency
      end
      object lbPercent: TLabel
        Left = 158
        Top = 139
        Width = 8
        Height = 13
        Caption = '&%'
      end
      object edLength: TEdit
        Left = 61
        Top = 25
        Width = 52
        Height = 21
        TabOrder = 0
        Text = '50'
      end
      object udLength: TUpDown
        Left = 113
        Top = 25
        Width = 15
        Height = 21
        Associate = edLength
        Min = 50
        Max = 2000
        Position = 50
        TabOrder = 1
        Thousands = False
        Wrap = False
      end
      object ckNudgeRuler: TCheckBox
        Left = 16
        Top = 70
        Width = 233
        Height = 17
        Caption = '&Arrow keys nudge ruler (+Ctrl nudges cursor)'
        TabOrder = 2
      end
      object cbTransparent: TCheckBox
        Left = 16
        Top = 112
        Width = 241
        Height = 17
        Caption = '&Transparent (but invisible for the loupe)'
        TabOrder = 3
      end
      object edTransparency: TEdit
        Left = 111
        Top = 136
        Width = 25
        Height = 21
        TabOrder = 4
        Text = '50'
      end
      object udTransparency: TUpDown
        Left = 136
        Top = 136
        Width = 15
        Height = 21
        Associate = edTransparency
        Min = 0
        Position = 50
        TabOrder = 5
        Thousands = False
        Wrap = False
      end
    end
  end
  object plLoupe: TPanel
    Left = 406
    Top = 306
    Width = 281
    Height = 230
    BevelOuter = bvNone
    TabOrder = 3
    object gbLoupe: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 210
      Caption = 'Loupe'
      TabOrder = 0
      object lbRefreshrate: TLabel
        Left = 16
        Top = 28
        Width = 77
        Height = 13
        Caption = '&Refresh interval:'
        FocusControl = edRefresh
      end
      object lbMilliseconds: TLabel
        Left = 186
        Top = 28
        Width = 56
        Height = 13
        Caption = 'milliseconds'
      end
      object edRefresh: TEdit
        Left = 104
        Top = 25
        Width = 57
        Height = 21
        TabOrder = 0
        Text = '100'
      end
      object udRefresh: TUpDown
        Left = 161
        Top = 25
        Width = 15
        Height = 21
        Associate = edRefresh
        Min = 50
        Max = 5000
        Increment = 10
        Position = 100
        TabOrder = 1
        Wrap = False
      end
      object hkpZoomIn: THotkeyPanel
        Left = 8
        Top = 51
        Width = 250
        Height = 65
        BevelOuter = bvNone
        TabOrder = 2
        CheckboxText = 'Hotkey zooms &in'
      end
      object hkpZoomOut: THotkeyPanel
        Left = 8
        Top = 109
        Width = 250
        Height = 65
        BevelOuter = bvNone
        TabOrder = 3
        CheckboxText = 'Hotkey zooms &out'
      end
    end
  end
  object plInfo: TPanel
    Left = 694
    Top = 306
    Width = 281
    Height = 230
    BevelOuter = bvNone
    TabOrder = 4
    object gbInfo: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 210
      Caption = 'Info'
      TabOrder = 0
      object gbAttributes: TGroupBox
        Left = 16
        Top = 85
        Width = 233
        Height = 57
        Hint = 'Specify attributes to add to the webcolor string.'
        Caption = 'When copying:'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
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
      object hkpColorCopy: THotkeyPanel
        Left = 8
        Top = 19
        Width = 250
        Height = 67
        BevelOuter = bvNone
        TabOrder = 0
        CheckboxText = '&Hotkey copies pixel color'
      end
    end
  end
  object plBaseconv: TPanel
    Left = 408
    Top = 538
    Width = 281
    Height = 230
    BevelOuter = bvNone
    TabOrder = 5
    object gbBaseConv: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 210
      Caption = 'Base Converter'
      TabOrder = 0
      object ckBinary: TCheckBox
        Left = 16
        Top = 27
        Width = 113
        Height = 17
        Caption = '&Show binary field'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
    end
  end
  object plRef: TPanel
    Left = 118
    Top = 0
    Width = 281
    Height = 224
    BevelOuter = bvNone
    TabOrder = 9
    object gbRef: TGroupBox
      Left = 8
      Top = 7
      Width = 265
      Height = 210
      Caption = 'Reference'
      TabOrder = 0
    end
  end
  object plGrabber: TPanel
    Left = 695
    Top = 538
    Width = 281
    Height = 230
    BevelOuter = bvNone
    TabOrder = 6
    object gbGrab: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 210
      Caption = 'Screen Grabber'
      TabOrder = 0
      object hkpGrabScreen: THotkeyPanel
        Left = 7
        Top = 19
        Width = 250
        Height = 67
        BevelOuter = bvNone
        TabOrder = 0
        CheckboxText = '&Hotkey starts capture'
      end
      object gbAutoSave: TGroupBox
        Left = 16
        Top = 99
        Width = 233
        Height = 57
        Caption = 'Autosave to file'
        TabOrder = 1
        object ckAutosave: TCheckBox
          Left = 16
          Top = 24
          Width = 113
          Height = 17
          Hint = 'Adds TopTools to the run section in the registry'
          Caption = '&Enable'
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
        end
        object bnAutosaveOptions: TButton
          Left = 144
          Top = 20
          Width = 75
          Height = 25
          Caption = 'Configure...'
          TabOrder = 1
          OnClick = bnAutosaveOptionsClick
        end
      end
      object ckShowLoupeOnGrab: TCheckBox
        Left = 16
        Top = 176
        Width = 225
        Height = 17
        Caption = '&Show Loupe during screen capture'
        TabOrder = 2
      end
    end
  end
  object plMarker: TPanel
    Left = 400
    Top = 264
    Width = 105
    Height = 17
    Caption = 'RightBottomMarker'
    TabOrder = 10
    Visible = False
  end
  object bnOk: TButton
    Left = 224
    Top = 230
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 7
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 315
    Top = 230
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 8
  end
  object plTrayicon: TPanel
    Left = 118
    Top = 306
    Width = 281
    Height = 230
    BevelOuter = bvNone
    TabOrder = 11
    object gbTrayicon: TGroupBox
      Left = 8
      Top = 8
      Width = 265
      Height = 210
      Caption = 'Tray Icon'
      TabOrder = 0
      object gbDoubleClick: TGroupBox
        Left = 16
        Top = 27
        Width = 233
        Height = 105
        Caption = '&Double-clicking tray icon shows:'
        TabOrder = 0
        object ckOpenRuler: TCheckBox
          Left = 14
          Top = 48
          Width = 100
          Height = 17
          Caption = '&Ruler'
          TabOrder = 0
        end
        object ckOpenInfo: TCheckBox
          Left = 117
          Top = 24
          Width = 100
          Height = 17
          Caption = '&Info'
          TabOrder = 1
        end
        object ckOpenLoupe: TCheckBox
          Left = 15
          Top = 72
          Width = 100
          Height = 17
          Caption = '&Loupe'
          TabOrder = 2
        end
        object ckOpenBaseconv: TCheckBox
          Left = 117
          Top = 48
          Width = 100
          Height = 17
          Caption = '&Base Converter'
          TabOrder = 3
        end
        object ckOpenToolbar: TCheckBox
          Left = 13
          Top = 24
          Width = 100
          Height = 17
          Caption = '&Toolbar'
          TabOrder = 4
        end
        object ckOptionsDlg: TCheckBox
          Left = 117
          Top = 72
          Width = 100
          Height = 17
          Caption = '&Options Dialog'
          TabOrder = 5
        end
      end
      object hkpDoubleClick: THotkeyPanel
        Left = 9
        Top = 136
        Width = 250
        Height = 67
        BevelOuter = bvNone
        TabOrder = 1
        CheckboxText = '&Hotkey double-clicks tray icon'
      end
    end
  end
end
