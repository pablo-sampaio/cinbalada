object FrameExperimentResult: TFrameExperimentResult
  Left = 0
  Top = 0
  Width = 690
  Height = 381
  Align = alClient
  TabOrder = 0
  DesignSize = (
    690
    381)
  object LabelNumFiles: TLabel
    Left = 24
    Top = 8
    Width = 169
    Height = 21
    AutoSize = False
    Caption = 'Number of Experiments Loaded:'
    Layout = tlCenter
  end
  object EditNumFiles: TEdit
    Left = 184
    Top = 8
    Width = 33
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 0
    Text = '0'
  end
  object GroupBox1: TGroupBox
    Left = 16
    Top = 32
    Width = 321
    Height = 161
    Caption = ' Best Patterns Frequency '
    TabOrder = 1
    DesignSize = (
      321
      161)
    object ListViewBestFreq: TListView
      Left = 8
      Top = 16
      Width = 305
      Height = 113
      Anchors = [akLeft, akTop, akRight, akBottom]
      Columns = <
        item
          Caption = 'Round'
        end
        item
          Caption = 'Random'
          Width = 80
        end
        item
          Caption = 'Single'
          Width = 80
        end
        item
          Caption = 'Role'
          Width = 80
        end>
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
    object ComboBoxBestFreq: TComboBox
      Left = 8
      Top = 133
      Width = 145
      Height = 21
      Style = csDropDownList
      Anchors = [akRight, akBottom]
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 1
      Text = 'First / Best'
      OnChange = ComboBoxBestFreqChange
      Items.Strings = (
        'First / Best'
        'Second'
        'Third / Worst')
    end
  end
  object GroupBox2: TGroupBox
    Left = 355
    Top = 172
    Width = 321
    Height = 103
    Anchors = [akRight, akBottom]
    Caption = ' Quality Notes Frequency Summary '
    TabOrder = 2
    DesignSize = (
      321
      103)
    object ListViewNotesFreqAll: TListView
      Left = 8
      Top = 15
      Width = 305
      Height = 80
      Anchors = [akLeft, akTop, akRight, akBottom]
      Columns = <
        item
          Caption = 'Note'
        end
        item
          Caption = 'Random'
          Width = 80
        end
        item
          Caption = 'Single'
          Width = 80
        end
        item
          Caption = 'Role'
          Width = 80
        end>
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
  object GroupBox3: TGroupBox
    Left = 356
    Top = 279
    Width = 321
    Height = 92
    Anchors = []
    Caption = ' Variability'
    TabOrder = 3
    DesignSize = (
      321
      92)
    object ListViewVariation: TListView
      Left = 8
      Top = 15
      Width = 305
      Height = 69
      Anchors = [akLeft, akTop, akRight, akBottom]
      Columns = <
        item
          Width = 110
        end
        item
          Caption = 'Random'
          Width = 90
        end
        item
          Caption = 'Role'
          Width = 90
        end>
      Items.Data = {
        690000000300000000000000FFFFFFFFFFFFFFFF000000000000000007415645
        5241474500000000FFFFFFFFFFFFFFFF00000000000000000D4D4F5354205641
        52494154454400000000FFFFFFFFFFFFFFFF00000000000000000E4E4F524D41
        4C495A454420415647}
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
  object GroupBox4: TGroupBox
    Left = 16
    Top = 196
    Width = 321
    Height = 93
    Anchors = [akLeft, akBottom]
    Caption = ' Best Patterns Frequency Summary '
    TabOrder = 4
    DesignSize = (
      321
      93)
    object ListViewBestFreqAll: TListView
      Left = 8
      Top = 15
      Width = 305
      Height = 70
      Anchors = [akLeft, akTop, akRight, akBottom]
      Columns = <
        item
          Caption = 'Position'
        end
        item
          Caption = 'Random'
          Width = 80
        end
        item
          Caption = 'Single'
          Width = 80
        end
        item
          Caption = 'Role'
          Width = 80
        end>
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
  object GroupBox5: TGroupBox
    Left = 355
    Top = 32
    Width = 321
    Height = 137
    Anchors = [akTop, akRight]
    Caption = ' Quality Notes Frequency '
    TabOrder = 5
    DesignSize = (
      321
      137)
    object ListViewNotesFreq: TListView
      Left = 8
      Top = 16
      Width = 305
      Height = 114
      Anchors = [akLeft, akTop, akRight, akBottom]
      Columns = <
        item
          Caption = 'Round'
        end
        item
          Caption = 'Random'
          Width = 80
        end
        item
          Caption = 'Single'
          Width = 80
        end
        item
          Caption = 'Role'
          Width = 80
        end>
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
  object ButtonAddExperiment: TButton
    Left = 296
    Top = 8
    Width = 105
    Height = 21
    Caption = 'Add Experiment'
    TabOrder = 6
    OnClick = ButtonAddExperimentClick
  end
  object OpenDialog1: TOpenDialog
    Left = 408
  end
end
