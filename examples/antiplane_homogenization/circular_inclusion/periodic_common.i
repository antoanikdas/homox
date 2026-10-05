!include common.i

[Variables]
  [disp_z]
  []
  [macro_gradient]
    family = SCALAR
    order = SECOND
  []
[]

[Kernels]
  [equilibrium]
    type = ADHomoxAntiplaneHomogenization
    variable = disp_z
    scalar_variable = macro_gradient
  []
[]

[BCs]
  [Periodic]
    [disp_z]
      variable = disp_z
      auto_direction = 'x y'
    []
  []
  [pin]
    type = DirichletBC
    variable = disp_z
    boundary = pin_origin
    value = 0
  []
[]

[Materials]
  [strain]
    type = ADHomoxAntiplaneStrain
    displacement = disp_z
  []
[]
