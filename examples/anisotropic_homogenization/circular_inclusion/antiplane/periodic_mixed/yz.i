# Periodic mixed antiplane loading: <tau_xz> = 0 and <gamma_yz> = 0.01.
[GlobalParams]
  macro_gradient = macro_gradient
  constraint_types = 'stress strain'
  targets = 'target_x target_y'
[]
[Functions]
  [target_x]
    type = ParsedFunction
    expression = 0
  []
  [target_y]
    type = ParsedFunction
    expression = 0.01
  []
[]
!include ../periodic_common.i
