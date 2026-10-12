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
| xz | 0.0030504983 | 0.0030528762 | 0.077949% |
| yz | 0.0031049102 | 0.0031112995 | 0.205783% |

| Mesh | Case | Newton iterations | Maximum Krylov iteration index | Final residual | Energy consistency mismatch |
| --- | --- | ---: | ---: | ---: | ---: |
| concentric | xx | 1 | 2 | 2.172e-14 | 1.322e-13 |
| concentric | yy | 1 | 2 | 5.334e-14 | 1.640e-13 |
| concentric | xy | 1 | 2 | 2.462e-14 | 1.471e-13 |
| concentric | xz | 1 | 2 | 8.521e-15 | 1.585e-13 |
| concentric | yz | 1 | 2 | 1.443e-14 | 1.307e-13 |
| structured | xx | 1 | 2 | 9.778e-15 | 1.987e-14 |
| structured | yy | 1 | 2 | 1.077e-14 | 2.002e-14 |
| structured | xy | 1 | 2 | 9.300e-15 | 1.992e-14 |
| structured | xz | 1 | 2 | 3.753e-15 | 3.551e-15 |
| structured | yz | 1 | 2 | 4.181e-15 | 1.227e-14 |

## Matrix Checks

| Mesh | Matrix | Relative symmetry error | Eigenvalues |
| --- | --- | ---: | --- |
| concentric | C | 2.961e-15 | 57.358426, 116.230505, 227.445616 |
| concentric | K | 1.813e-16 | 60.983981, 62.124189 |
| structured | C | 5.366e-15 | 57.026121, 116.665521, 227.761747 |
| structured | K | 5.403e-18 | 61.030705, 62.252809 |

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
