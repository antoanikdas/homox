strain_xx = 0
strain_yy = 0
strain_xy = 0
axial_target = 0.01

[Mesh]
  [rve]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 2
    ny = 2
  []
  [pin]
    type = ExtraNodesetGenerator
    input = rve
    new_boundary = bottom_left
    coord = '0 0 0'
  []
[]
[GlobalParams]
  displacements = 'disp_x disp_y'
  macro_strain = macro_strain
  constraint_types = 'strain none none strain strain none none none strain'
  targets = 'target_xx target_xy target_yy target_zz'
[]
[Functions]
  [target_xx]
    type = ParsedFunction
    expression = ${strain_xx}
  []
  [target_xy]
    type = ParsedFunction
    expression = ${strain_xy}
  []
  [target_yy]
    type = ParsedFunction
    expression = ${strain_yy}
  []
[]
[Functions/target_zz]
  type = ParsedFunction
  expression = ${axial_target}
[]
[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [macro_strain]
    family = SCALAR
    order = FOURTH
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
[Materials]
  [strain]
    type = ADHomoxSmallStrain
  []
  [matrix]
    type = ADHomoxLinearElasticMaterial
    youngs_modulus = 100
    poissons_ratio = 0.25
  []
[]
[AuxVariables]
  [szz]
    family = MONOMIAL
    order = CONSTANT
  []
  [ezz]
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
  [szz]
    type = ADHomoxRankTwoAux
    variable = szz
    tensor = stress
    index_i = 2
    index_j = 2
  []
  [ezz]
    type = ADHomoxRankTwoAux
    variable = ezz
    tensor = mechanical_strain
    index_i = 2
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
[Postprocessors]
  [szz]
    type = ElementAverageValue
    variable = szz
  []
  [ezz]
    type = ElementAverageValue
    variable = ezz
  []
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
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu superlu_dist'
  nl_abs_tol = 1e-12
[]
[Outputs]
  csv = true
[]
