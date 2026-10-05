# Nonperiodic kinematic homogenization with epsilon_xy = 0.01 (gamma_xy = 0.02).
[GlobalParams]
  displacements = 'disp_x disp_y'
[]
!include ../nonperiodic_common.i
[Functions]
  [affine_x]
    type = ParsedFunction
    expression = '0.01*y'
  []
  [affine_y]
    type = ParsedFunction
    expression = '0.01*x'
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
    function = affine_y
    preset = true
  []
[]
