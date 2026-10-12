# Concentric vs Structured Homogenization

The same 22 periodic circular fibers (radius 0.06, seed 1980) are represented
by a fitted all-TRI3 mesh and a structured 128 x 128 all-QUAD4 mesh.
The exported fiber centers were checked for exact equality before the solves.

Matrix E=100, fiber E=1000, nu=0.25 for both phases. Consequently G_matrix=40
and G_fiber=400. Stiffness units follow E; strains are dimensionless.
All cases use periodic kinematic constraints, NEWTON, and SuperLU_DIST.

The in-plane problem is **plane strain**. The order is stress=(xx, yy, xy)
and strain=(epsilon_xx, epsilon_yy, gamma_xy), with gamma_xy=2*epsilon_xy.
Normal loads use strain 0.01; the xy load uses epsilon_xy=0.005.
Antiplane uses (tau_xz, tau_yz) and engineering strains (gamma_xz, gamma_yz),
each loaded to 0.01. Both matrices are reconstructed from measured stress
and strain columns without suppressing normal-shear or cross-shear coupling.

## Geometry

| Mesh | Elements | Fiber fraction |
| --- | ---: | ---: |
| Concentric TRI3 | 13,710 | 24.88141382% |
| Structured QUAD4 | 16,384 | 24.81079102% |

The ideal circles coincide, but centroid assignment on the structured grid
approximates phase area. The fitted mesh preserves nominal disk area.

## Concentric Matrices

C_eff (plane strain):

```text
[  170.853604891   55.598811945  -0.079643267 ]
[  55.598811945   172.821874039  -0.266473196 ]
[ -0.079643267  -0.266473196   57.359067868 ]
```

K_eff (antiplane):

```text
[  61.009966534  -0.170159174 ]
[ -0.170159174   62.098203063 ]
```

## Structured Matrices

C_eff (plane strain):

```text
[  171.146246519   55.537757970  -0.106661305 ]
[  55.537757970   173.280433106  -0.260753663 ]
[ -0.106661305  -0.260753663   57.026709671 ]
```

K_eff (antiplane):

```text
[  61.057523432  -0.179041577 ]
[ -0.179041577   62.225990597 ]
```

## Differences

Differences below are structured minus concentric; the fitted mesh is the
comparison reference, not an exact continuum solution.
Relative Frobenius difference in C_eff: **0.246188%**.
Relative Frobenius difference in K_eff: **0.157290%**.
These norms use the reported engineering-shear matrix coordinates.

| Component | Concentric | Structured | Signed difference | Relative difference |
| --- | ---: | ---: | ---: | ---: |
| C11 | 170.853604891 | 171.146246519 | +0.292641627 | 0.171282% |
| C22 | 172.821874039 | 173.280433106 | +0.458559068 | 0.265336% |
| C12 | 55.598811945 | 55.537757970 | -0.061053975 | 0.109812% |
| C66 | 57.359067868 | 57.026709671 | -0.332358198 | 0.579434% |
| C16 | -0.079643267 | -0.106661305 | -0.027018039 | 33.923820% |
| C26 | -0.266473196 | -0.260753663 | +0.005719533 | 2.146382% |
| K11 | 61.009966534 | 61.057523432 | +0.047556899 | 0.077949% |
| K22 | 62.098203063 | 62.225990597 | +0.127787533 | 0.205783% |
| K12 | -0.170159174 | -0.179041577 | -0.008882403 | 5.220055% |

Relative percentages for small coupling entries can be large despite a small
absolute change; the full matrices and norm differences provide context.

## Energy and Convergence

Energy is the integral of the AD elastic-energy property divided by domain area.
It is checked against half the contraction of average stress and average strain.

| Case | Concentric energy density | Structured energy density | Relative difference |
| --- | ---: | ---: | ---: |
| xx | 0.0085426802 | 0.0085573123 | 0.171282% |
| yy | 0.0086410937 | 0.0086640217 | 0.265336% |
| xy | 0.0028679534 | 0.0028513355 | 0.579434% |
| axial_xx | 0.0085426802 | 0.0085573123 | 0.171282% |
| axial_yy | 0.0086410937 | 0.0086640217 | 0.265336% |
| axial_xy | 0.0028679534 | 0.0028513355 | 0.579434% |
| axial_zz | 0.0176181147 | 0.0175883003 | 0.169226% |
| xz | 0.0030504983 | 0.0030528762 | 0.077949% |
| yz | 0.0031049102 | 0.0031112995 | 0.205783% |

| Mesh | Case | Newton iterations | Maximum Krylov iteration index | Final residual | Energy consistency mismatch |
| --- | --- | ---: | ---: | ---: | ---: |
| concentric | xx | 1 | 2 | 2.172e-14 | 1.322e-13 |
| concentric | yy | 1 | 2 | 5.334e-14 | 1.640e-13 |
| concentric | xy | 1 | 2 | 2.462e-14 | 1.471e-13 |
| concentric | axial_xx | 1 | 2 | 2.191e-14 | 1.322e-13 |
| concentric | axial_yy | 1 | 2 | 3.651e-14 | 1.640e-13 |
| concentric | axial_xy | 1 | 2 | 2.103e-14 | 1.647e-13 |
| concentric | axial_zz | 1 | 2 | 1.399e-14 | 3.525e-14 |
| concentric | xz | 1 | 2 | 8.521e-15 | 1.585e-13 |
| concentric | yz | 1 | 2 | 1.443e-14 | 1.307e-13 |
| structured | xx | 1 | 2 | 9.778e-15 | 1.987e-14 |
| structured | yy | 1 | 2 | 1.077e-14 | 2.002e-14 |
| structured | xy | 1 | 2 | 9.300e-15 | 1.992e-14 |
| structured | axial_xx | 1 | 2 | 9.864e-15 | 1.987e-14 |
| structured | axial_yy | 1 | 2 | 1.077e-14 | 2.002e-14 |
| structured | axial_xy | 1 | 2 | 9.577e-15 | 1.992e-14 |
| structured | axial_zz | 1 | 2 | 4.071e-15 | 6.904e-15 |
| structured | xz | 1 | 2 | 3.753e-15 | 3.551e-15 |
| structured | yz | 1 | 2 | 4.181e-15 | 1.227e-14 |

## Matrix Checks

| Mesh | Matrix | Relative symmetry error | Eigenvalues |
| --- | --- | ---: | --- |
| concentric | C | 2.961e-15 | 57.358426, 116.230505, 227.445616 |
| concentric | K | 1.813e-16 | 60.983981, 62.124189 |
| concentric | full | 2.603e-14 | 57.358422, 60.983981, 62.124189, 116.229761, 188.086030, 391.722626 |
| structured | C | 5.366e-15 | 57.026121, 116.665521, 227.761747 |
| structured | K | 5.403e-18 | 61.030705, 62.252809 |
| structured | full | 1.026e-14 | 57.026118, 61.030705, 62.252809, 116.664632, 188.137159, 391.391487 |

All symmetry, positive-definiteness, energy-consistency, and Voigt-Reuss
bound checks passed. Bounds use each mesh's actually represented fiber fraction.

## Reproduce

With the MOOSE environment activated, from this directory:

```bash
python run_comparison.py
```

Results, Exodus files, and solver logs are separated under `results/concentric`
and `results/structured`. The script uses the saved meshes one directory above.
Set `RUN_CASES = False` to rebuild this report from existing results.

## Axial Extension and Full 3D Stiffness

The `axial` inputs activate four macro-strain scalars (xx, xy, yy, zz).
For axial_zz, epsilon_zz=0.01 and all other average strains are zero.
For axial_xx, axial_yy, and axial_xy, epsilon_zz remains zero while sigma_zz
is measured. This independently verifies reciprocal axial couplings.
The 2D material adds uniform epsilon_zz to its full 3D strain tensor;
in-plane periodic fluctuations are still solved on the cross-sectional mesh.

The assembled full tensor uses stress=(xx, yy, zz, yz, xz, xy) and
engineering strain=(epsilon_xx, epsilon_yy, epsilon_zz, gamma_yz, gamma_xz, gamma_xy).
Its 4-by-4 normal/in-plane-shear block is measured from four independent solves.
The 2-by-2 antiplane block is measured from xz and yz. Cross-block entries
are zero by z-reflection symmetry for straight, z-invariant isotropic phases.
This assembly is not a general recovery method for arbitrarily anisotropic
phases with normal/antiplane coupling or a geometry varying along z.

Full C_eff, concentric:

```text
[  170.853604891   55.598811945   56.613104209   0.000000000   0.000000000  -0.079643267 ]
[  55.598811945   172.821874039   57.105171496   0.000000000   0.000000000  -0.266473196 ]
[  56.613104209   57.105171496   352.362293274   0.000000000   0.000000000  -0.086529116 ]
[  0.000000000   0.000000000   0.000000000   62.098203063  -0.170159174   0.000000000 ]
[  0.000000000   0.000000000   0.000000000  -0.170159174   61.009966534   0.000000000 ]
[ -0.079643267  -0.266473196  -0.086529116   0.000000000   0.000000000   57.359067868 ]
```

Full C_eff, structured:

```text
[  171.146246519   55.537757970   56.671001122   0.000000000   0.000000000  -0.106661305 ]
[  55.537757970   173.280433106   57.204547769   0.000000000   0.000000000  -0.260753663 ]
[  56.671001122   57.204547769   351.766006363   0.000000000   0.000000000  -0.091853742 ]
[  0.000000000   0.000000000   0.000000000   62.225990597  -0.179041577   0.000000000 ]
[  0.000000000   0.000000000   0.000000000  -0.179041577   61.057523432   0.000000000 ]
[ -0.106661305  -0.260753663  -0.091853742   0.000000000   0.000000000   57.026709671 ]
```

Relative full-matrix Frobenius difference: **0.195658%**.

| Axial component | Concentric | Structured | Relative difference |
| --- | ---: | ---: | ---: |
| C13 | 56.613104209 | 56.671001122 | 0.102268% |
| C23 | 57.105171496 | 57.204547769 | 0.174023% |
| C33 | 352.362293274 | 351.766006363 | 0.169226% |
| C36 | -0.086529116 | -0.091853742 | 6.153566% |

The full matrices are symmetric to numerical precision and positive definite.
Their in-plane submatrices reproduce the original plane-strain matrices.
Full 3D Voigt-Reuss bounds and energy consistency, including sigma_zz*epsilon_zz,
were checked. The tensor has 13 potentially independent entries under this
symmetry; equal phase Poisson ratios introduce additional relations.
