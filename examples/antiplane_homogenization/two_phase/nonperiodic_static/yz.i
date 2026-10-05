# Nonperiodic static antiplane loading with tau_yz = 1.
!include ../nonperiodic_common.i
[Functions]
  [positive]
    type = ParsedFunction
    expression = 1
  []
  [negative]
    type = ParsedFunction
    expression = -1
  []
[]
[BCs]
  [top]
    type = FunctionNeumannBC
    variable = disp_z
    boundary = top
    function = positive
  []
  [bottom]
    type = FunctionNeumannBC
    variable = disp_z
    boundary = bottom
    function = negative
  []
  [pin]
    type = DirichletBC
    variable = disp_z
    boundary = pin_origin
    value = 0
  []
[]
