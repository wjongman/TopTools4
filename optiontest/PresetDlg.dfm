object PresetDialog: TPresetDialog
  Left = 1219
  Top = 619
  Width = 577
  Height = 234
  Caption = 'Grabber Presets'
  Color = clBtnFace
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
    Top = 168
    Width = 569
    Height = 39
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 6
    object bnOk: TButton
      Left = 7
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
      Left = 167
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 0
      OnClick = bnCancelClick
    end
  end
  object bnAdd: TButton
    Left = 168
    Top = 40
    Width = 75
    Height = 25
    Caption = '&Add...'
    TabOrder = 2
    OnClick = bnAddClick
  end
  object bnRemove: TButton
    Left = 168
    Top = 72
    Width = 75
    Height = 25
    Caption = '&Remove'
    TabOrder = 3
    OnClick = bnRemoveClick
  end
  object bnEdit: TButton
    Left = 168
    Top = 8
    Width = 75
    Height = 25
    Caption = '&Edit...'
    TabOrder = 1
    OnClick = bnEditClick
  end
  object ListBox: TListBox
    Left = 8
    Top = 8
    Width = 153
    Height = 153
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnDblClick = bnEditClick
    OnDragDrop = ListBoxDragDrop
    OnDragOver = ListBoxDragOver
    OnEndDrag = ListBoxEndDrag
    OnMouseDown = ListBoxMouseDown
    OnMouseMove = ListBoxMouseMove
    OnMouseUp = ListBoxMouseUp
    OnStartDrag = ListBoxStartDrag
  end
  object bnImport: TButton
    Left = 168
    Top = 104
    Width = 75
    Height = 25
    Caption = '&Import...'
    TabOrder = 4
    OnClick = bnImportClick
  end
  object bnExport: TButton
    Left = 168
    Top = 136
    Width = 75
    Height = 25
    Caption = '&Export...'
    TabOrder = 5
    OnClick = bnExportClick
  end
  object ListView: TListView
    Left = 248
    Top = 8
    Width = 313
    Height = 153
    Columns = <>
    DragMode = dmAutomatic
    TabOrder = 7
    ViewStyle = vsReport
    OnDragDrop = ListViewDragDrop
    OnDragOver = ListViewDragOver
  end
end
