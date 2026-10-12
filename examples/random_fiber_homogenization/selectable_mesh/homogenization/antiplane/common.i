!include ../solver.i
[Mesh]
  [rve]
    type = FileMeshGenerator
    file = ../../concentric.e
  []
[]
[GlobalParams]
  macro_gradient = macro_gradient
  constraint_types = 'strain strain'
  targets = 'target_x target_y'
[]
[Functions]
  [target_x]
    type = ParsedFunction
    expression = ${gamma_xz}
  []
  [target_y]
    type = ParsedFunction
    expression = ${gamma_yz}
  []
[]
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
    boundary = bottom_left
    value = 0
  []
[]
[Materials]
  [strain]
    type = ADHomoxAntiplaneStrain
    displacement = disp_z
  []
  [matrix]
    type = ADHomoxAntiplaneLinearElasticMaterial
    block = matrix
    shear_modulus = 40
  []
  [fiber]
    type = ADHomoxAntiplaneLinearElasticMaterial
    block = fiber
    shear_modulus = 400
  []
[]
[AuxVariables]
  [gamma_xz]
    family = MONOMIAL
    order = CONSTANT
  []
  [gamma_yz]
    family = MONOMIAL
    order = CONSTANT
  []
  [tau_xz]
    family = MONOMIAL
    order = CONSTANT
  []
  [tau_yz]
    family = MONOMIAL
    order = CONSTANT
  []
[]
[AuxKernels]
  [gamma_xz]
    type = ADMaterialRealVectorValueAux
    variable = gamma_xz
    property = antiplane_strain
    component = 0
  []
  [gamma_yz]
    type = ADMaterialRealVectorValueAux
    variable = gamma_yz
    property = antiplane_strain
    component = 1
  []
  [tau_xz]
    type = ADMaterialRealVectorValueAux
    variable = tau_xz
    property = antiplane_stress
    component = 0
  []
  [tau_yz]
    type = ADMaterialRealVectorValueAux
    variable = tau_yz
    property = antiplane_stress
    component = 1
  []
[]
[Postprocessors]
  [gamma_xz]
    type = ElementAverageValue
    variable = gamma_xz
  []
  [gamma_yz]
    type = ElementAverageValue
    variable = gamma_yz
  []
  [tau_xz]
    type = ElementAverageValue
    variable = tau_xz
  []
  [tau_yz]
    type = ElementAverageValue
    variable = tau_yz
  []
  [energy]
    type = ADElementIntegralMaterialProperty
    mat_prop = antiplane_energy
  []
  [area]
    type = VolumePostprocessor
  []
  [fiber_area]
    type = VolumePostprocessor
    block = fiber
  []
[]
