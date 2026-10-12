# Homogeneous Axial Verification: All Six BC Families

Unit-square 4 x 4 QUAD4 cross-section, homogeneous isotropic E=100, nu=0.25.
All 24 solves use NEWTON and SuperLU_DIST. The fiber direction is z.

## Exact Matrix

Order: (xx, yy, zz, xy), with engineering shear gamma_xy=2*epsilon_xy.
The 4-by-4 generalized-plane-strain block is:

```text
[120  40  40   0]
[ 40 120  40   0]
[ 40  40 120   0]
[  0   0   0  40]
```

Thus C13=C23=40, C33=120, and C36=0 for every BC family.

## BC Definitions

Periodic cases solve affine macro strain plus periodic displacement fluctuations.
Kinematic imposes all four average strains. Mixed imposes the loaded transverse
strain while relaxing the complementary transverse stresses. For the zz mixed
case, epsilon_xx=0, sigma_yy=sigma_xy=0, and epsilon_zz=0.01.
Static prescribes all three transverse average stresses, while epsilon_zz is imposed.

Nonperiodic kinematic prescribes affine displacement on the complete outer boundary.
Nonperiodic mixed prescribes opposing x or y displacements with complementary
traction-free directions; its xy case uses top/bottom x-displacement and fixed
left/right y-displacement. Nonperiodic static applies uniform transverse boundary
tractions and removes only rigid translations and rotation with corner pins.

For nonperiodic cases, ADHomoxCompatibleSmallStrain supplies uniform epsilon_zz
through out_of_plane_strain. The name static refers to transverse control; no
physical z-end faces exist on this 2D mesh. These are axial-strain-controlled
generalized-plane-strain tests, not fully 3D uniform-traction tests.

## Axial Load Response

All zz cases impose epsilon_zz=0.01. Their transverse restraints differ:

| Family | epsilon_xx | epsilon_yy | sigma_xx | sigma_yy | sigma_zz | Energy density |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| periodic_kinematic | 0.000000000 | 0.000000000 | 0.400000000 | 0.400000000 | 1.200000000 | 0.006000000 |
| periodic_mixed | 0.000000000 | -0.003333333 | 0.266666667 | 0.000000000 | 1.066666667 | 0.005333333 |
| periodic_static | -0.002500000 | -0.002500000 | 0.000000000 | 0.000000000 | 1.000000000 | 0.005000000 |
| nonperiodic_kinematic | 0.000000000 | 0.000000000 | 0.400000000 | 0.400000000 | 1.200000000 | 0.006000000 |
| nonperiodic_mixed | 0.000000000 | -0.003333333 | 0.266666667 | 0.000000000 | 1.066666667 | 0.005333333 |
| nonperiodic_static | -0.002500000 | -0.002500000 | -0.000000000 | 0.000000000 | 1.000000000 | 0.005000000 |

The direct ratio sigma_zz/epsilon_zz is 120 for kinematic, 106.666667 for
mixed, and 100 for transverse static relaxation. These are different restraint
responses, not conflicting estimates of C33. The matrix is recovered from all
four measured strain/stress columns: C = Sigma * inverse(Epsilon).

## Recovered Axial Terms

| Family | C13 | C23 | C33 | C36 | Relative full-block error |
| --- | ---: | ---: | ---: | ---: | ---: |
| periodic_kinematic | 40.000000000 | 40.000000000 | 120.000000000 | 1.083e-32 | 8.098e-33 |
| periodic_mixed | 40.000000000 | 40.000000000 | 120.000000000 | 3.009e-32 | 2.594e-14 |
| periodic_static | 40.000000000 | 40.000000000 | 120.000000000 | -1.367e-31 | 8.060e-17 |
| nonperiodic_kinematic | 40.000000000 | 40.000000000 | 120.000000000 | -4.004e-29 | 5.147e-31 |
| nonperiodic_mixed | 40.000000000 | 40.000000000 | 120.000000000 | -6.272e-30 | 2.594e-14 |
| nonperiodic_static | 40.000000000 | 40.000000000 | 120.000000000 | -8.375e-29 | 1.802e-16 |

All strain, stress, and energy outputs match the analytical solution for each
control condition. Recovered matrices are symmetric and positive definite.

## Convergence

| Family | Load | Newton iterations | Final residual |
| --- | --- | ---: | ---: |
| periodic_kinematic | xx | 1 | 1.372e-16 |
| periodic_kinematic | yy | 1 | 1.969e-16 |
| periodic_kinematic | zz | 1 | 7.635e-17 |
| periodic_kinematic | xy | 1 | 7.570e-17 |
| periodic_mixed | xx | 1 | 1.466e-16 |
| periodic_mixed | yy | 1 | 1.590e-16 |
| periodic_mixed | zz | 1 | 7.156e-17 |
| periodic_mixed | xy | 1 | 7.570e-17 |
| periodic_static | xx | 1 | 1.826e-16 |
| periodic_static | yy | 1 | 1.878e-16 |
| periodic_static | zz | 1 | 5.596e-17 |
| periodic_static | xy | 1 | 2.205e-16 |
| nonperiodic_kinematic | xx | 1 | 4.341e-16 |
| nonperiodic_kinematic | yy | 1 | 8.398e-16 |
| nonperiodic_kinematic | zz | 0 | 5.637e-17 |
| nonperiodic_kinematic | xy | 1 | 3.838e-16 |
| nonperiodic_mixed | xx | 1 | 5.753e-16 |
| nonperiodic_mixed | yy | 1 | 6.666e-16 |
| nonperiodic_mixed | zz | 1 | 3.196e-16 |
| nonperiodic_mixed | xy | 1 | 4.561e-16 |
| nonperiodic_static | xx | 1 | 6.038e-16 |
| nonperiodic_static | yy | 1 | 8.422e-16 |
| nonperiodic_static | zz | 1 | 2.529e-16 |
| nonperiodic_static | xy | 1 | 1.698e-15 |

## Reproduce

Activate the MOOSE environment, then run:

```bash
python run_validation.py
```

Set RUN_CASES=False to rebuild the report from existing outputs.
CSV, Exodus, and logs remain in each BC family's subdirectory.
