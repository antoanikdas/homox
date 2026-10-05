# Periodic mixed homogenization: <epsilon_yy> = 0.01, <sigma_xx> = <sigma_xy> = 0.
[GlobalParams]
  displacements = 'disp_x disp_y'
  macro_strain = macro_strain
  constraint_types = 'stress none none stress strain none none none none'
  targets = 'target_xx target_xy target_yy'
[]
[Functions]
  [target_xx]
    type = ParsedFunction
    expression = 0
  []
  [target_xy]
    type = ParsedFunction
    expression = 0
  []
  [target_yy]
    type = ParsedFunction
    expression = 0.01
  []
[]
!include ../periodic_common.i
