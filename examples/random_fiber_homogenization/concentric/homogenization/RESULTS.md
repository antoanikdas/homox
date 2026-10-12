# Periodic Random-Fiber Homogenization

All five cases use the same saved `../mesh_in.e`: 22 radius-0.06 fibers,
a unit-square domain, 13,710 TRI3 elements, and fitted periodic interfaces.
Fiber fraction: **24.88141382%**.

Matrix: E=100, nu=0.25, G=40. Fiber: E=1000, nu=0.25, G=400.
Stiffness and stress units are the units chosen for E; strains are dimensionless.

## Formulation

The in-plane calculation is **plane strain**, with epsilon_zz=epsilon_xz=epsilon_yz=0.
Displacements are an affine macro part plus periodic fluctuations; average strains
are imposed through the Homox scalar homogenization equations. One corner fixes
the fluctuation translation. The antiplane calculation solves a periodic disp_z
fluctuation with two prescribed macro gradients. Both use NEWTON and SuperLU_DIST.

## Effective Matrices

In-plane order: stress=(sigma_xx, sigma_yy, sigma_xy),
strain=(epsilon_xx, epsilon_yy, gamma_xy), with gamma_xy=2*epsilon_xy.
Normal loading amplitude is 0.01; shear loading uses epsilon_xy=0.005.
Measured columns satisfy C_eff = stress_columns * inverse(strain_columns).

```text
[  170.853604891   55.598811945  -0.079643267 ]
[  55.598811945   172.821874039  -0.266473196 ]
[ -0.079643267  -0.266473196   57.359067868 ]
```

Antiplane order: stress=(tau_xz, tau_yz), strain=(gamma_xz, gamma_yz).
Each prescribed engineering-shear amplitude is 0.01. K_eff is reconstructed
from the two measured stress and gradient columns.

```text
[  61.009966534  -0.170159174 ]
[ -0.170159174   62.098203063 ]
```

These are reduced in-plane and antiplane matrices, not a full 3D 6-by-6 tensor.
A finite random realization need not be isotropic: normal-shear and antiplane
cross-couplings are retained rather than forced to zero.

## Load Results

| Case | Average stress components | Average energy density | Relative energy mismatch |
| --- | --- | ---: | ---: |
| xx | 1.708536049, 0.555988119, -0.000796433 | 0.0085426802 | 1.322e-13 |
| yy | 0.555988119, 1.728218740, -0.002664732 | 0.0086410937 | 1.640e-13 |
| xy | -0.000796433, -0.002664732, 0.573590679 | 0.0028679534 | 1.471e-13 |
| xz | 0.610099665, -0.001701592 | 0.0030504983 | 1.585e-13 |
| yz | -0.001701592, 0.620982031 | 0.0031049102 | 1.307e-13 |

Energy is integrated directly from the AD elastic material property and divided
by domain area. The comparison is with 0.5 * average_stress : average_strain
(or 0.5 * average_traction dot average_gradient for antiplane).

## Verification

Relative in-plane symmetry error: 2.961e-15.
Relative antiplane symmetry error: 1.813e-16.
Both symmetric matrices have positive eigenvalues.
Minimum eigenvalue of C_eff - C_Reuss: 5.811404608.
Minimum eigenvalue of C_Voigt - C_eff: 72.213496887.

| Case | Newton iterations | Maximum Krylov iteration index per solve | Final nonlinear residual |
| --- | ---: | ---: | ---: |
| xx | 1 | 2 | 2.172e-14 |
| yy | 1 | 2 | 5.334e-14 |
| xy | 1 | 2 | 2.462e-14 |
| xz | 1 | 2 | 8.521e-15 |
| yz | 1 | 2 | 1.443e-14 |

## Reproduce

From this directory, with the MOOSE environment activated:

```bash
python run_homogenization.py
```

The script runs inputs sequentially and saves CSV, Exodus, and logs in the
`inplane` and `antiplane` directories. Set `RUN_CASES = False` to reconstruct
this report from existing results without repeating the solves.
