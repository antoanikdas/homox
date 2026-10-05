!include common.i

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[Kernels]
  [equilibrium_x]
    type = ADHomoxStressDivergence
    variable = disp_x
    component = 0
  []
  [equilibrium_y]
    type = ADHomoxStressDivergence
    variable = disp_y
    component = 1
  []
[]

[Materials]
  [strain]
    type = ADHomoxCompatibleSmallStrain
  []
[]
