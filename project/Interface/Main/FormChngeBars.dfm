object FormChangeBars: TFormChangeBars
  Left = 432
  Top = 288
  BorderStyle = bsDialog
  Caption = 'Mudar Compassos'
  ClientHeight = 92
  ClientWidth = 249
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poOwnerFormCenter
  OnShow = FormShow
  DesignSize = (
    249
    92)
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 233
    Height = 49
    Anchors = [akLeft, akTop, akRight, akBottom]
    Shape = bsFrame
  end
  object OKBtn: TButton
    Left = 47
    Top = 65
    Width = 75
    Height = 20
    Anchors = [akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 136
    Top = 65
    Width = 75
    Height = 20
    Anchors = [akBottom]
    Cancel = True
    Caption = 'Cancelar'
    ModalResult = 2
    TabOrder = 1
  end
  object ComboBoxBars: TComboBox
    Left = 24
    Top = 24
    Width = 201
    Height = 21
    ItemHeight = 13
    TabOrder = 2
    OnChange = ComboBoxBarsChange
    Items.Strings = (
      '8'
      '16'
      '24'
      '32')
  end
end
