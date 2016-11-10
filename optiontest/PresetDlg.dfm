object PresetDialog: TPresetDialog
  Left = 1174
  Top = 504
  BorderStyle = bsDialog
  Caption = 'Grabber Presets'
  ClientHeight = 207
  ClientWidth = 411
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
    Width = 411
    Height = 39
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 3
    object bnOk: TButton
      Left = 239
      Top = 6
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 1
      OnClick = bnOkClick
    end
    object bnCancel: TButton
      Left = 327
      Top = 6
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 0
      OnClick = bnCancelClick
    end
    object bnImport: TButton
      Left = 8
      Top = 6
      Width = 75
      Height = 25
      Caption = '&Import...'
      TabOrder = 2
      OnClick = bnImportClick
    end
    object bnExport: TButton
      Left = 96
      Top = 6
      Width = 75
      Height = 25
      Caption = '&Export...'
      TabOrder = 3
      OnClick = bnExportClick
    end
  end
  object bnAdd: TButton
    Left = 328
    Top = 40
    Width = 75
    Height = 25
    Caption = '&Add...'
    TabOrder = 1
    OnClick = bnAddClick
  end
  object bnRemove: TButton
    Left = 328
    Top = 72
    Width = 75
    Height = 25
    Caption = '&Remove'
    TabOrder = 2
    OnClick = bnRemoveClick
  end
  object bnEdit: TButton
    Left = 328
    Top = 8
    Width = 75
    Height = 25
    Caption = '&Edit...'
    TabOrder = 0
    OnClick = bnEditClick
  end
  object ListView: TListView
    Left = 8
    Top = 8
    Width = 305
    Height = 153
    Columns = <>
    ColumnClick = False
    DragMode = dmAutomatic
    TabOrder = 4
    ViewStyle = vsReport
    OnDblClick = bnEditClick
    OnDragDrop = ListViewDragDrop
    OnDragOver = ListViewDragOver
  end
end
