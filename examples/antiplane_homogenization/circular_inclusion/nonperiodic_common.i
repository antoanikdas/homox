!include common.i

[Variables]
  [disp_z]
  []
[]

[Kernels]
  [equilibrium]
    type = ADHomoxAntiplaneStressDivergence
    variable = disp_z
  []
[]

[Materials]
  [strain]
    type = ADHomoxAntiplaneCompatibleStrain
    displacement = disp_z
  []
[]
