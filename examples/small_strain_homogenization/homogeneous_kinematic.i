# Homogeneous periodic SVE. Exact plane-strain result for E=100, nu=0.25:
# <sigma_xx> = 1.2 and <sigma_yy> = 0.4 when <epsilon_xx> = 0.01.
[GlobalParams]
  displacements = 'disp_x disp_y'
  macro_strain = macro_strain
  constraint_types = 'strain none none strain strain none none none none'
  targets = 'target_xx target_xy target_yy'
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
[]

[Mesh]
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 4
    ny = 4
  []
  [pin_origin]
    type = ExtraNodesetGenerator
    input = square
    new_boundary = pin_origin
    coord = '0 0 0'
  []
  [pin_x]
    type = ExtraNodesetGenerator
    input = pin_origin
    new_boundary = pin_x
    coord = '1 0 0'
  []
[]

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
  [elasticity]
    type = ADHomoxLinearElasticMaterial
    youngs_modulus = 100
    poissons_ratio = 0.25
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
