object FormTests: TFormTests
  Left = 236
  Top = 218
  Width = 677
  Height = 383
  Caption = 'CInBalada Internal Tests'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 669
    Height = 349
    ActivePage = TabSheet3
    Align = alClient
    TabIndex = 2
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Meter/Tempo'
      object Label4: TLabel
        Left = 144
        Top = 16
        Width = 42
        Height = 13
        Caption = 'MIDI File'
      end
      object Label5: TLabel
        Left = 272
        Top = 72
        Width = 107
        Height = 13
        Caption = 'TUBS Representation:'
      end
      object Label6: TLabel
        Left = 272
        Top = 176
        Width = 24
        Height = 13
        Caption = 'Bars:'
      end
      object Label7: TLabel
        Left = 272
        Top = 128
        Width = 49
        Height = 13
        Caption = 'Num Bars:'
      end
      object Label1: TLabel
        Left = 16
        Top = 128
        Width = 114
        Height = 13
        Caption = 'Preferred Tempo Range'
      end
      object Label2: TLabel
        Left = 16
        Top = 176
        Width = 68
        Height = 13
        Caption = 'Tempo Range'
      end
      object LabelMeter: TLabel
        Left = 16
        Top = 72
        Width = 27
        Height = 13
        Caption = 'Meter'
      end
      object LabelA: TLabel
        Left = 536
        Top = 248
        Width = 33
        Height = 13
        Caption = 'LabelA'
      end
      object LabelB: TLabel
        Left = 536
        Top = 264
        Width = 33
        Height = 13
        Caption = 'LabelB'
      end
      object LabelC: TLabel
        Left = 536
        Top = 280
        Width = 33
        Height = 13
        Caption = 'LabelC'
      end
      object ComboBoxFiles: TComboBox
        Left = 144
        Top = 32
        Width = 225
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ComboBoxFilesChange
      end
      object EditFullTubs: TEdit
        Left = 272
        Top = 88
        Width = 225
        Height = 21
        ReadOnly = True
        TabOrder = 1
      end
      object EditBars: TEdit
        Left = 272
        Top = 192
        Width = 225
        Height = 21
        ReadOnly = True
        TabOrder = 2
      end
      object EditNumBars: TEdit
        Left = 272
        Top = 144
        Width = 121
        Height = 21
        ReadOnly = True
        TabOrder = 3
      end
      object EditPrefTempo: TEdit
        Left = 16
        Top = 144
        Width = 225
        Height = 21
        ReadOnly = True
        TabOrder = 4
      end
      object EditTempo: TEdit
        Left = 16
        Top = 192
        Width = 225
        Height = 21
        ReadOnly = True
        TabOrder = 5
      end
      object EditMeter: TEdit
        Left = 16
        Top = 88
        Width = 225
        Height = 21
        ReadOnly = True
        TabOrder = 6
      end
      object ButtonSave: TButton
        Left = 136
        Top = 232
        Width = 99
        Height = 25
        Caption = 'Save Copy'
        TabOrder = 7
        OnClick = ButtonSaveClick
      end
      object EditFileName: TEdit
        Left = 256
        Top = 232
        Width = 121
        Height = 21
        TabOrder = 8
      end
      object Button1: TButton
        Left = 448
        Top = 264
        Width = 75
        Height = 25
        Caption = 'Button1'
        TabOrder = 9
        OnClick = Button1Click
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'PercAgent'
      ImageIndex = 1
      OnShow = TabSheet2Show
      object LabelPrintf: TLabel
        Left = 280
        Top = 184
        Width = 55
        Height = 13
        Caption = 'Scanf Test:'
      end
      object LabelString: TLabel
        Left = 280
        Top = 144
        Width = 30
        Height = 13
        Caption = 'String:'
      end
      object LabelRpMute: TLabel
        Left = 280
        Top = 96
        Width = 117
        Height = 13
        Caption = 'Mute Pattern Evaluation:'
      end
      object ListViewRoles: TListView
        Left = 40
        Top = 32
        Width = 177
        Height = 193
        Columns = <
          item
            Caption = 'Index'
          end
          item
            Caption = 'Role'
            Width = 100
          end>
        TabOrder = 0
        ViewStyle = vsReport
      end
      object ButtonPlusThree: TButton
        Left = 40
        Top = 248
        Width = 75
        Height = 25
        Caption = '+3'
        TabOrder = 1
        OnClick = ButtonPlusThreeClick
      end
      object ButtonMinusTwo: TButton
        Left = 144
        Top = 248
        Width = 75
        Height = 25
        Caption = '-2'
        TabOrder = 2
        OnClick = ButtonMinusTwoClick
      end
      object EditAddRole: TEdit
        Left = 40
        Top = 280
        Width = 73
        Height = 21
        TabOrder = 3
      end
      object EditPrintf: TEdit
        Left = 312
        Top = 200
        Width = 121
        Height = 21
        TabOrder = 4
      end
      object ButtonDoTest: TButton
        Left = 288
        Top = 232
        Width = 145
        Height = 21
        Caption = 'Test'
        TabOrder = 5
        OnClick = ButtonDoTestClick
      end
      object EditString: TEdit
        Left = 312
        Top = 160
        Width = 121
        Height = 21
        TabOrder = 6
        Text = 'bar=3'
      end
      object EditRpMute: TEdit
        Left = 312
        Top = 112
        Width = 121
        Height = 21
        ReadOnly = True
        TabOrder = 7
      end
      object ButtonCreateDir: TButton
        Left = 288
        Top = 24
        Width = 145
        Height = 25
        Caption = 'Create Directories'
        TabOrder = 8
        OnClick = ButtonCreateDirClick
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Statistics'
      ImageIndex = 2
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 661
        Height = 129
        Align = alTop
        Caption = ' Input Project '
        TabOrder = 0
        DesignSize = (
          661
          129)
        object LabelNumIterations: TLabel
          Left = 24
          Top = 56
          Width = 97
          Height = 21
          AutoSize = False
          Caption = 'Number of iterations:'
          Layout = tlCenter
        end
        object EditProjectName: TEdit
          Left = 8
          Top = 28
          Width = 593
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
        object ButtonChooseProject: TButton
          Left = 608
          Top = 28
          Width = 41
          Height = 21
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = ButtonChooseProjectClick
        end
        object EditNumIterations: TEdit
          Left = 136
          Top = 56
          Width = 57
          Height = 21
          BiDiMode = bdLeftToRight
          ParentBiDiMode = False
          TabOrder = 2
          Text = '1'
        end
        object CheckBoxRandomAgent: TCheckBox
          Left = 232
          Top = 56
          Width = 137
          Height = 21
          Alignment = taLeftJustify
          Caption = 'Use random evaluation:'
          TabOrder = 3
          OnClick = CheckBoxRandomAgentClick
        end
        object CheckBoxSingleFuncAgent: TCheckBox
          Left = 416
          Top = 56
          Width = 129
          Height = 21
          Alignment = taLeftJustify
          Caption = 'Use single evaluation:'
          Checked = True
          State = cbChecked
          TabOrder = 4
          OnClick = CheckBoxSingleFuncAgentClick
        end
        object RadioGroup1: TRadioGroup
          Left = 2
          Top = 87
          Width = 657
          Height = 40
          Align = alBottom
          Caption = ' Evaluation Type '
          Columns = 3
          Enabled = False
          Items.Strings = (
            'Random'
            'Single-Criteria'
            'Role-Based')
          TabOrder = 5
          OnClick = RadioGroup1Click
        end
      end
      object GroupBox2: TGroupBox
        Left = 0
        Top = 129
        Width = 661
        Height = 72
        Align = alTop
        Caption = ' Output File '
        TabOrder = 1
        DesignSize = (
          661
          72)
        object EditOutputFile: TEdit
          Left = 8
          Top = 28
          Width = 593
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
        object ButtonOutputFile: TButton
          Left = 608
          Top = 28
          Width = 41
          Height = 21
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = ButtonOutputFileClick
        end
      end
      object ButtonStartProcessing: TButton
        Left = 248
        Top = 208
        Width = 153
        Height = 25
        Caption = 'Start Processing'
        TabOrder = 2
        OnClick = ButtonStartProcessingClick
      end
      object GroupBox3: TGroupBox
        Left = 0
        Top = 256
        Width = 661
        Height = 65
        Align = alBottom
        Caption = ' Results '
        TabOrder = 3
        object LabelTotalTime: TLabel
          Left = 336
          Top = 24
          Width = 113
          Height = 21
          AutoSize = False
          Caption = 'Total processing time:'
          Layout = tlCenter
        end
        object LabelCurrIteration: TLabel
          Left = 64
          Top = 24
          Width = 81
          Height = 21
          AutoSize = False
          Caption = 'Current iteration:'
          Layout = tlCenter
        end
        object EditCurrIteration: TEdit
          Left = 176
          Top = 24
          Width = 65
          Height = 21
          BiDiMode = bdLeftToRight
          Color = clBtnFace
          ParentBiDiMode = False
          ReadOnly = True
          TabOrder = 0
          Text = '0'
        end
        object EditTotalTime: TEdit
          Left = 464
          Top = 24
          Width = 65
          Height = 21
          Color = clBtnFace
          ReadOnly = True
          TabOrder = 1
          Text = '0'
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'MIDI File'
      ImageIndex = 3
      object ButtonXmlTest: TButton
        Left = 24
        Top = 8
        Width = 153
        Height = 25
        Caption = 'Test XML Conversion'
        TabOrder = 0
        OnClick = ButtonXmlTestClick
      end
      object MemoXmlTest: TMemo
        Left = 24
        Top = 48
        Width = 313
        Height = 257
        TabOrder = 1
      end
    end
  end
  object OpenProjectDialog: TOpenDialog
    DefaultExt = 'cib'
    Filter = 'CInBalada Projects (*.cib)|*.cib'
    Left = 568
    Top = 304
  end
  object SaveOutputDialog: TSaveDialog
    DefaultExt = '.log'
    Filter = 'Log File (*.log)|*.log|Text File (*.txt)|*.txt|All files|*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 600
    Top = 304
  end
  object ApplicationEvents1: TApplicationEvents
    Left = 632
    Top = 304
  end
end
