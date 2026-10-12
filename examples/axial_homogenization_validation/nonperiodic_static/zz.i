zz_strain = 0.01
!include ../nonperiodic_common.i
[BCs]
  [pin_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom_left'
    value = 0
  []
  [pin_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom_left'
    value = 0
  []
  [rotation]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom_right'
    value = 0
  []
[]
[Outputs]
  file_base = zz_out
[]
