!include common.i
[GlobalParams]
  macro_strain = macro_strain
  constraint_types = ${controls}
  targets = 'target_xx target_xy target_yy target_zz'
[]
[Functions]
  [target_xx]
    type = ParsedFunction
    expression = ${xx_target}
  []
  [target_xy]
    type = ParsedFunction
    expression = ${xy_target}
  []
  [target_yy]
    type = ParsedFunction
    expression = ${yy_target}
  []
  [target_zz]
    type = ParsedFunction
    expression = ${zz_target}
  []
[]
[Variables/macro_strain]
  family = SCALAR
  order = FOURTH
[]
[Kernels]
  [x]
    type = ADHomoxHomogenization
    variable = disp_x
    component = 0
    scalar_variable = macro_strain
  []
  [y]
    type = ADHomoxHomogenization
    variable = disp_y
    component = 1
    scalar_variable = macro_strain
    compute_scalar_residuals = false
  []
[]
[Materials/strain]
  type = ADHomoxSmallStrain
[]
[BCs]
  [Periodic]
    [x]
      variable = disp_x
      auto_direction = 'x y'
    []
    [y]
      variable = disp_y
      auto_direction = 'x y'
    []
  []
  [pin_x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom_left
    value = 0
  []
  [pin_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom_left
    value = 0
  []
[]
