!include common.i
[Kernels]
  [x]
    type = ADHomoxStressDivergence
    variable = disp_x
    component = 0
  []
  [y]
    type = ADHomoxStressDivergence
    variable = disp_y
    component = 1
  []
[]
[Materials/strain]
  type = ADHomoxCompatibleSmallStrain
  out_of_plane_strain = ${zz_strain}
[]
