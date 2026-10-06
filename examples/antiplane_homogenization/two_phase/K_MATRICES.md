# Two-Phase Antiplane Homogenization Matrices

The matrices in this file were reproduced from the refreshed CSV outputs for all 12 antiplane load
cases. Every solve converged, and each effective matrix was reconstructed from the final
volume-averaged shear strain and stress values.

## Problem

The unit-square SVE contains a centered square inclusion occupying 25% of the area. The phase
properties are

| Phase | Shear modulus | Volume fraction |
|---|---:|---:|
| Matrix | 40 | 0.75 |
| Inclusion | 400 | 0.25 |

For antiplane displacement $u_z=w(x,y)$,

\[
\begin{bmatrix}
\bar\tau_{xz}\\
\bar\tau_{yz}
\end{bmatrix}
=
\mathbf K^{\mathrm{eff}}
\begin{bmatrix}
\bar\gamma_{xz}\\
\bar\gamma_{yz}
\end{bmatrix},
\qquad
\gamma_{xz}=w_{,x},
\quad
\gamma_{yz}=w_{,y}.
\]

The `xz.i` and `yz.i` cases provide the two independent loading directions. For kinematic and
mixed loading, the relevant matrix column is obtained from average stress divided by the imposed
average engineering shear. For static loading, the two simulations first produce the compliance
matrix, which is then inverted.

Numerically negligible coupling terms are reported as zero. The equality of the two diagonal
entries follows from the square geometry and centered square inclusion.

## Periodic results

### Periodic kinematic

\[
\mathbf K^{\mathrm{PK}}=
\begin{bmatrix}
61.990661 & 0\\
0 & 61.990661
\end{bmatrix}.
\]

### Periodic mixed

\[
\mathbf K^{\mathrm{PM}}=
\begin{bmatrix}
61.990661 & 0\\
0 & 61.990661
\end{bmatrix}.
\]

### Periodic static

\[
\mathbf K^{\mathrm{PS}}=
\begin{bmatrix}
61.990661 & 0\\
0 & 61.990661
\end{bmatrix}.
\]

All three periodic formulations recover the same effective matrix to numerical precision.

## Nonperiodic results

### Nonperiodic kinematic

\[
\mathbf K^{\mathrm{NK}}=
\begin{bmatrix}
62.726550 & 0\\
0 & 62.726550
\end{bmatrix}.
\]

### Nonperiodic mixed

\[
\mathbf K^{\mathrm{NM}}=
\begin{bmatrix}
61.990661 & 0\\
0 & 61.990661
\end{bmatrix}.
\]

### Nonperiodic static

\[
\mathbf K^{\mathrm{NS}}=
\begin{bmatrix}
61.723981 & 0\\
0 & 61.723981
\end{bmatrix}.
\]

## Summary

| Boundary condition | $K_{xz,xz}$ | $K_{yz,yz}$ | Coupling |
|---|---:|---:|---:|
| Periodic kinematic | 61.990661 | 61.990661 | approximately 0 |
| Periodic mixed | 61.990661 | 61.990661 | approximately 0 |
| Periodic static | 61.990661 | 61.990661 | approximately 0 |
| Nonperiodic kinematic | 62.726550 | 62.726550 | approximately 0 |
| Nonperiodic mixed | 61.990661 | 61.990661 | approximately 0 |
| Nonperiodic static | 61.723981 | 61.723981 | approximately 0 |

The finite-SVE stiffness ordering is

\[
\mathbf K^{\mathrm{NK}}
\succeq
\mathbf K^{\mathrm{periodic}}
=
\mathbf K^{\mathrm{NM}}
\succeq
\mathbf K^{\mathrm{NS}}.
\]

This is the expected ordering: the complete affine kinematic boundary is the most restrictive,
while uniform static traction is the least restrictive. The mixed result coincides with the
periodic result for this aligned, symmetric microstructure.

The averaged elastic energy also satisfies

\[
\bar W=\frac{1}{2}\bar{\boldsymbol\gamma}\cdot\bar{\boldsymbol\tau}
\]

for every case to the reported numerical precision.

## Antiplane stiffness and shear-modulus comparison

The symbol \(\mathbf K^{\mathrm{eff}}\) in this file denotes the antiplane stiffness matrix; it
does **not** denote a bulk modulus. For the present symmetric microstructure,

\[
\mathbf K^{\mathrm{eff}}=
\begin{bmatrix}
G_{xz} & 0\\
0 & G_{yz}
\end{bmatrix}.
\]

| Material or method | $G_{xz}$ | $G_{yz}$ |
|---|---:|---:|
| Matrix phase | 40.000000 | 40.000000 |
| Inclusion phase | 400.000000 | 400.000000 |
| Periodic kinematic | 61.990661 | 61.990661 |
| Periodic mixed | 61.990661 | 61.990661 |
| Periodic static | 61.990661 | 61.990661 |
| Nonperiodic kinematic | 62.726550 | 62.726550 |
| Nonperiodic mixed | 61.990661 | 61.990661 |
| Nonperiodic static | 61.723981 | 61.723981 |

Antiplane deformation has no in-plane volumetric strain, so it cannot determine a bulk modulus.
The bulk information comes from the in-plane homogenization cases. A direct comparison of the two
studies is

| Method | In-plane $K^{2D}$ | In-plane $G_{xy}$ | Antiplane $G_{xz}=G_{yz}$ |
|---|---:|---:|---:|
| Periodic kinematic | 114.087132 | 54.221668 | 61.990661 |
| Periodic mixed | 114.087132 | 54.221668 | 61.990661 |
| Periodic static | 114.087132 | 54.221668 | 61.990661 |
| Nonperiodic kinematic | 114.514747 | 58.239285 | 62.726550 |
| Nonperiodic mixed | 114.923724 | 54.221668 | 61.990661 |
| Nonperiodic static | 113.925239 | 53.232150 | 61.723981 |

The difference between $G_{xy}$ and $G_{xz}=G_{yz}$ is physically admissible for an extruded
two-dimensional inclusion pattern: its effective three-dimensional symmetry is transverse
isotropy, not general three-dimensional isotropy.
