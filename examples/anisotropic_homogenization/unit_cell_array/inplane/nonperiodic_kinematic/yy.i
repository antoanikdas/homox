# Nonperiodic kinematic homogenization with epsilon_yy = 0.01.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]
!include ../nonperiodic_common.i
[Functions]
  [zero]
    type = ParsedFunction
    expression = 0
  []
  [affine_y]
    type = ParsedFunction
    expression = '0.01*y'
  []
[]
[BCs]
  [affine_x]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'left right bottom top'
    function = zero
    preset = true
  []
  [affine_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'left right bottom top'
    function = affine_y
    preset = true
  []
[]
