object FrameExperiment: TFrameExperiment
  Left = 0
  Top = 0
  Width = 322
  Height = 249
  TabOrder = 0
  object ListViewFirst: TListView
    Left = 0
    Top = 0
    Width = 322
    Height = 145
    Align = alClient
    Columns = <
      item
        Caption = 'Rodada'
        Width = 80
      end
      item
        Caption = 'A'
        Width = 40
      end
      item
        Caption = 'B'
        Width = 40
      end
      item
        Caption = 'C'
        Width = 40
      end
      item
        Caption = 'Melhor'
      end
      item
        Caption = 'Pior'
      end>
    ReadOnly = True
    RowSelect = True
    TabOrder = 0
    ViewStyle = vsReport
  end
  object ListViewSecond: TListView
    Left = 0
    Top = 145
    Width = 322
    Height = 104
    Align = alBottom
    Columns = <
      item
        Caption = 'Conjunto 1'
        Width = 90
      end
      item
        Caption = 'Conjunto 2'
        Width = 90
      end
      item
        Caption = 'Mais Variado'
        Width = 115
      end>
    ReadOnly = True
    RowSelect = True
    TabOrder = 1
    ViewStyle = vsReport
  end
end
