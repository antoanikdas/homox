# Periodic mixed homogenization: <epsilon_xy> = 0.01, <sigma_xx> = <sigma_yy> = 0.
[GlobalParams]
  displacements = 'disp_x disp_y'
  macro_strain = macro_strain
  constraint_types = 'stress none none strain stress none none none none'
  targets = 'target_xx target_xy target_yy'
[]
[Functions]
  [target_xx]
    type = ParsedFunction
    expression = 0
  []
  [target_xy]
    type = ParsedFunction
    expression = 0.01
  []
  [target_yy]
    type = ParsedFunction
    expression = 0
  []
[]
!include ../periodic_common.i
