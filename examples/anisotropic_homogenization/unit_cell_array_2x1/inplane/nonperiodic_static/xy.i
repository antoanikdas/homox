# Nonperiodic static homogenization with sigma_xy = 1 on all opposing faces.
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
  [top_x]
    type = FunctionNeumannBC
    variable = disp_x
    boundary = top
    function = positive_traction
  []
  [bottom_x]
    type = FunctionNeumannBC
    variable = disp_x
    boundary = bottom
    function = negative_traction
  []
  [right_y]
    type = FunctionNeumannBC
    variable = disp_y
    boundary = right
    function = positive_traction
  []
  [left_y]
    type = FunctionNeumannBC
    variable = disp_y
    boundary = left
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
