# Shared 3x3 circular-inclusion array for antiplane periodic homogenization.
[Mesh]
  [multi_inclusion]
    type = HomoxMultiInclusionMeshGenerator
    radius = 0.2
    n_left = 1
    n_right = 1
    n_top = 1
    n_bottom = 1
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
  [inclusion]
    type = ADHomoxAntiplaneLinearElasticMaterial
    block = inclusion
    shear_modulus = 400
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

