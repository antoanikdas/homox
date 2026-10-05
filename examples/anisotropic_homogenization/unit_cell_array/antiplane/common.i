# Four-cell (2x2) anisotropic antiplane homogenization problem.
[Mesh]
  [unit_cells]
    type = HomoxMultiInclusionMeshGenerator
    radius = 0.2820947918
    n_left = 0
    n_right = 1
    n_top = 1
    n_bottom = 0
    num_sectors = 8
    inclusion_rings = 6
    matrix_rings = 8
  []
  [pin_origin]
    type = ExtraNodesetGenerator
    input = unit_cells
    new_boundary = pin_origin
    coord = '-0.5 -0.5 0'
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
    block = matrix
    antiplane_stiffness = '70 10 45'
  []
  [inclusion]
    type = ADHomoxAntiplaneLinearElasticMaterial
    block = inclusion
    antiplane_stiffness = '220 -12 140'
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
