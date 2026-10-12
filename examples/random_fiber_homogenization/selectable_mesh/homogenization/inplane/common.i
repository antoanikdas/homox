!include ../solver.i
[Mesh]
  [rve]
    type = FileMeshGenerator
    file = ../../concentric.e
  []
[]
[GlobalParams]
  displacements = 'disp_x disp_y'
  macro_strain = macro_strain
  constraint_types = 'strain none none strain strain none none none none'
  targets = 'target_xx target_xy target_yy'
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
    block = matrix
    youngs_modulus = 100
    poissons_ratio = 0.25
  []
  [fiber]
    type = ADHomoxLinearElasticMaterial
    block = fiber
    youngs_modulus = 1000
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
  [energy]
    type = ADElementIntegralMaterialProperty
    mat_prop = elastic_energy
  []
  [area]
    type = VolumePostprocessor
  []
  [fiber_area]
    type = VolumePostprocessor
    block = fiber
  []
[]
