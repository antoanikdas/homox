zz_strain = 0
!include ../nonperiodic_common.i
[BCs]
  [bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value = 0
  []
  [top_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'top'
    value = 0.01
  []
  [left_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'left'
    value = 0
  []
  [right_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'right'
    value = 0
  []
[]
[Outputs]
  file_base = xy_out
[]
