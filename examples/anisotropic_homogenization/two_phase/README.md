# Two-Phase Anisotropic Homogenization

## Problem

The unit-square SVE contains a centered square inclusion occupying 25% of the area. Both phases
are anisotropic and contain coupling terms. The mesh has 256 equal-area quadrilateral elements:
192 matrix elements and 64 inclusion elements.

The engineering-shear in-plane phase matrices are

\[
\mathbf C_m=
\begin{bmatrix}
200&50&12\\
50&120&-8\\
12&-8&60
\end{bmatrix},
\qquad
\mathbf C_i=
\begin{bmatrix}
600&100&-20\\
100&400&15\\
-20&15&180
\end{bmatrix}.
\]

The corresponding antiplane blocks, taken from the same full phase tensors, are

\[
\mathbf K_m=
\begin{bmatrix}70&10\\10&45\end{bmatrix},
\qquad
\mathbf K_i=
\begin{bmatrix}220&-12\\-12&140\end{bmatrix}.
\]

All four matrices are positive definite. Notice that the inclusion has opposite-sign coupling in
both the in-plane normal-shear components and the antiplane off-diagonal component.

## Extraction

For each boundary method, the independent simulations are assembled column-wise into average
strain and stress matrices. The effective tensors are recovered from

\[
\mathbf C^{\mathrm{eff}}=\boldsymbol\Sigma\mathbf E^{-1},
\qquad
\mathbf K^{\mathrm{eff}}=\mathbf T\boldsymbol\Gamma^{-1}.
\]

This extraction uses the measured average strain from every simulation. It therefore remains
valid for mixed loading, where anisotropic coupling generates additional unconstrained strain
components.

## In-plane results

The three periodic formulations give the same result:

\[
\mathbf C^{\mathrm{periodic}}=
\begin{bmatrix}
252.564362&57.577478&10.969277\\
57.577478&155.311598&-7.339823\\
10.969277&-7.339823&73.175749
\end{bmatrix}.
\]

The nonperiodic results are

\[
\mathbf C^{\mathrm{NK}}=
\begin{bmatrix}
253.285363&57.691084&10.819392\\
57.691084&155.732634&-7.120314\\
10.819392&-7.120314&74.952252
\end{bmatrix},
\]

\[
\mathbf C^{\mathrm{NM}}=
\begin{bmatrix}
252.002710&59.479976&10.969593\\
59.479976&153.929620&-7.508044\\
10.969593&-7.508044&73.188837
\end{bmatrix},
\]

\[
\mathbf C^{\mathrm{NS}}=
\begin{bmatrix}
246.998787&61.602760&11.925655\\
61.602760&152.226550&-8.035104\\
11.925655&-8.035104&72.498914
\end{bmatrix}.
\]

| Method | $C_{11}$ | $C_{22}$ | $C_{12}$ | $C_{16}$ | $C_{26}$ | $C_{66}$ |
|---|---:|---:|---:|---:|---:|---:|
| Periodic kinematic | 252.564362 | 155.311598 | 57.577478 | 10.969277 | -7.339823 | 73.175749 |
| Periodic mixed | 252.564362 | 155.311598 | 57.577478 | 10.969277 | -7.339823 | 73.175749 |
| Periodic static | 252.564362 | 155.311598 | 57.577478 | 10.969277 | -7.339823 | 73.175749 |
| Nonperiodic kinematic | 253.285363 | 155.732634 | 57.691084 | 10.819392 | -7.120314 | 74.952252 |
| Nonperiodic mixed | 252.002710 | 153.929620 | 59.479976 | 10.969593 | -7.508044 | 73.188837 |
| Nonperiodic static | 246.998787 | 152.226550 | 61.602760 | 11.925655 | -8.035104 | 72.498914 |

## Antiplane results

The periodic antiplane result is

\[
\mathbf K^{\mathrm{periodic}}=
\begin{bmatrix}
90.523113&8.435199\\
8.435199&58.713956
\end{bmatrix}.
\]

| Method | $K_{xx}$ | $K_{xy}$ | $K_{yy}$ |
|---|---:|---:|---:|
| Periodic kinematic | 90.523113 | 8.435199 | 58.713956 |
| Periodic mixed | 90.523113 | 8.435199 | 58.713956 |
| Periodic static | 90.523113 | 8.435199 | 58.713956 |
| Nonperiodic kinematic | 90.628166 | 8.486818 | 59.148980 |
| Nonperiodic mixed | 90.573507 | 8.549944 | 58.744795 |
| Nonperiodic static | 90.055372 | 8.657822 | 58.590102 |

## Consistency checks

The maximum component difference among the three periodic formulations is
$8.42\times10^{-12}$ in-plane and $7.18\times10^{-13}$ antiplane.

The stiffness-bound differences are positive definite:

\[
\mathbf C^{\mathrm{NK}}-\mathbf C^{\mathrm{periodic}}\succ0,
\qquad
\mathbf C^{\mathrm{periodic}}-\mathbf C^{\mathrm{NS}}\succ0,
\]

with eigenvalues $(0.3370,0.7548,1.8267)$ and $(0.1131,0.5030,8.7113)$, respectively. For
antiplane loading, the corresponding eigenvalues are $(0.0972,0.4429)$ and $(0.0145,0.5771)$.
Thus both formulations retain the expected nonperiodic kinematic upper bound and static lower
bound for this anisotropic two-phase SVE.

