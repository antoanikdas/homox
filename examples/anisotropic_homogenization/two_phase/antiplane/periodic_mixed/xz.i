# Periodic mixed antiplane loading: <gamma_xz> = 0.01 and <tau_yz> = 0.
[GlobalParams]
  macro_gradient = macro_gradient
  constraint_types = 'strain stress'
  targets = 'target_x target_y'
[]
[Functions]
  [target_x]
    type = ParsedFunction
    expression = 0.01
  []
  [target_y]
    type = ParsedFunction
    expression = 0
  []
[]
!include ../periodic_common.i
