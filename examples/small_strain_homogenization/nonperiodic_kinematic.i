!include nonperiodic_common.i

# Kinematic uniform boundary condition: u = epsilon_bar * x on the complete boundary.
[Functions]
  [affine_x]
    type = ParsedFunction
    expression = '0.01*x'
  []
  [zero]
    type = ParsedFunction
    expression = 0
  []
[]

[BCs]
  [affine_x]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'left right bottom top'
    function = affine_x
    preset = true
  []
  [affine_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'left right bottom top'
    function = zero
    preset = true
  []
[]
