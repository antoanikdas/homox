# Periodic static homogenization with <sigma_xy> = 1.
[GlobalParams]
  displacements = 'disp_x disp_y'
  macro_strain = macro_strain
  constraint_types = 'stress none none stress stress none none none none'
  targets = 'target_xx target_xy target_yy'
[]
[Functions]
  [target_xx]
    type = ParsedFunction
    expression = 0
  []
  [target_xy]
    type = ParsedFunction
    expression = 1
  []
  [target_yy]
    type = ParsedFunction
    expression = 0
  []
[]
!include ../periodic_common.i
