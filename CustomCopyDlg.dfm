object CustomCopyDlg: TCustomCopyDlg
  Left = 903
  Top = 227
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Define Template'
  ClientHeight = 389
  ClientWidth = 425
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ButtonPanel: TPanel
    Left = 0
    Top = 0
    Width = 425
    Height = 25
    Align = alTop
    BevelOuter = bvNone
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object Button1: TButton
      Left = 0
      Top = 0
      Width = 25
      Height = 25
      Hint = 'mouse x-position'
      Caption = '[&x]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = AddPlaceholder
    end
    object Button2: TButton
      Left = 24
      Top = 0
      Width = 25
      Height = 25
      Hint = 'mouse y-position'
      Caption = '[&y]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = AddPlaceholder
    end
    object Button3: TButton
      Left = 56
      Top = 0
      Width = 25
      Height = 25
      Hint = 'decimal red value'
      Caption = '[&r]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = AddPlaceholder
    end
    object Button4: TButton
      Left = 80
      Top = 0
      Width = 25
      Height = 25
      Hint = 'decimal green value'
      Caption = '[&g]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = AddPlaceholder
    end
    object Button5: TButton
      Left = 104
      Top = 0
      Width = 25
      Height = 25
      Hint = 'decimal blue value'
      Caption = '[&b]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = AddPlaceholder
    end
    object Button6: TButton
      Left = 136
      Top = 0
      Width = 25
      Height = 25
      Hint = '2 digit hexadecimal red value'
      Caption = '[R]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = AddPlaceholder
    end
    object Button7: TButton
      Left = 160
      Top = 0
      Width = 25
      Height = 25
      Hint = '2 digit hexadecimal green value'
      Caption = '[G]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 6
      OnClick = AddPlaceholder
    end
    object Button8: TButton
      Left = 184
      Top = 0
      Width = 25
      Height = 25
      Hint = '2 digit hexadecimal blue value'
      Caption = '[B]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 7
      OnClick = AddPlaceholder
    end
    object Button9: TButton
      Left = 216
      Top = 0
      Width = 25
      Height = 25
      Hint = 'HSV hue'
      Caption = '[&h]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 8
      OnClick = AddPlaceholder
    end
    object Button10: TButton
      Left = 240
      Top = 0
      Width = 25
      Height = 25
      Hint = 'HSV saturation'
      Caption = '[&s]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 9
      OnClick = AddPlaceholder
    end
    object Button11: TButton
      Left = 264
      Top = 0
      Width = 25
      Height = 25
      Hint = 'HSV value'
      Caption = '[&v]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 10
      OnClick = AddPlaceholder
    end
    object Button12: TButton
      Left = 296
      Top = 0
      Width = 25
      Height = 25
      Hint = 'lowercase webcolor'
      Caption = '[w]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 11
      OnClick = AddPlaceholder
    end
    object Button13: TButton
      Left = 320
      Top = 0
      Width = 25
      Height = 25
      Hint = 'uppercase webcolor'
      Caption = '[&W]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 12
      OnClick = AddPlaceholder
    end
    object Button14: TButton
      Left = 352
      Top = 0
      Width = 25
      Height = 25
      Hint = 'line break'
      Caption = '[&n]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 13
      OnClick = AddPlaceholder
    end
    object Button15: TButton
      Left = 376
      Top = 0
      Width = 25
      Height = 25
      Hint = 'literal ['
      Caption = '[&[]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 14
      OnClick = AddPlaceholder
    end
    object Button16: TButton
      Left = 400
      Top = 0
      Width = 25
      Height = 25
      Hint = 'literal ]'
      Caption = '[&]]'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 15
      OnClick = AddPlaceholder
    end
  end
  object HelpPanel: TPanel
    Left = 0
    Top = 169
    Width = 425
    Height = 220
    BevelInner = bvLowered
    Color = clInfoBk
    TabOrder = 1
    object HelpMemo: TMemo
      Left = 16
      Top = 5
      Width = 392
      Height = 213
      BorderStyle = bsNone
      Color = clInfoBk
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      Lines.Strings = (
        'Possible placeholders:'
        ''
        '     [x], [y] - mouse coordinates'
        '[r], [g], [b] - decimal value of RGB color'
        '[R], [G], [B] - 2 digit hexadecimal value of RGB color'
        '[h], [s], [v] - values of HSV color'
        '          [[] - literal ['
        '          []] - literal ]'
        '          [n] - line break'
        '          [w] - lower-case webcolor (without #)'
        '          [W] - upper-case webcolor, same as [R][G][B]'
        ''
        'Everything not enclosed in square brackets is '
        'interpreted as literal text, unknown placeholders are '
        'ignored.')
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
    end
  end
  object EditPanel: TPanel
    Left = 0
    Top = 25
    Width = 425
    Height = 144
    Align = alTop
    TabOrder = 2
    object Label1: TLabel
      Left = 16
      Top = 15
      Width = 47
      Height = 13
      Caption = 'Template:'
    end
    object Label2: TLabel
      Left = 16
      Top = 47
      Width = 41
      Height = 13
      Caption = 'Preview:'
    end
    object edMask: TEdit
      Left = 72
      Top = 12
      Width = 337
      Height = 21
      TabOrder = 0
      OnChange = edMaskChange
    end
    object edPreview: TMemo
      Left = 72
      Top = 44
      Width = 337
      Height = 49
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 1
      WordWrap = False
    end
    object bnOK: TButton
      Left = 248
      Top = 106
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 2
      OnClick = bnOKClick
    end
    object ckHelp: TCheckBox
      Left = 16
      Top = 110
      Width = 97
      Height = 17
      Caption = '&Show Help'
      TabOrder = 3
      OnClick = ckHelpClick
    end
    object bnCancel: TButton
      Left = 336
      Top = 106
      Width = 75
      Height = 25
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 4
      OnClick = bnCancelClick
    end
  end
end
