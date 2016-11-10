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
    TabOrder = 4
    object bnOk: TButton
      Left = 239
      Top = 6
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 2
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
      TabOrder = 3
      OnClick = bnCancelClick
    end
    object bnImport: TButton
      Left = 8
      Top = 6
      Width = 75
      Height = 25
      Caption = '&Import...'
      TabOrder = 0
      OnClick = bnImportClick
    end
    object bnExport: TButton
      Left = 96
      Top = 6
      Width = 75
      Height = 25
      Caption = 'E&xport...'
      TabOrder = 1
      OnClick = bnExportClick
    end
  end
  object bnAdd: TButton
    Left = 328
    Top = 8
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
    TabOrder = 3
    OnClick = bnRemoveClick
  end
  object bnEdit: TButton
    Left = 328
    Top = 40
    Width = 75
    Height = 25
    Caption = '&Edit...'
    TabOrder = 2
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
    PopupMenu = ListViewMenu
    TabOrder = 0
    ViewStyle = vsReport
    OnChange = ListViewChange
    OnDblClick = bnEditClick
    OnEditing = ListViewEditing
    OnDragDrop = ListViewDragDrop
    OnDragOver = ListViewDragOver
  end
  object ListViewMenu: TPopupMenu
    Left = 360
    Top = 120
    object Edit1: TMenuItem
      Caption = '&Edit...'
      OnClick = bnEditClick
    end
    object Add1: TMenuItem
      Caption = '&Add...'
      OnClick = bnAddClick
    end
    object Remove1: TMenuItem
      Caption = '&Remove'
      OnClick = bnRemoveClick
    end
  end
end
