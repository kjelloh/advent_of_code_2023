object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Advent of Code 2023 day 03'
  ClientHeight = 649
  ClientWidth = 628
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  TextHeight = 15
  object Splitter1: TSplitter
    Left = 0
    Top = 161
    Width = 628
    Height = 1
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 120
    ExplicitTop = 175
    ExplicitWidth = 297
  end
  object Splitter2: TSplitter
    Left = 0
    Top = 588
    Width = 628
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitTop = 203
    ExplicitWidth = 388
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 628
    Height = 161
    Align = alTop
    Caption = 'Panel1'
    TabOrder = 0
    object Memo1: TMemo
      Left = 1
      Top = 1
      Width = 626
      Height = 159
      Align = alClient
      Lines.Strings = (
        '')
      TabOrder = 0
      ExplicitLeft = 120
      ExplicitTop = 56
      ExplicitWidth = 185
      ExplicitHeight = 89
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 591
    Width = 628
    Height = 58
    Align = alBottom
    TabOrder = 1
    ExplicitTop = 384
    object Button1: TButton
      Left = 16
      Top = 15
      Width = 75
      Height = 25
      Caption = 'Solve'
      TabOrder = 0
      OnClick = Solve1Click
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 162
    Width = 628
    Height = 426
    Align = alClient
    Caption = 'Panel3'
    TabOrder = 2
    ExplicitLeft = 224
    ExplicitTop = 336
    ExplicitWidth = 185
    ExplicitHeight = 41
    object Memo2: TMemo
      Left = 1
      Top = 1
      Width = 626
      Height = 424
      Align = alClient
      TabOrder = 0
      ExplicitLeft = 208
      ExplicitTop = 232
      ExplicitWidth = 185
      ExplicitHeight = 89
    end
  end
  object MainMenu1: TMainMenu
    Left = 144
    Top = 33
    object File1: TMenuItem
      Caption = 'File'
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
    object Puzzle1: TMenuItem
      Caption = 'Puzzle'
      object Solve1: TMenuItem
        Caption = 'Solve'
        OnClick = Solve1Click
      end
    end
  end
end