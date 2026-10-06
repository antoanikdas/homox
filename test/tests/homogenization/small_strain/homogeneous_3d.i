# Homogeneous 3D periodic cell. E=100 and nu=0.25 give lambda=mu=40.
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  macro_strain = macro_strain
  constraint_types = 'strain none none strain strain none strain strain strain'
  targets = 'target_xx target_xy target_yy target_xz target_yz target_zz'
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
  [target_xz]
    type = ParsedFunction
    expression = 0
  []
  [target_yz]
    type = ParsedFunction
    expression = 0
  []
  [target_zz]
    type = ParsedFunction
    expression = 0
  []
[]

[Mesh]
  [cube]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 2
    ny = 2
    nz = 2
    elem_type = HEX8
  []
  [pin_origin]
    type = ExtraNodesetGenerator
    input = cube
    new_boundary = pin_origin
    coord = '0 0 0'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
  [macro_strain]
    family = SCALAR
    order = SIXTH
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
  [equilibrium_z]
    type = ADHomoxHomogenization
    variable = disp_z
    component = 2
    scalar_variable = macro_strain
    compute_scalar_residuals = false
  []
[]

[BCs]
  [Periodic]
    [disp_x]
      variable = disp_x
      auto_direction = 'x y z'
    []
    [disp_y]
      variable = disp_y
      auto_direction = 'x y z'
    []
    [disp_z]
      variable = disp_z
      auto_direction = 'x y z'
    []
  []
  [pin_x]
    type = DirichletBC
    variable = disp_x
    boundary = pin_origin
    value = 0
  []
  [pin_y]
    type = DirichletBC
    variable = disp_y
    boundary = pin_origin
    value = 0
  []
  [pin_z]
    type = DirichletBC
    variable = disp_z
    boundary = pin_origin
    value = 0
  []
[]

[Materials]
  [strain]
    type = ADHomoxSmallStrain
  []
  [elasticity]
    type = ADHomoxLinearElasticMaterial
    youngs_modulus = 100
    poissons_ratio = 0.25
  []
[]

[AuxVariables]
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
  [exz]
    family = MONOMIAL
    order = CONSTANT
  []
  [eyz]
    family = MONOMIAL
    order = CONSTANT
  []
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
  [sxz]
    family = MONOMIAL
    order = CONSTANT
  []
  [syz]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
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
  [exz]
    type = ADHomoxRankTwoAux
    variable = exz
    tensor = mechanical_strain
    index_i = 0
    index_j = 2
  []
  [eyz]
    type = ADHomoxRankTwoAux
    variable = eyz
    tensor = mechanical_strain
    index_i = 1
    index_j = 2
  []
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
  [sxz]
    type = ADHomoxRankTwoAux
    variable = sxz
    tensor = stress
    index_i = 0
    index_j = 2
  []
  [syz]
    type = ADHomoxRankTwoAux
    variable = syz
    tensor = stress
    index_i = 1
    index_j = 2
  []
[]

[Postprocessors]
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
  [exz]
    type = ElementAverageValue
    variable = exz
  []
  [eyz]
    type = ElementAverageValue
    variable = eyz
  []
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
  [sxz]
    type = ElementAverageValue
    variable = sxz
  []
  [syz]
    type = ElementAverageValue
    variable = syz
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
  file_base = homogeneous_3d_xx_out
  csv = true
  exodus = false
[]
