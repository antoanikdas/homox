# Periodic kinematic homogenization with <epsilon_xx> = 0.01.
[GlobalParams]
  displacements = 'disp_x disp_y'
  macro_strain = macro_strain
  constraint_types = 'strain none none strain strain none none none none'
  targets = 'target_xx target_xy target_yy'
[]
[Functions]
  [target_xx]
    type = ParsedFunction
    expression = 0.01
  []
  [target_xy]
    type = ParsedFunction
    expression = 0
  []
  [target_yy]
    type = ParsedFunction
    expression = 0
  []
[]
!include ../periodic_common.i
