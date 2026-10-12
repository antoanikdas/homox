zz_strain = 0.01
!include ../nonperiodic_common.i
[BCs]
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0
  []
  [right_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'right'
    value = 0
  []
  [pin_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom_left'
    value = 0
  []
[]
[Outputs]
  file_base = zz_out
[]
