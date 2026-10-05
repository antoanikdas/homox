!include nonperiodic_common.i

# Static uniform boundary condition for sigma_xx = 1; other boundary tractions are zero.
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
  [right_x]
    type = FunctionNeumannBC
    variable = disp_x
    boundary = right
    function = positive_traction
  []
  [left_x]
    type = FunctionNeumannBC
    variable = disp_x
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
