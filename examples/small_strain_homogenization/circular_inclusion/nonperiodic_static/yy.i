# Nonperiodic static homogenization with sigma_yy = 1.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]
!include ../nonperiodic_common.i
[Functions]
  [positive_traction]
    type = ParsedFunction
    expression = 1
  []
  [negative_traction]
    type = ParsedFunction
    expression = -1
  []
[]
[BCs]
  [top_y]
    type = FunctionNeumannBC
    variable = disp_y
    boundary = top
    function = positive_traction
  []
  [bottom_y]
    type = FunctionNeumannBC
    variable = disp_y
    boundary = bottom
    function = negative_traction
  []
  [pin_origin_x]
    type = DirichletBC
    variable = disp_x
    boundary = pin_origin
    value = 0
  []
  [pin_origin_y]
    type = DirichletBC
    variable = disp_y
    boundary = pin_origin
    value = 0
  []
  [remove_rotation]
    type = DirichletBC
    variable = disp_y
    boundary = pin_x
    value = 0
  []
[]
