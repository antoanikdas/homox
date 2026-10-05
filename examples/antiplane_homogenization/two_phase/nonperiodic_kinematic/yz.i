# Nonperiodic kinematic antiplane loading with gamma_yz = 0.01.
!include ../nonperiodic_common.i
[Functions]
  [affine]
    type = ParsedFunction
    expression = '0.01*y'
  []
[]
[BCs]
  [affine]
    type = FunctionDirichletBC
    variable = disp_z
    boundary = 'left right bottom top'
    function = affine
    preset = true
  []
[]
