zz_strain = 0
!include ../nonperiodic_common.i
[Functions]
  [ux]
    type = ParsedFunction
    expression = '0.005*y'
  []
  [uy]
    type = ParsedFunction
    expression = '0.005*x'
  []
[]
[BCs]
  [x]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'left right bottom top'
    function = ux
  []
  [y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'left right bottom top'
    function = uy
  []
[]
[Outputs]
  file_base = xy_out
[]
