# Periodic static antiplane loading with <tau_xz> = 1.
[GlobalParams]
  macro_gradient = macro_gradient
  constraint_types = 'stress stress'
  targets = 'target_x target_y'
[]
[Functions]
  [target_x]
    type = ParsedFunction
    expression = 1
  []
  [target_y]
    type = ParsedFunction
    expression = 0
  []
[]
!include ../periodic_common.i
