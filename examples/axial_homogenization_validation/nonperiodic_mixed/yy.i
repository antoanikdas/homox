zz_strain = 0
!include ../nonperiodic_common.i
[BCs]
  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [top_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'top'
    value = 0.01
  []
  [pin_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom_left'
    value = 0
  []
[]
[Outputs]
  file_base = yy_out
[]
