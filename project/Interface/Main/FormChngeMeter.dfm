object FormChangeMeter: TFormChangeMeter
  Left = 363
  Top = 298
  BorderStyle = bsToolWindow
  Caption = 'Mudar M'#233'trica'
  ClientHeight = 90
  ClientWidth = 249
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poOwnerFormCenter
  OnShow = FormShow
  DesignSize = (
    249
    90)
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 233
    Height = 46
    Anchors = [akLeft, akTop, akRight, akBottom]
    Shape = bsFrame
  end
  object LabelMeter: TLabel
    Left = 24
    Top = 22
    Width = 35
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'M'#233'trica'
  end
  object OKBtn: TButton
    Left = 47
    Top = 63
    Width = 75
    Height = 20
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 135
    Top = 63
    Width = 75
    Height = 20
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancelar'
    ModalResult = 2
    TabOrder = 1
  end
  object ComboBoxMeter: TComboBox
    Left = 72
    Top = 21
    Width = 153
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akBottom]
    ItemHeight = 13
    TabOrder = 2
    OnChange = ComboBoxMeterChange
  end
end
