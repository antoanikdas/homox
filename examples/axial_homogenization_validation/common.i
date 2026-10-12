[GlobalParams]
  displacements = 'disp_x disp_y'
[]
[Mesh]
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 4
    ny = 4
  []
  [pin]
    type = ExtraNodesetGenerator
    input = square
    new_boundary = bottom_left
    coord = '0 0 0'
  []
  [pin_right]
    type = ExtraNodesetGenerator
    input = pin
    new_boundary = bottom_right
    coord = '1 0 0'
  []
[]
[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]
[Materials]
  [elasticity]
    type = ADHomoxLinearElasticMaterial
    youngs_modulus = 100
    poissons_ratio = 0.25
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
  [szz]
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
  [ezz]
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
  [szz]
    type = ADHomoxRankTwoAux
    variable = szz
    tensor = stress
    index_i = 2
    index_j = 2
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
  [ezz]
    type = ADHomoxRankTwoAux
    variable = ezz
    tensor = mechanical_strain
    index_i = 2
    index_j = 2
  []
  [exy]
    type = ADHomoxRankTwoAux
    variable = exy
    tensor = mechanical_strain
    index_i = 0
    index_j = 1
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
  [szz]
    type = ElementAverageValue
    variable = szz
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
  [ezz]
    type = ElementAverageValue
    variable = ezz
  []
  [exy]
    type = ElementAverageValue
    variable = exy
  []
  [energy]
    type = ADElementIntegralMaterialProperty
    mat_prop = elastic_energy
  []
  [area]
    type = VolumePostprocessor
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
