zz_strain = 0
!include ../nonperiodic_common.i
[BCs]
  [right_y]
    type = NeumannBC
    variable = disp_y
    boundary = right
    value = 1
  []
  [left_y]
    type = NeumannBC
    variable = disp_y
    boundary = left
    value = -1
  []
  [top_x]
    type = NeumannBC
    variable = disp_x
    boundary = top
    value = 1
  []
  [bottom_x]
    type = NeumannBC
    variable = disp_x
    boundary = bottom
    value = -1
  []
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
  file_base = xy_out
[]
