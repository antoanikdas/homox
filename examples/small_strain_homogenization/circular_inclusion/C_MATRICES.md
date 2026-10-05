# Circular-Inclusion In-Plane Homogenization

## Problem

The unit-square SVE contains a centered circular inclusion with radius

\[
r=\sqrt{0.25/\pi}=0.2820947918,
\]

so the inclusion area fraction is 0.25. `ConcentricCircleMeshGenerator` creates a conforming
quadrilateral mesh and preserves the circular area. The matrix has $E_m=100$, $\nu_m=0.25$;
the inclusion has $E_i=1000$, $\nu_i=0.25$. All cases use plane strain.

The engineering-shear Voigt convention is

\[
\begin{bmatrix}\bar\sigma_{xx}\\\bar\sigma_{yy}\\\bar\sigma_{xy}\end{bmatrix}
=\mathbf C^{\mathrm{eff}}
\begin{bmatrix}\bar\epsilon_{xx}\\\bar\epsilon_{yy}\\\bar\gamma_{xy}\end{bmatrix},
\qquad \bar\gamma_{xy}=2\bar\epsilon_{xy}.
\]

The `xx.i`, `yy.i`, and `xy.i` inputs provide the three independent load cases. Kinematic
loading directly provides stiffness columns. Static loading provides compliance columns, which
are inverted. Mixed loading combines prescribed normal extension with traction-free transverse
faces; the full normal block is reconstructed from the measured transverse strain.

## Effective matrices

The three periodic formulations agree to the reported precision:

\[
\mathbf C^{\mathrm{PK}}=\mathbf C^{\mathrm{PM}}=\mathbf C^{\mathrm{PS}}=
\begin{bmatrix}
172.188161 & 53.390251 & 0\\
53.390251 & 172.188161 & 0\\
0 & 0 & 54.275329
\end{bmatrix}.
\]

The nonperiodic results are

\[
\mathbf C^{\mathrm{NK}}=
\begin{bmatrix}
173.147136 & 53.472963 & 0\\
53.472963 & 173.147136 & 0\\
0 & 0 & 58.181083
\end{bmatrix},
\]

\[
\mathbf C^{\mathrm{NM}}=
\begin{bmatrix}
171.629332 & 57.102237 & 0\\
57.102237 & 171.629332 & 0\\
0 & 0 & 54.275329
\end{bmatrix},
\]

\[
\mathbf C^{\mathrm{NS}}=
\begin{bmatrix}
165.203502 & 59.106921 & 0\\
59.106921 & 165.203502 & 0\\
0 & 0 & 53.432466
\end{bmatrix}.
\]

## Bulk and shear comparison

For the in-plane plane-strain matrix,

\[
K^{2D}=\frac{C_{11}+C_{12}}{2},\qquad G_{xy}=C_{66}.
\]

| Method | $C_{11}$ | $C_{12}$ | $K^{2D}$ | $G_{xy}$ |
|---|---:|---:|---:|---:|
| Matrix phase | 120.000000 | 40.000000 | 80.000000 | 40.000000 |
| Inclusion phase | 1200.000000 | 400.000000 | 800.000000 | 400.000000 |
| Mori-Tanaka | 166.936231 | 52.445213 | 109.690722 | 57.245509 |
| Periodic kinematic | 172.188161 | 53.390251 | 112.789206 | 54.275329 |
| Periodic mixed | 172.188161 | 53.390251 | 112.789206 | 54.275329 |
| Periodic static | 172.188161 | 53.390251 | 112.789206 | 54.275329 |
| Nonperiodic kinematic | 173.147136 | 53.472963 | 113.310049 | 58.181083 |
| Nonperiodic mixed | 171.629332 | 57.102237 | 114.365784 | 54.275329 |
| Nonperiodic static | 165.203502 | 59.106921 | 112.155211 | 53.432466 |

Mori-Tanaka represents an isolated circular cylindrical inclusion in a matrix mean field. The
numerical SVE instead represents a finite or periodically repeated circular-inclusion cell, so
exact equality is not expected. The periodic tensor differs from the Mori-Tanaka tensor by
3.193% in Frobenius norm.

