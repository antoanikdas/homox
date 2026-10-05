# Nonperiodic static antiplane loading with tau_xz = 1.
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
  [right]
    type = FunctionNeumannBC
    variable = disp_z
    boundary = right
    function = positive
  []
  [left]
    type = FunctionNeumannBC
    variable = disp_z
    boundary = left
    function = negative
  []
  [pin]
    type = DirichletBC
    variable = disp_z
    boundary = pin_origin
    value = 0
  []
[]
