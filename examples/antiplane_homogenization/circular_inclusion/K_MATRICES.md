# Circular-Inclusion Antiplane Homogenization

## Problem

The geometry is the same unit-square SVE and conforming circular inclusion used by the in-plane
study. The inclusion radius is $r=\sqrt{0.25/\pi}$, giving a 25% area fraction. The matrix and
inclusion shear moduli are $G_m=40$ and $G_i=400$.

For antiplane displacement $u_z=w(x,y)$,

\[
\begin{bmatrix}\bar\tau_{xz}\\\bar\tau_{yz}\end{bmatrix}
=\mathbf K^{\mathrm{eff}}
\begin{bmatrix}\bar\gamma_{xz}\\\bar\gamma_{yz}\end{bmatrix},
\qquad
\gamma_{xz}=w_{,x},\quad \gamma_{yz}=w_{,y}.
\]

The `xz.i` and `yz.i` inputs provide the independent columns. Static cases first generate the
effective compliance matrix, which is inverted to obtain $\mathbf K^{\mathrm{eff}}$.

## Effective matrices

The periodic formulations give

\[
\mathbf K^{\mathrm{PK}}=\mathbf K^{\mathrm{PM}}=\mathbf K^{\mathrm{PS}}=
\begin{bmatrix}
60.635612 & 0\\
0 & 60.635612
\end{bmatrix}.
\]

The nonperiodic matrices are

\[
\mathbf K^{\mathrm{NK}}=
\begin{bmatrix}61.124284&0\\0&61.124284\end{bmatrix},\qquad
\mathbf K^{\mathrm{NM}}=
\begin{bmatrix}60.635612&0\\0&60.635612\end{bmatrix},
\]

\[
\mathbf K^{\mathrm{NS}}=
\begin{bmatrix}60.134851&0\\0&60.134851\end{bmatrix}.
\]

## Shear-modulus comparison

For this fourfold-symmetric SVE, $K_{xz,xz}=G_{xz}$ and $K_{yz,yz}=G_{yz}$.
Antiplane deformation contains no volumetric strain, so it cannot determine a bulk modulus.

The two-dimensional circular-inclusion Mori-Tanaka result is

\[
G_{\mathrm{MT}}=G_m
\frac{G_i+G_m+c_i(G_i-G_m)}{G_i+G_m-c_i(G_i-G_m)}
=60.571429.
\]

| Method | $G_{xz}$ | $G_{yz}$ | Difference from MT |
|---|---:|---:|---:|
| Matrix phase | 40.000000 | 40.000000 | -33.962% |
| Inclusion phase | 400.000000 | 400.000000 | 560.377% |
| Mori-Tanaka | 60.571429 | 60.571429 | 0% |
| Periodic kinematic | 60.635612 | 60.635612 | 0.106% |
| Periodic mixed | 60.635612 | 60.635612 | 0.106% |
| Periodic static | 60.635612 | 60.635612 | 0.106% |
| Nonperiodic kinematic | 61.124284 | 61.124284 | 0.913% |
| Nonperiodic mixed | 60.635612 | 60.635612 | 0.106% |
| Nonperiodic static | 60.134851 | 60.134851 | -0.721% |

The finite-SVE ordering is

\[
\mathbf K^{\mathrm{NK}}\succeq
\mathbf K^{\mathrm{periodic}}=
\mathbf K^{\mathrm{NM}}\succeq
\mathbf K^{\mathrm{NS}}.
\]

The periodic result lies close to Mori-Tanaka because both describe a circular-inclusion problem,
although the periodic cell and mean-field models are not identical.

