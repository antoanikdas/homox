# Shared two-phase antiplane SVE: G_matrix = 40, G_inclusion = 400.
[Mesh]
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 16
    ny = 16
    elem_type = QUAD4
  []
  [inclusion]
    type = ParsedSubdomainMeshGenerator
    input = square
    combinatorial_geometry = 'x > 0.25 & x < 0.75 & y > 0.25 & y < 0.75'
    block_id = 1
  []
  [pin_origin]
    type = ExtraNodesetGenerator
    input = inclusion
    new_boundary = pin_origin
    coord = '0 0 0'
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
  [energy]
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
  [energy]
    type = ADMaterialRealAux
    variable = energy
    property = antiplane_energy
  []
[]

[Materials]
  [matrix]
    type = ADHomoxAntiplaneLinearElasticMaterial
    block = 0
    shear_modulus = 40
  []
  [inclusion]
    type = ADHomoxAntiplaneLinearElasticMaterial
    block = 1
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
  [energy]
    type = ElementAverageValue
    variable = energy
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
