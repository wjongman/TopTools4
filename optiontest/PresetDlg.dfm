object PresetDialog: TPresetDialog
  Left = 619
  Top = 253
  Width = 346
  Height = 268
  Caption = 'Grabber Presets'
  Color = clBtnFace
  Constraints.MaxWidth = 346
  Constraints.MinHeight = 120
  Constraints.MinWidth = 346
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnContextPopup = FormContextPopup
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 205
    Width = 338
    Height = 36
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object bnLoad: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Load...'
      TabOrder = 0
      OnClick = bnLoadClick
    end
    object bnOk: TButton
      Left = 135
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 1
      OnClick = bnOkClick
    end
    object bnCancel: TButton
      Left = 255
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 2
      OnClick = bnCancelClick
    end
  end
  object Grid: TStringGrid
    Left = 0
    Top = 0
    Width = 338
    Height = 153
    Align = alTop
    ColCount = 6
    DefaultColWidth = 48
    DefaultRowHeight = 17
    FixedCols = 0
    RowCount = 8
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goTabs, goRowSelect, goThumbTracking]
    ParentShowHint = False
    ScrollBars = ssVertical
    ShowHint = True
    TabOrder = 0
    OnGetEditMask = GridGetEditMask
    OnGetEditText = GridGetEditText
    OnSelectCell = GridSelectCell
    OnSetEditText = GridSetEditText
    ColWidths = (
      118
      48
      48
      48
      48
      48)
  end
  object bnAdd: TButton
    Left = 8
    Top = 168
    Width = 75
    Height = 25
    Caption = 'Add'
    TabOrder = 2
    OnClick = bnAddClick
  end
  object bnRemove: TButton
    Left = 104
    Top = 168
    Width = 75
    Height = 25
    Caption = 'Remove'
    TabOrder = 3
    OnClick = bnRemoveClick
  end
  object bnEdit: TButton
    Left = 192
    Top = 168
    Width = 75
    Height = 25
    Caption = 'Edit...'
    TabOrder = 4
    OnClick = bnEditClick
  end
end
