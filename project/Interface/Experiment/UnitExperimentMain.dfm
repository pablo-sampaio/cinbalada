object FormExperimentMain: TFormExperimentMain
  Left = 281
  Top = 280
  BorderStyle = bsDialog
  Caption = 'CInBalada Experiment Results'
  ClientHeight = 402
  ClientWidth = 701
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 701
    Height = 402
    ActivePage = TabSheet3
    Align = alClient
    TabIndex = 2
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Experiment Inputs'
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 337
        Height = 374
        Align = alLeft
        Caption = ' Primeiro Experimento '
        TabOrder = 0
        object Label1: TLabel
          Left = 32
          Top = 96
          Width = 51
          Height = 21
          AutoSize = False
          Caption = 'Amostra A:'
          Layout = tlCenter
        end
        object Label2: TLabel
          Left = 32
          Top = 128
          Width = 51
          Height = 21
          AutoSize = False
          Caption = 'Amostra B:'
          Layout = tlCenter
        end
        object Label3: TLabel
          Left = 32
          Top = 160
          Width = 51
          Height = 21
          AutoSize = False
          Caption = 'Amostra C:'
          Layout = tlCenter
        end
        object Label4: TLabel
          Left = 32
          Top = 216
          Width = 51
          Height = 21
          AutoSize = False
          Caption = 'Melhor:'
          Layout = tlCenter
        end
        object Label5: TLabel
          Left = 32
          Top = 248
          Width = 51
          Height = 21
          AutoSize = False
          Caption = 'Pior:'
          Layout = tlCenter
        end
        object Label6: TLabel
          Left = 32
          Top = 48
          Width = 51
          Height = 21
          AutoSize = False
          Caption = 'Rodada:'
          Layout = tlCenter
        end
        object ComboBox1: TComboBox
          Left = 104
          Top = 96
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          Items.Strings = (
            'p'#233'ssimo'
            'ruim'
            'bom'
            'excelente')
        end
        object ComboBox2: TComboBox
          Left = 104
          Top = 128
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 1
          Items.Strings = (
            'p'#233'ssimo'
            'ruim'
            'bom'
            'excelente')
        end
        object ComboBox3: TComboBox
          Left = 104
          Top = 160
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 2
          Items.Strings = (
            'p'#233'ssimo'
            'ruim'
            'bom'
            'excelente')
        end
        object ComboBox4: TComboBox
          Left = 104
          Top = 216
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 3
          Items.Strings = (
            'A'
            'B'
            'C')
        end
        object ComboBox5: TComboBox
          Left = 104
          Top = 248
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 4
          Items.Strings = (
            'A'
            'B'
            'C')
        end
        object ButtonSave1: TButton
          Left = 128
          Top = 296
          Width = 75
          Height = 25
          Caption = 'Save'
          TabOrder = 5
        end
        object EditRodada: TEdit
          Left = 104
          Top = 48
          Width = 129
          Height = 21
          TabOrder = 6
        end
      end
      object GroupBox2: TGroupBox
        Left = 337
        Top = 0
        Width = 356
        Height = 374
        Align = alClient
        Caption = ' Segundo Experimento '
        TabOrder = 1
        object Label7: TLabel
          Left = 32
          Top = 48
          Width = 145
          Height = 21
          AutoSize = False
          Caption = 'Diversidade do Conjunto 1:'
          Layout = tlCenter
        end
        object Label8: TLabel
          Left = 32
          Top = 80
          Width = 145
          Height = 21
          AutoSize = False
          Caption = 'Diversidade do Conjunto 2:'
          Layout = tlCenter
        end
        object Label9: TLabel
          Left = 32
          Top = 128
          Width = 145
          Height = 21
          AutoSize = False
          Caption = 'Conjunto Mais Diverso:'
          Layout = tlCenter
        end
        object ComboBox6: TComboBox
          Left = 200
          Top = 48
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10')
        end
        object ComboBox7: TComboBox
          Left = 200
          Top = 80
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 1
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10')
        end
        object ComboBox8: TComboBox
          Left = 200
          Top = 128
          Width = 129
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 2
          Items.Strings = (
            'Conjunto 1'
            'Conjunto 2')
        end
        object ButtonSave2: TButton
          Left = 160
          Top = 288
          Width = 75
          Height = 25
          Caption = 'Save'
          TabOrder = 3
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Experiments Overview'
      ImageIndex = 1
      object GroupBox3: TGroupBox
        Left = 168
        Top = 80
        Width = 361
        Height = 249
        Caption = ' Experiment '
        TabOrder = 0
        inline FrameExperiment1: TFrameExperiment
          Left = 2
          Top = 15
          Width = 357
          Height = 232
          Align = alClient
          TabOrder = 0
          inherited ListViewFirst: TListView
            Width = 357
            Height = 128
          end
          inherited ListViewSecond: TListView
            Top = 128
            Width = 357
          end
        end
      end
      object ButtonOpenExpFile: TButton
        Left = 272
        Top = 32
        Width = 137
        Height = 25
        Caption = 'Open Experiment File'
        TabOrder = 1
        OnClick = ButtonOpenExpFileClick
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Experiments Results'
      ImageIndex = 2
      inline FrameExperimentResult1: TFrameExperimentResult
        Left = 0
        Top = 0
        Width = 693
        Height = 374
        Align = alClient
        TabOrder = 0
        inherited GroupBox2: TGroupBox
          Left = 358
          Top = 173
        end
        inherited GroupBox3: TGroupBox
          Left = 358
          Top = 281
        end
        inherited GroupBox4: TGroupBox
          Top = 197
        end
        inherited GroupBox5: TGroupBox
          Left = 358
        end
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 672
  end
end
