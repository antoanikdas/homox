!include common.i

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [macro_strain]
    family = SCALAR
    order = THIRD
  []
[]

[Kernels]
  [equilibrium_x]
    type = ADHomoxHomogenization
    variable = disp_x
    component = 0
    scalar_variable = macro_strain
  []
  [equilibrium_y]
    type = ADHomoxHomogenization
    variable = disp_y
    component = 1
    scalar_variable = macro_strain
    compute_scalar_residuals = false
  []
[]

[BCs]
  [Periodic]
    [disp_x]
      variable = disp_x
      auto_direction = 'x y'
    []
    [disp_y]
      variable = disp_y
      auto_direction = 'x y'
    []
  []
  [pin_origin_x]
    type = DirichletBC
    variable = disp_x
    boundary = pin_origin
    value = 0
  []
  [pin_origin_y]
    type = DirichletBC
    variable = disp_y
    boundary = pin_origin
    value = 0
  []
  [remove_rotation]
    type = DirichletBC
    variable = disp_y
    boundary = pin_x
    value = 0
  []
[]

[Materials]
  [strain]
    type = ADHomoxSmallStrain
  []
[]
