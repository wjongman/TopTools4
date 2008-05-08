object MainForm: TMainForm
  Left = 1397
  Top = 597
  BorderStyle = bsSingle
  Caption = 'TopTools 4'
  ClientHeight = 144
  ClientWidth = 240
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = TrayMenu
  PixelsPerInch = 96
  TextHeight = 13
  object TrayActions: TActionList
    Left = 72
    Top = 16
    object actCapture: TAction
      Category = 'Main'
      Caption = 'Grab Screen'
      Hint = 'Capture a portion of the screen'
      ImageIndex = 4
      OnExecute = actCommandExecute
    end
    object actRuler: TAction
      Category = 'Main'
      Caption = 'Ruler'
      Hint = 'Ruler'
      ImageIndex = 0
      OnExecute = actCommandExecute
    end
    object actLoupe: TAction
      Category = 'Main'
      Caption = 'Loupe'
      Hint = 'Loupe'
      ImageIndex = 1
      OnExecute = actCommandExecute
    end
    object actBaseConv: TAction
      Category = 'Main'
      Caption = 'Base Converter'
      Hint = 'Base Converter'
      ImageIndex = 3
      OnExecute = actCommandExecute
    end
    object actInfo: TAction
      Category = 'Main'
      Caption = 'Info'
      Hint = 'Info'
      ImageIndex = 2
      OnExecute = actCommandExecute
    end
    object actAbout: TAction
      Category = 'Main'
      Caption = 'About...'
      Hint = 'About TopTools'
      ImageIndex = 6
      OnExecute = actAboutExecute
    end
    object actExit: TAction
      Category = 'Main'
      Caption = 'Exit'
      Hint = 'Exit TopTools'
      ImageIndex = 7
      OnExecute = actExitExecute
    end
    object actHelp: TAction
      Category = 'Main'
      Caption = 'Help'
      Hint = 'Help'
      ImageIndex = 6
    end
    object actOnTop: TAction
      Category = 'Options'
      Caption = 'Stay on &Top'
      Hint = 'Stay on Top'
    end
    object actTrayIcon: TAction
      Category = 'Options'
      Caption = 'Tray Icon'
      Hint = 'Tray Icon'
    end
    object actOrientation: TAction
      Category = 'Ruler'
      Caption = 'Orientation'
      Hint = 'Orientation'
    end
    object actZero: TAction
      Category = 'Ruler'
      Caption = 'Slide to Zero'
      Hint = 'Slide to Zero'
    end
    object actOptions: TAction
      Category = 'Options'
      Caption = 'Options...'
      Hint = 'Options'
      ImageIndex = 5
      OnExecute = actOptionsExecute
    end
    object actControl: TAction
      Category = 'Main'
      Caption = 'Main window'
      OnExecute = actCommandExecute
    end
    object actGrab: TAction
      Category = 'Main'
      Caption = 'Grab Screen...'
    end
  end
  object TrayMenu: TPopupMenu
    Left = 120
    Top = 16
    object miToolBar: TMenuItem
      Action = actControl
      Default = True
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miRuler: TMenuItem
      Action = actRuler
    end
    object miLoupe: TMenuItem
      Action = actLoupe
    end
    object miInfo: TMenuItem
      Action = actInfo
    end
    object miBaseConv: TMenuItem
      Action = actBaseConv
    end
    object miGrabber: TMenuItem
      Action = actCapture
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object miOptions: TMenuItem
      Action = actOptions
    end
    object miAbout: TMenuItem
      Action = actAbout
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miExit: TMenuItem
      Action = actExit
    end
  end
  object Timer: TTimer
    Interval = 100
    OnTimer = HandleTimerEvent
    Left = 24
    Top = 16
  end
end
