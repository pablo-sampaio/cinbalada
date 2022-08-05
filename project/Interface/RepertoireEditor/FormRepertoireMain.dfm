object FormRepertoireMain: TFormRepertoireMain
  Left = 346
  Top = 185
  BorderStyle = bsSingle
  Caption = 'CInBalada Repertoire Editor'
  ClientHeight = 482
  ClientWidth = 543
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PanelMain: TPanel
    Left = 0
    Top = 0
    Width = 543
    Height = 482
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    ParentColor = True
    TabOrder = 0
    object PageControl: TPageControl
      Left = 5
      Top = 5
      Width = 533
      Height = 453
      ActivePage = TabSheetInstruments
      Align = alClient
      TabIndex = 2
      TabOrder = 0
      object TabSheetFiles: TTabSheet
        Caption = 'Files'
        OnShow = TabSheetFilesShow
        object ToolBarFiles: TToolBar
          Left = 0
          Top = 0
          Width = 525
          Height = 29
          Caption = 'ToolBarFiles'
          Images = ImageList1
          TabOrder = 0
          object ToolButtonAddFile: TToolButton
            Left = 0
            Top = 2
            Caption = 'Add file(s)'
            ImageIndex = 0
            OnClick = ToolButtonAddFileClick
          end
          object ToolButtonAddDir: TToolButton
            Left = 23
            Top = 2
            Caption = 'Add files from directory'
            ImageIndex = 1
            OnClick = ToolButtonAddDirClick
          end
          object ToolButtonSeparator: TToolButton
            Left = 46
            Top = 2
            Width = 8
            Caption = 'ToolButtonSeparator'
            ImageIndex = 2
            Style = tbsSeparator
          end
          object ToolButtonRemoveFile: TToolButton
            Left = 54
            Top = 2
            Caption = 'Remove file'
            ImageIndex = 2
            OnClick = ToolButtonRemoveFileClick
          end
        end
        object ListViewFiles: TListView
          Left = 0
          Top = 29
          Width = 525
          Height = 396
          Align = alClient
          Columns = <
            item
              Caption = 'ID'
              Width = 40
            end
            item
              Caption = 'MIDI File'
              Width = 200
            end
            item
              Caption = 'Instrument'
              Width = 160
            end
            item
              Caption = 'Type'
              Width = 100
            end>
          MultiSelect = True
          ReadOnly = True
          RowSelect = True
          TabOrder = 1
          ViewStyle = vsReport
          OnSelectItem = ListViewFilesSelectItem
        end
      end
      object TabSheetEditFile: TTabSheet
        Caption = 'Edit Pattern'
        OnShow = TabSheetEditFileShow
        object ListViewEditFiles: TListView
          Left = 0
          Top = 209
          Width = 525
          Height = 216
          Align = alClient
          Columns = <
            item
              Caption = 'ID'
              Width = 40
            end
            item
              Caption = 'MIDI File'
              Width = 150
            end
            item
              Caption = 'Name'
              Width = 120
            end
            item
              Caption = 'Instrument'
              Width = 120
            end
            item
              Caption = 'Type'
              Width = 70
            end>
          ReadOnly = True
          RowSelect = True
          TabOrder = 1
          ViewStyle = vsReport
          OnSelectItem = ListViewEditFilesSelectItem
        end
        object GroupBoxRPInfo: TGroupBox
          Left = 0
          Top = 0
          Width = 525
          Height = 209
          Align = alTop
          Caption = ' Rhythm Pattern Information '
          TabOrder = 0
          object LabelType: TLabel
            Left = 10
            Top = 142
            Width = 90
            Height = 21
            AutoSize = False
            Caption = 'Type:'
            Constraints.MinWidth = 90
            Layout = tlCenter
          end
          object LabelMeter: TLabel
            Left = 10
            Top = 52
            Width = 103
            Height = 21
            AutoSize = False
            Caption = 'Meter:'
            Constraints.MinWidth = 90
            Layout = tlCenter
          end
          object LabelInstrument: TLabel
            Left = 10
            Top = 112
            Width = 90
            Height = 21
            AutoSize = False
            Caption = 'Instrument:'
            Constraints.MinWidth = 90
            Layout = tlCenter
          end
          object Bevel1: TBevel
            Left = 260
            Top = 35
            Width = 2
            Height = 110
            Constraints.MaxWidth = 2
            Constraints.MinWidth = 2
          end
          object LabelMusicalStyles: TLabel
            Left = 272
            Top = 52
            Width = 73
            Height = 21
            AutoSize = False
            Caption = 'Musical Styles:'
            Layout = tlCenter
          end
          object Label1: TLabel
            Left = 10
            Top = 22
            Width = 90
            Height = 21
            AutoSize = False
            Caption = 'MIDI File:'
            Constraints.MinWidth = 90
            Layout = tlCenter
          end
          object LabelNumTrack: TLabel
            Left = 10
            Top = 82
            Width = 90
            Height = 21
            AutoSize = False
            Caption = 'Track #:'
            Constraints.MinWidth = 90
            Layout = tlCenter
          end
          object LabelName: TLabel
            Left = 272
            Top = 22
            Width = 90
            Height = 21
            AutoSize = False
            Caption = 'Pattern Name:'
            Constraints.MinWidth = 90
            Layout = tlCenter
          end
          object ComboBoxType: TComboBox
            Left = 88
            Top = 142
            Width = 153
            Height = 21
            Style = csDropDownList
            ItemHeight = 0
            TabOrder = 3
            OnChange = ComboBoxTypeChange
          end
          object ComboBoxInstrument: TComboBox
            Left = 88
            Top = 112
            Width = 153
            Height = 21
            CharCase = ecLowerCase
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemHeight = 0
            ParentFont = False
            TabOrder = 2
            OnChange = ComboBoxInstrumentChange
          end
          object ButtonSave: TBitBtn
            Left = 224
            Top = 176
            Width = 75
            Height = 25
            Caption = 'Save'
            TabOrder = 8
            OnClick = ButtonSaveClick
            Kind = bkOK
          end
          object CheckListBoxStyles: TCheckListBox
            Left = 296
            Top = 74
            Width = 209
            Height = 42
            ItemHeight = 20
            Style = lbOwnerDrawVariable
            TabOrder = 4
          end
          object ButtonAddStyle: TButton
            Left = 296
            Top = 120
            Width = 50
            Height = 21
            Hint = 'Add Musical Style'
            Caption = 'Add'
            Constraints.MaxHeight = 21
            Constraints.MaxWidth = 50
            TabOrder = 6
            OnClick = ButtonAddStyleClick
          end
          object EditAddStyle: TEdit
            Left = 355
            Top = 120
            Width = 150
            Height = 21
            TabOrder = 5
          end
          object ButtonRemoveStyle: TButton
            Left = 296
            Top = 144
            Width = 209
            Height = 21
            Hint = 'Remove Selected Styles'
            Caption = 'Remove Styles'
            Constraints.MaxHeight = 21
            Constraints.MinWidth = 140
            TabOrder = 7
            OnClick = ButtonRemoveStyleClick
          end
          object EditMidiFile: TEdit
            Left = 88
            Top = 22
            Width = 153
            Height = 21
            Color = clBtnFace
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ReadOnly = True
            TabOrder = 0
            OnChange = EditNamexChange
          end
          object EditNumTrack: TEdit
            Left = 88
            Top = 82
            Width = 153
            Height = 21
            TabOrder = 1
            OnChange = EditNumTrackChange
          end
          object EditName: TEdit
            Left = 352
            Top = 22
            Width = 153
            Height = 21
            TabOrder = 9
            OnChange = EditNamexChange
          end
          object ComboBoxMeter: TComboBox
            Left = 88
            Top = 52
            Width = 153
            Height = 21
            Style = csDropDownList
            CharCase = ecLowerCase
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemHeight = 0
            ParentFont = False
            TabOrder = 10
            OnChange = ComboBoxMeterChange
          end
        end
      end
      object TabSheetInstruments: TTabSheet
        Caption = 'Instruments'
        ImageIndex = 4
        OnShow = TabSheetInstrumentsShow
        object GroupBoxInstrumentInfo: TGroupBox
          Left = 0
          Top = 0
          Width = 525
          Height = 241
          Align = alTop
          Caption = ' Instrument Info '
          TabOrder = 0
          object LabelInstrName: TLabel
            Left = 8
            Top = 24
            Width = 83
            Height = 13
            Caption = 'Instrument Name:'
            Layout = tlCenter
          end
          object LabelFreqClass: TLabel
            Left = 272
            Top = 24
            Width = 81
            Height = 13
            Caption = 'Frequency Class:'
            Layout = tlCenter
          end
          object LabelInstrNotes: TLabel
            Left = 8
            Top = 72
            Width = 31
            Height = 13
            Caption = 'Notes:'
            Layout = tlCenter
          end
          object Bevel2: TBevel
            Left = 80
            Top = 68
            Width = 361
            Height = 2
          end
          object Bevel3: TBevel
            Left = 80
            Top = 192
            Width = 361
            Height = 2
          end
          object LabelMdNote: TLabel
            Left = 288
            Top = 110
            Width = 57
            Height = 21
            AutoSize = False
            Caption = 'MIDI Note:'
            Layout = tlCenter
          end
          object LabelRelPitch: TLabel
            Left = 288
            Top = 133
            Width = 73
            Height = 21
            AutoSize = False
            Caption = 'Relative Pitch:'
            Layout = tlCenter
          end
          object EditInstrName: TEdit
            Left = 40
            Top = 40
            Width = 209
            Height = 21
            CharCase = ecLowerCase
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 0
            OnChange = EditInstrNameChange
          end
          object ComboBoxFreqClass: TComboBox
            Left = 304
            Top = 40
            Width = 201
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 1
            OnChange = ComboBoxFreqClassChange
          end
          object ComboBoxMidiNote: TComboBox
            Left = 368
            Top = 110
            Width = 137
            Height = 21
            Style = csDropDownList
            Enabled = False
            ItemHeight = 13
            TabOrder = 2
          end
          object ButtonAddInstrNote: TButton
            Left = 264
            Top = 88
            Width = 73
            Height = 21
            Caption = 'Add'
            Enabled = False
            TabOrder = 4
            OnClick = ButtonAddInstrNoteClick
          end
          object ButtonRemoveInstrNote: TButton
            Left = 264
            Top = 163
            Width = 73
            Height = 21
            Caption = 'Remove'
            Enabled = False
            TabOrder = 6
            OnClick = ButtonRemoveInstrNoteClick
          end
          object ButtonNewInstrSave: TButton
            Left = 144
            Top = 203
            Width = 105
            Height = 25
            Caption = 'New / Save'
            TabOrder = 7
            OnClick = ButtonNewInstrSaveClick
          end
          object ListViewInstrNotes: TListView
            Left = 40
            Top = 88
            Width = 209
            Height = 97
            Columns = <
              item
                Caption = 'MIDI Note'
                Width = 100
              end
              item
                Caption = 'Rel. Pitch'
                Width = 105
              end>
            RowSelect = True
            TabOrder = 5
            ViewStyle = vsReport
            OnSelectItem = ListViewInstrNotesSelectItem
          end
          object ButtonDeleteInstr: TButton
            Left = 272
            Top = 203
            Width = 105
            Height = 25
            Caption = 'Delete'
            TabOrder = 8
            OnClick = ButtonDeleteInstrClick
          end
          object ComboBoxNoteRelPitch: TComboBox
            Left = 368
            Top = 133
            Width = 137
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 3
          end
        end
        object ListViewInstruments: TListView
          Left = 0
          Top = 241
          Width = 525
          Height = 184
          Align = alClient
          Columns = <
            item
              Caption = 'Instrument Name'
              Width = 180
            end
            item
              Caption = 'Frequency Class'
              Width = 170
            end
            item
              Caption = 'Notes'
              Width = 70
            end
            item
              Caption = 'Patterns'
              Width = 80
            end>
          ReadOnly = True
          RowSelect = True
          TabOrder = 1
          ViewStyle = vsReport
          OnSelectItem = ListViewInstrumentsSelectItem
        end
      end
      object TabSheetMidiInfo: TTabSheet
        Caption = 'MIDI Info'
        ImageIndex = 4
        OnHide = TabSheetMidiInfoHide
        OnShow = TabSheetMidiInfoShow
        object GroupBoxMiChEvents: TGroupBox
          Left = 0
          Top = 129
          Width = 525
          Height = 296
          Align = alClient
          Caption = ' MIDI Events '
          TabOrder = 2
          DesignSize = (
            525
            296)
          object ListViewMiChEvents: TListView
            Left = 8
            Top = 16
            Width = 505
            Height = 274
            Anchors = [akLeft, akTop, akRight, akBottom]
            Columns = <
              item
                Caption = 'Track'
                Width = 45
              end
              item
                Caption = 'Tick'
                Width = 60
              end
              item
                Caption = 'Type'
                Width = 190
              end
              item
                Caption = 'Event Data'
                Width = 185
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
          end
        end
        object GroupBoxMiGeneralInfo: TGroupBox
          Left = 0
          Top = 33
          Width = 525
          Height = 96
          Align = alTop
          Caption = ' General Information '
          TabOrder = 1
          object LabelMiTracks: TLabel
            Left = 24
            Top = 16
            Width = 89
            Height = 21
            AutoSize = False
            Caption = 'Tracks:'
            Layout = tlCenter
          end
          object LabelMiTempo: TLabel
            Left = 24
            Top = 64
            Width = 89
            Height = 21
            AutoSize = False
            Caption = 'Tempo:'
            Layout = tlCenter
          end
          object LabelMiResolution: TLabel
            Left = 24
            Top = 40
            Width = 89
            Height = 21
            AutoSize = False
            Caption = 'Resolution:'
            Layout = tlCenter
          end
          object Bevel4: TBevel
            Left = 260
            Top = 24
            Width = 2
            Height = 49
            Constraints.MaxWidth = 2
            Constraints.MinWidth = 2
          end
          object LabelMiMeter: TLabel
            Left = 288
            Top = 16
            Width = 65
            Height = 21
            AutoSize = False
            Caption = 'Meter:'
            Layout = tlCenter
          end
          object LabelMiMaxTick: TLabel
            Left = 288
            Top = 40
            Width = 65
            Height = 21
            AutoSize = False
            Caption = 'Max Tick:'
            Layout = tlCenter
          end
          object LabelMiBars: TLabel
            Left = 288
            Top = 64
            Width = 65
            Height = 21
            AutoSize = False
            Caption = 'Bars:'
            Layout = tlCenter
          end
          object EditMiTempo: TEdit
            Left = 120
            Top = 64
            Width = 73
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 2
          end
          object EditMiTracks: TEdit
            Left = 120
            Top = 16
            Width = 73
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 0
          end
          object EditMiResolution: TEdit
            Left = 120
            Top = 40
            Width = 73
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 1
          end
          object EditMiMeter: TEdit
            Left = 376
            Top = 16
            Width = 73
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 3
          end
          object EditMiMaxTick: TEdit
            Left = 376
            Top = 40
            Width = 73
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 4
          end
          object EditMiBars: TEdit
            Left = 376
            Top = 64
            Width = 73
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 5
          end
        end
        object PanelMiSuperiorPanel: TPanel
          Left = 0
          Top = 0
          Width = 525
          Height = 33
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object LabelMiFile: TLabel
            Left = 88
            Top = 6
            Width = 89
            Height = 21
            AutoSize = False
            Caption = 'Pattern MIDI File:'
            Layout = tlCenter
          end
          object ComboBoxMiFile: TComboBox
            Left = 192
            Top = 6
            Width = 217
            Height = 21
            Style = csDropDownList
            ItemHeight = 0
            TabOrder = 0
            OnChange = ComboBoxMiFileChange
          end
        end
      end
      object TabSheetComparator: TTabSheet
        Caption = 'Comparator'
        ImageIndex = 3
        OnHide = TabSheetComparatorHide
        OnShow = TabSheetComparatorShow
        object GroupBoxPatterns: TGroupBox
          Left = 0
          Top = 0
          Width = 525
          Height = 89
          Align = alTop
          Caption = ' Rhythm Patterns '
          TabOrder = 0
          object PanelPatternComp2: TPanel
            Left = 2
            Top = 51
            Width = 519
            Height = 22
            BevelOuter = bvNone
            TabOrder = 1
            object LabelPatternComp2: TLabel
              Left = 0
              Top = 0
              Width = 90
              Height = 22
              Align = alLeft
              Alignment = taCenter
              Caption = 'Pattern 2 :'
              Constraints.MinWidth = 90
              Layout = tlCenter
            end
            object ComboBoxPatternComp2: TComboBox
              Left = 112
              Top = 1
              Width = 345
              Height = 21
              Style = csDropDownList
              ItemHeight = 13
              TabOrder = 0
              OnChange = ComboBoxPatternComp2Change
            end
            object BitBtnPlayComp2: TBitBtn
              Left = 469
              Top = 2
              Width = 15
              Height = 15
              TabOrder = 1
              OnClick = BitBtnPlayComp2Click
              Glyph.Data = {
                A2040000424DA204000000000000360400002800000009000000090000000100
                0800000000006C00000000000000000000000001000000000000000000000000
                80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
                A6000020400000206000002080000020A0000020C0000020E000004000000040
                20000040400000406000004080000040A0000040C0000040E000006000000060
                20000060400000606000006080000060A0000060C0000060E000008000000080
                20000080400000806000008080000080A0000080C0000080E00000A0000000A0
                200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
                200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
                200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
                20004000400040006000400080004000A0004000C0004000E000402000004020
                20004020400040206000402080004020A0004020C0004020E000404000004040
                20004040400040406000404080004040A0004040C0004040E000406000004060
                20004060400040606000406080004060A0004060C0004060E000408000004080
                20004080400040806000408080004080A0004080C0004080E00040A0000040A0
                200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
                200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
                200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
                20008000400080006000800080008000A0008000C0008000E000802000008020
                20008020400080206000802080008020A0008020C0008020E000804000008040
                20008040400080406000804080008040A0008040C0008040E000806000008060
                20008060400080606000806080008060A0008060C0008060E000808000008080
                20008080400080806000808080008080A0008080C0008080E00080A0000080A0
                200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
                200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
                200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
                2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
                2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
                2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
                2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
                2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
                2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
                2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F6F6F6F6F6F6
                F6F6F6000000F6F6F600F6F6F6F6F6000000F6F6F60000F6F6F6F6000000F6F6
                F6000000F6F6F6000000F6F6F600000000F6F6000000F6F6F6000000F6F6F600
                0000F6F6F60000F6F6F6F6000000F6F6F600F6F6F6F6F6000000F6F6F6F6F6F6
                F6F6F6000000}
              Style = bsNew
            end
            object BitBtnPauseComp2: TBitBtn
              Left = 485
              Top = 2
              Width = 15
              Height = 15
              TabOrder = 2
              OnClick = BitBtnPauseComp2Click
              Glyph.Data = {
                A2040000424DA204000000000000360400002800000009000000090000000100
                0800000000006C00000000000000000000000001000000000000000000000000
                80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
                A6000020400000206000002080000020A0000020C0000020E000004000000040
                20000040400000406000004080000040A0000040C0000040E000006000000060
                20000060400000606000006080000060A0000060C0000060E000008000000080
                20000080400000806000008080000080A0000080C0000080E00000A0000000A0
                200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
                200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
                200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
                20004000400040006000400080004000A0004000C0004000E000402000004020
                20004020400040206000402080004020A0004020C0004020E000404000004040
                20004040400040406000404080004040A0004040C0004040E000406000004060
                20004060400040606000406080004060A0004060C0004060E000408000004080
                20004080400040806000408080004080A0004080C0004080E00040A0000040A0
                200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
                200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
                200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
                20008000400080006000800080008000A0008000C0008000E000802000008020
                20008020400080206000802080008020A0008020C0008020E000804000008040
                20008040400080406000804080008040A0008040C0008040E000806000008060
                20008060400080606000806080008060A0008060C0008060E000808000008080
                20008080400080806000808080008080A0008080C0008080E00080A0000080A0
                200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
                200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
                200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
                2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
                2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
                2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
                2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
                2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
                2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
                2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F6F6F6F6F6F6
                F6F6F6000000F6F6F6F6F6F6F6F6F6000000F6F60000F60000F6F6000000F6F6
                0000F60000F6F6000000F6F60000F60000F6F6000000F6F60000F60000F6F600
                0000F6F60000F60000F6F6000000F6F6F6F6F6F6F6F6F6000000F6F6F6F6F6F6
                F6F6F6000000}
              Style = bsNew
            end
          end
          object PanelPatternComp1: TPanel
            Left = 2
            Top = 21
            Width = 519
            Height = 22
            BevelOuter = bvNone
            TabOrder = 0
            object LabelPatternComp1: TLabel
              Left = 0
              Top = 0
              Width = 90
              Height = 22
              Align = alLeft
              Alignment = taCenter
              Caption = 'Pattern 1 :'
              Constraints.MinWidth = 90
              Layout = tlCenter
            end
            object ComboBoxPatternComp1: TComboBox
              Left = 112
              Top = 1
              Width = 345
              Height = 21
              Style = csDropDownList
              ItemHeight = 13
              TabOrder = 0
              OnChange = ComboBoxPatternComp1Change
            end
            object BitBtnPlayComp1: TBitBtn
              Left = 469
              Top = 2
              Width = 15
              Height = 15
              TabOrder = 1
              OnClick = BitBtnPlayComp1Click
              Glyph.Data = {
                A2040000424DA204000000000000360400002800000009000000090000000100
                0800000000006C00000000000000000000000001000000000000000000000000
                80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
                A6000020400000206000002080000020A0000020C0000020E000004000000040
                20000040400000406000004080000040A0000040C0000040E000006000000060
                20000060400000606000006080000060A0000060C0000060E000008000000080
                20000080400000806000008080000080A0000080C0000080E00000A0000000A0
                200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
                200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
                200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
                20004000400040006000400080004000A0004000C0004000E000402000004020
                20004020400040206000402080004020A0004020C0004020E000404000004040
                20004040400040406000404080004040A0004040C0004040E000406000004060
                20004060400040606000406080004060A0004060C0004060E000408000004080
                20004080400040806000408080004080A0004080C0004080E00040A0000040A0
                200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
                200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
                200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
                20008000400080006000800080008000A0008000C0008000E000802000008020
                20008020400080206000802080008020A0008020C0008020E000804000008040
                20008040400080406000804080008040A0008040C0008040E000806000008060
                20008060400080606000806080008060A0008060C0008060E000808000008080
                20008080400080806000808080008080A0008080C0008080E00080A0000080A0
                200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
                200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
                200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
                2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
                2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
                2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
                2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
                2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
                2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
                2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F6F6F6F6F6F6
                F6F6F6000000F6F6F600F6F6F6F6F6000000F6F6F60000F6F6F6F6000000F6F6
                F6000000F6F6F6000000F6F6F600000000F6F6000000F6F6F6000000F6F6F600
                0000F6F6F60000F6F6F6F6000000F6F6F600F6F6F6F6F6000000F6F6F6F6F6F6
                F6F6F6000000}
              Style = bsNew
            end
            object BitBtnPauseComp1: TBitBtn
              Left = 485
              Top = 2
              Width = 15
              Height = 15
              TabOrder = 2
              OnClick = BitBtnPauseComp1Click
              Glyph.Data = {
                A2040000424DA204000000000000360400002800000009000000090000000100
                0800000000006C00000000000000000000000001000000000000000000000000
                80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
                A6000020400000206000002080000020A0000020C0000020E000004000000040
                20000040400000406000004080000040A0000040C0000040E000006000000060
                20000060400000606000006080000060A0000060C0000060E000008000000080
                20000080400000806000008080000080A0000080C0000080E00000A0000000A0
                200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
                200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
                200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
                20004000400040006000400080004000A0004000C0004000E000402000004020
                20004020400040206000402080004020A0004020C0004020E000404000004040
                20004040400040406000404080004040A0004040C0004040E000406000004060
                20004060400040606000406080004060A0004060C0004060E000408000004080
                20004080400040806000408080004080A0004080C0004080E00040A0000040A0
                200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
                200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
                200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
                20008000400080006000800080008000A0008000C0008000E000802000008020
                20008020400080206000802080008020A0008020C0008020E000804000008040
                20008040400080406000804080008040A0008040C0008040E000806000008060
                20008060400080606000806080008060A0008060C0008060E000808000008080
                20008080400080806000808080008080A0008080C0008080E00080A0000080A0
                200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
                200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
                200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
                2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
                2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
                2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
                2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
                2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
                2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
                2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F6F6F6F6F6F6
                F6F6F6000000F6F6F6F6F6F6F6F6F6000000F6F60000F60000F6F6000000F6F6
                0000F60000F6F6000000F6F60000F60000F6F6000000F6F60000F60000F6F600
                0000F6F60000F60000F6F6000000F6F6F6F6F6F6F6F6F6000000F6F6F6F6F6F6
                F6F6F6000000}
              Style = bsNew
            end
          end
        end
        object GroupBoxVisualization: TGroupBox
          Left = 0
          Top = 149
          Width = 525
          Height = 276
          Align = alClient
          Caption = ' Visualization '
          TabOrder = 2
          object RhythmPatternControl: TRhythmPatternControl
            Left = 2
            Top = 37
            Width = 521
            Height = 237
            HBorder = 8
            VBorder = 8
            Align = alClient
          end
          object PanelVisualizationTp: TPanel
            Left = 2
            Top = 15
            Width = 521
            Height = 22
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 0
            object LabelGraphRepr: TLabel
              Left = 22
              Top = 0
              Width = 83
              Height = 22
              AutoSize = False
              Caption = 'Representation :'
              Layout = tlCenter
            end
            object LabelGraphAlignment: TLabel
              Left = 260
              Top = 0
              Width = 63
              Height = 22
              AutoSize = False
              Caption = 'Alignment :'
              Layout = tlCenter
            end
            object ComboBoxGraphRepr: TComboBox
              Left = 104
              Top = 1
              Width = 119
              Height = 21
              Style = csDropDownList
              ItemHeight = 13
              TabOrder = 0
              OnChange = ComboBoxGraphReprChange
            end
            object ComboBoxGraphAlignment: TComboBox
              Left = 320
              Top = 0
              Width = 177
              Height = 21
              Style = csDropDownList
              ItemHeight = 13
              TabOrder = 1
              OnChange = ComboBoxGraphAlignmentChange
            end
          end
        end
        object GroupBoxAvaliation: TGroupBox
          Left = 0
          Top = 89
          Width = 525
          Height = 60
          Align = alTop
          Caption = ' Avaliation '
          TabOrder = 1
          object LabelAvCriteria: TLabel
            Left = 23
            Top = 21
            Width = 47
            Height = 21
            AutoSize = False
            Caption = 'Criteria :'
            Layout = tlCenter
          end
          object ComboBoxAvCriteria: TComboBox
            Left = 80
            Top = 21
            Width = 193
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 0
            OnChange = ComboBoxAvCriteriaChange
          end
          object EditAvaliationResult: TEdit
            Left = 297
            Top = 21
            Width = 201
            Height = 21
            Color = clBtnFace
            ReadOnly = True
            TabOrder = 1
          end
        end
      end
    end
    object StatusBar: TStatusBar
      Left = 5
      Top = 458
      Width = 533
      Height = 19
      Panels = <
        item
          Alignment = taCenter
          Text = '0 instruments'
          Width = 140
        end
        item
          Alignment = taCenter
          Text = '0 rhythm patterns'
          Width = 160
        end>
      SimplePanel = False
    end
  end
  object OpenDialogFiles: TOpenDialog
    Filter = 'MIDI Files|*.mid|All Files|*'
    InitialDir = '.'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Add MIDI File(s)'
    Left = 409
    Top = 65535
  end
  object ImageList1: TImageList
    Left = 368
    Bitmap = {
      494C010105000900040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000003000000001002000000000000030
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000808080008080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000080808000555F5500555F550080808000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF00000080000000
      80008080800000000000000000000000000000000000000000000000FF008080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000555F550000800000557F0000555F5500000000000000
      0000000000000000000000000000000000000000000000000000008080000080
      8000008080000080800000808000008080000080800000808000008080000000
      000000000000000000000000000000000000000000000000FF00000080000000
      800000008000808080000000000000000000000000000000FF00000080000000
      8000808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000555F550080800000557F0000005F7F00000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000080
      8000008080000080800000808000008080000080800000808000008080000080
      800000000000000000000000000000000000000000000000FF00000080000000
      8000000080000000800080808000000000000000FF0000008000000080000000
      8000000080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000555F550055BF000080800000557F5500000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000008080000080800000808000008080000080800000808000008080000080
      80000080800000000000000000000000000000000000000000000000FF000000
      8000000080000000800000008000808080000000800000008000000080000000
      8000000080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000080808000555F
      5500555F5500005F7F0080808000AABF0000559F000080808000555F5500555F
      5500555F55008080800000000000000000000000000000FFFF00FFFFFF0000FF
      FF00000000000080800000808000008080000080800000808000008080000080
      8000008080000080800000000000000000000000000000000000000000000000
      FF00000080000000800000008000000080000000800000008000000080000000
      8000808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555F550055DF
      0000AADF000055DF0000AADF000055BF0000AABF00008080000055BF0000557F
      0000007F0000555F7F00000000000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000800000008000000080000000800000008000000080008080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555F550055DF
      2A00AADF000055DF0000AADF000055DF0000AABF000055DF000080800000AA9F
      0000557F0000555F550000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000800000008000000080000000800000008000808080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000080808000555F
      5500555F5500555F55008080800055DF0000AADF000080808000555F7F00005F
      5500555F7F00557F7F00000000000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF0000008000000080000000800000008000808080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000555F550055DF0000AADF0000555F5500000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000800000008000000080000000800000008000808080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000555F550055DF000055DF0000555F7F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF00000080000000800000008000808080000000800000008000000080008080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000005F7F00AADF2A00AADF0000555F5500000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      8000000080000000800080808000000000000000FF0000008000000080000000
      8000808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000080808000555F5500005F550080808000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      800000008000808080000000000000000000000000000000FF00000080000000
      8000000080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000800000000000000000000000000000000000000000000000FF000000
      8000000080000000800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF00000080000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000300000000100010000000000800100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF000000000000FFFF000000000000FFFF000000000000
      F93F000000000000F93F000000000000F93F000000000000F93F000000000000
      F93F000000000000FFFF000000000000FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFCFFFFFFF
      FC3F001F87CFFFFFFC3F000F8387FFFFFC3F00078103FFFFFC3F0003C003FDFF
      C0030001E007FCFFC0030000F00FFC7FC003001FF81FFC3FC003001FF81FFC7F
      FC3F001FF01FFCFFFC3F8FFFE00FFDFFFC3FFFF1C107FFFFFC3FFFF9C383FFFF
      FFFFFF75E7C3FFFFFFFFFF8FFFE3FFFF00000000000000000000000000000000
      000000000000}
  end
end
