object FormAgentDetails: TFormAgentDetails
  Left = 571
  Top = 99
  BorderStyle = bsSingle
  Caption = 'Details of Agent <name>'
  ClientHeight = 506
  ClientWidth = 369
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxGeneralInfo: TGroupBox
    Left = 0
    Top = 0
    Width = 369
    Height = 105
    Align = alTop
    Caption = ' General Information '
    TabOrder = 0
    object LabelName: TLabel
      Left = 24
      Top = 16
      Width = 65
      Height = 21
      AutoSize = False
      Caption = 'Name:'
      Layout = tlCenter
    end
    object LabelInstrument: TLabel
      Left = 24
      Top = 32
      Width = 65
      Height = 21
      AutoSize = False
      Caption = 'Instrument:'
      Layout = tlCenter
    end
    object LabelStatus: TLabel
      Left = 24
      Top = 48
      Width = 65
      Height = 21
      AutoSize = False
      Caption = 'Status:'
      Layout = tlCenter
    end
    object LabelDefaultRole: TLabel
      Left = 24
      Top = 64
      Width = 73
      Height = 21
      AutoSize = False
      Caption = 'Default Role:'
      Layout = tlCenter
    end
  end
  object GroupBoxPerformanceHistory: TGroupBox
    Left = 0
    Top = 368
    Width = 369
    Height = 138
    Align = alBottom
    Caption = ' Performance History '
    TabOrder = 1
  end
  object GroupBoxSettings: TGroupBox
    Left = 0
    Top = 105
    Width = 369
    Height = 263
    Align = alClient
    Caption = ' Settings '
    TabOrder = 2
    object LabelDynamics: TLabel
      Left = 16
      Top = 24
      Width = 129
      Height = 21
      Hint = 'A multiplier factor to the dynamics of the notes'
      AutoSize = False
      Caption = 'Dynamics Scaling Factor:'
      ParentShowHint = False
      ShowHint = True
      Layout = tlCenter
    end
    object LabelTemporalDev: TLabel
      Left = 16
      Top = 80
      Width = 153
      Height = 21
      Hint = 'Temporal deviations as the proportion between consecutive beats'
      AutoSize = False
      Caption = 'Temporal Deviations (Swing):'
      ParentShowHint = False
      ShowHint = True
      Layout = tlCenter
    end
    object LabelAnticipation: TLabel
      Left = 16
      Top = 136
      Width = 73
      Height = 21
      Hint = 'Anticipation relative to each tatum'
      AutoSize = False
      Caption = 'Anticipation:'
      ParentShowHint = False
      ShowHint = True
      Layout = tlCenter
    end
    object LabelVertHorizRatio: TLabel
      Left = 16
      Top = 192
      Width = 185
      Height = 21
      Hint = 'Anticipation relative to each tatum'
      AutoSize = False
      Caption = 'Vertical/Horizontal Evaluation Ratio:'
      ParentShowHint = False
      ShowHint = True
      Layout = tlCenter
    end
    object TrackBarDynamics: TTrackBar
      Left = 24
      Top = 48
      Width = 305
      Height = 25
      Max = 180
      Orientation = trHorizontal
      ParentShowHint = False
      PageSize = 10
      Frequency = 10
      Position = 100
      SelEnd = 0
      SelStart = 0
      ShowHint = True
      TabOrder = 0
      ThumbLength = 14
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = TrackBarDynamicsChange
    end
    object TrackBarTemporalDev: TTrackBar
      Left = 24
      Top = 104
      Width = 305
      Height = 25
      Orientation = trHorizontal
      ParentShowHint = False
      PageSize = 10
      Frequency = 5
      Position = 5
      SelEnd = 0
      SelStart = 0
      ShowHint = True
      TabOrder = 1
      ThumbLength = 14
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = TrackBarTemporalDevChange
    end
    object TrackBarAnticipation: TTrackBar
      Left = 24
      Top = 160
      Width = 305
      Height = 25
      Max = 14
      Min = 6
      Orientation = trHorizontal
      ParentShowHint = False
      PageSize = 10
      Frequency = 2
      Position = 10
      SelEnd = 0
      SelStart = 0
      ShowHint = True
      TabOrder = 2
      ThumbLength = 14
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = TrackBarAnticipationChange
    end
    object TrackBarVertHorizRatio: TTrackBar
      Left = 24
      Top = 216
      Width = 305
      Height = 25
      Max = 32
      Orientation = trHorizontal
      ParentShowHint = False
      PageSize = 10
      Frequency = 4
      Position = 16
      SelEnd = 0
      SelStart = 0
      ShowHint = True
      TabOrder = 3
      ThumbLength = 14
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = TrackBarVertHorizRatioChange
    end
  end
end
