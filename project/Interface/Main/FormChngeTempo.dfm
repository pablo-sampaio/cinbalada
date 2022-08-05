object FormChangeTempo: TFormChangeTempo
  Left = 481
  Top = 344
  BorderStyle = bsToolWindow
  Caption = 'Mudar Andamento'
  ClientHeight = 125
  ClientWidth = 256
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poOwnerFormCenter
  OnShow = FormShow
  DesignSize = (
    256
    125)
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 240
    Height = 82
    Anchors = [akLeft, akTop, akRight, akBottom]
    Shape = bsFrame
  end
  object OKBtn: TButton
    Left = 55
    Top = 99
    Width = 75
    Height = 20
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 142
    Top = 99
    Width = 75
    Height = 20
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancelar'
    ModalResult = 2
    TabOrder = 2
  end
  object TrackBarTempo: TTrackBar
    Left = 32
    Top = 55
    Width = 190
    Height = 21
    Hint = 'Arraste o ponteiro para o andamento desejado'
    Anchors = [akLeft, akBottom]
    Max = 180
    Min = 50
    Orientation = trHorizontal
    ParentShowHint = False
    Frequency = 3
    Position = 100
    SelEnd = 0
    SelStart = 0
    ShowHint = True
    TabOrder = 0
    ThumbLength = 14
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = TrackBarTempoChange
  end
  object ComboBoxTempo: TComboBox
    Left = 38
    Top = 24
    Width = 180
    Height = 21
    Hint = 'Escolha uma categoria de andamento ou digite um valor em BPM'
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    OnChange = ComboBoxTempoChange
    OnExit = ComboBoxTempoExit
  end
end
