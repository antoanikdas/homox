# Two-phase anisotropic plane-strain SVE with a 25% circular inclusion.
[Mesh]
  [circular_sve]
    type = ConcentricCircleMeshGenerator
    num_sectors = 8
    radii = 0.2820947918
    rings = '6 8'
    has_outer_square = true
    pitch = 1
    preserve_volumes = true
    smoothing_max_it = 1
  []
  [pin_origin]
    type = ExtraNodesetGenerator
    input = circular_sve
    new_boundary = pin_origin
    coord = '-0.5 -0.5 0'
  []
  [pin_x]
    type = ExtraNodesetGenerator
    input = pin_origin
    new_boundary = pin_x
    coord = '0.5 -0.5 0'
  []
[]

[AuxVariables]
  [sxx]
    family = MONOMIAL
    order = CONSTANT
  []
  [syy]
    family = MONOMIAL
    order = CONSTANT
  []
  [sxy]
    family = MONOMIAL
    order = CONSTANT
  []
  [exx]
    family = MONOMIAL
    order = CONSTANT
  []
  [eyy]
    family = MONOMIAL
    order = CONSTANT
  []
  [exy]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [sxx]
    type = ADHomoxRankTwoAux
    variable = sxx
    tensor = stress
    index_i = 0
    index_j = 0
  []
  [syy]
    type = ADHomoxRankTwoAux
    variable = syy
    tensor = stress
    index_i = 1
    index_j = 1
  []
  [sxy]
    type = ADHomoxRankTwoAux
    variable = sxy
    tensor = stress
    index_i = 0
    index_j = 1
  []
  [exx]
    type = ADHomoxRankTwoAux
    variable = exx
    tensor = mechanical_strain
    index_i = 0
    index_j = 0
  []
  [eyy]
    type = ADHomoxRankTwoAux
    variable = eyy
    tensor = mechanical_strain
    index_i = 1
    index_j = 1
  []
  [exy]
    type = ADHomoxRankTwoAux
    variable = exy
    tensor = mechanical_strain
    index_i = 0
    index_j = 1
  []
[]

[Materials]
  [matrix]
    type = ADHomoxLinearElasticMaterial
    block = 2
    fill_method = symmetric21
    # 1111 1122 1133 1123 1113 1112 2222 2233 2223 2213 2212
    # 3333 3323 3313 3312 2323 2313 2312 1313 1312 1212
    C_ijkl = '200 50 40 0 0 12 120 30 0 0 -8 150 0 0 0 45 10 0 70 0 60'
  []
  [inclusion]
    type = ADHomoxLinearElasticMaterial
    block = 1
    fill_method = symmetric21
    # 1111 1122 1133 1123 1113 1112 2222 2233 2223 2213 2212
    # 3333 3323 3313 3312 2323 2313 2312 1313 1312 1212
    C_ijkl = '600 100 80 0 0 -20 400 60 0 0 15 500 0 0 0 140 -12 0 220 0 180'
  []
[]

[Postprocessors]
  [sxx]
    type = ElementAverageValue
    variable = sxx
  []
  [syy]
    type = ElementAverageValue
    variable = syy
  []
  [sxy]
    type = ElementAverageValue
    variable = sxy
  []
  [exx]
    type = ElementAverageValue
    variable = exx
  []
  [eyy]
    type = ElementAverageValue
    variable = eyy
  []
  [exy]
    type = ElementAverageValue
    variable = exy
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu superlu_dist'
  l_max_its = 200
  l_tol = 1e-14
  nl_max_its = 20
  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-12
[]

[Outputs]
  csv = true
  exodus = true
[]
