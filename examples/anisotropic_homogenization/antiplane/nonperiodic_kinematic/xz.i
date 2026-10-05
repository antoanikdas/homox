# Nonperiodic kinematic antiplane loading with gamma_xz = 0.01.
!include ../nonperiodic_common.i
[Functions]
  [affine]
    type = ParsedFunction
    expression = '0.01*x'
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
