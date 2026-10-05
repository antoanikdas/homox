# Small-Strain Homogenization

`homox` implements small-strain linear-elastic homogenization without linking the MOOSE Solid
Mechanics module. All implementation classes and verification cases reside in this application.

The displacement is decomposed as

\[
u_i(\mathbf{x}) = \bar{\epsilon}_{ij}x_j + \tilde{u}_i(\mathbf{x}),
\qquad
\epsilon_{ij} = \bar{\epsilon}_{ij}
 + \frac{1}{2}(\tilde{u}_{i,j}+\tilde{u}_{j,i}),
\]

where the periodic fluctuation field is `disp_x`, `disp_y`, and optionally `disp_z`. The components
of the macroscopic strain are scalar degrees of freedom in `macro_strain`. The constitutive response
is

\[
\sigma_{ij}=C_{ijkl}\epsilon_{kl},
\]

and equilibrium is solved from

\[
\int_\Omega \sigma_{ij}\,\delta\tilde{u}_{i,j}\,dV=0.
\]

For each active scalar component, `ADHomoxHomogenization` additionally enforces either

\[
\int_\Omega (\epsilon_{ij}-\epsilon_{ij}^{\mathrm{target}})\,dV=0
\]

or

\[
\int_\Omega (\sigma_{ij}-\sigma_{ij}^{\mathrm{target}})\,dV=0.
\]

The implementation consists of:

- `ADHomoxSmallStrain`: fluctuation plus macroscopic infinitesimal strain;
- `ADHomoxCompatibleSmallStrain`: compatible strain for direct nonperiodic boundary problems;
- `ADHomoxLinearElasticMaterial`: isotropic or general anisotropic stiffness, stress, and energy;
- `ADHomoxHomogenization`: equilibrium and average strain/stress constraints;
- `ADHomoxStressDivergence`: equilibrium without scalar average constraints;
- `ADHomoxRankTwoAux`: stress and strain component output.

The example `examples/small_strain_homogenization/homogeneous_kinematic.i` is a periodic,
homogeneous plane-strain verification problem. General anisotropy is selected with `C_ijkl` and
`fill_method`; isotropy is selected with `youngs_modulus` and `poissons_ratio`.

## Boundary-condition families

The periodic kinematic, mixed, and static cases use periodic displacement fluctuations and scalar
volume-average constraints. The nonperiodic cases are direct boundary-value problems:

- **Kinematic:** the affine displacement `u = epsilon_bar*x` is prescribed on the complete boundary.
- **Mixed:** for normal loading, axial displacement is prescribed on the loaded boundary pair while
  transverse boundaries remain traction-free. For shear, tangential displacement is prescribed on
  each boundary pair while the normal components remain traction-free; this also removes average
  rigid rotation.
- **Static:** uniform traction is prescribed on opposing boundaries, with only rigid translation
  and rotation removed.

The nonperiodic examples are `nonperiodic_kinematic.i`, `nonperiodic_mixed.i`, and
`nonperiodic_static.i` in `examples/small_strain_homogenization`.

The self-contained `two_phase` study uses a generated 16-by-16 square mesh with a centered square
inclusion occupying 25% of the area. Separate block-restricted `ADHomoxLinearElasticMaterial`
objects assign `E = 100`, `nu = 0.25` to the matrix and `E = 1000`, `nu = 0.25` to the inclusion.
It contains `xx`, `yy`, and `xy` loading cases for all six boundary-condition families:

- `periodic_kinematic`, `periodic_mixed`, and `periodic_static`;
- `nonperiodic_kinematic`, `nonperiodic_mixed`, and `nonperiodic_static`.

The periodic cases use scalar volume-average constraints. The nonperiodic cases impose their
corresponding affine displacement, mixed displacement/traction, or uniform-traction conditions
directly on the outer boundary. For shear cases, the prescribed tensor shear is
`epsilon_xy = 0.01`, equivalent to engineering shear `gamma_xy = 0.02`. Shared files in the
`two_phase` directory define the same mesh, phases, postprocessors, and solver for every case.
