# Circular-Inclusion Two-Phase Anisotropic Homogenization

## Problem

The unit-square SVE contains a centered circular inclusion with radius

\[
r=\sqrt{0.25/\pi}=0.2820947918,
\]

giving the same 25% inclusion area fraction as the square-inclusion anisotropic study. A conforming
quadrilateral mesh is generated with `ConcentricCircleMeshGenerator`, using volume preservation.

The engineering-shear in-plane phase matrices are

\[
\mathbf C_m=
\begin{bmatrix}200&50&12\\50&120&-8\\12&-8&60\end{bmatrix},
\qquad
\mathbf C_i=
\begin{bmatrix}600&100&-20\\100&400&15\\-20&15&180\end{bmatrix}.
\]

The matching antiplane blocks are

\[
\mathbf K_m=\begin{bmatrix}70&10\\10&45\end{bmatrix},
\qquad
\mathbf K_i=\begin{bmatrix}220&-12\\-12&140\end{bmatrix}.
\]

All phase tensors are positive definite. Effective matrices are recovered from the measured
average fields using

\[
\mathbf C^{\mathrm{eff}}=\boldsymbol\Sigma\mathbf E^{-1},
\qquad
\mathbf K^{\mathrm{eff}}=\mathbf T\boldsymbol\Gamma^{-1}.
\]

## In-plane results

All three periodic formulations give

\[
\mathbf C^{\mathrm{periodic}}=
\begin{bmatrix}
250.674663&58.433306&11.115973\\
58.433306&154.019586&-7.461313\\
11.115973&-7.461313&73.289828
\end{bmatrix}.
\]

| Method | $C_{11}$ | $C_{22}$ | $C_{12}$ | $C_{16}$ | $C_{26}$ | $C_{66}$ |
|---|---:|---:|---:|---:|---:|---:|
| Periodic kinematic | 250.674663 | 154.019586 | 58.433306 | 11.115973 | -7.461313 | 73.289828 |
| Periodic mixed | 250.674663 | 154.019586 | 58.433306 | 11.115973 | -7.461313 | 73.289828 |
| Periodic static | 250.674663 | 154.019586 | 58.433306 | 11.115973 | -7.461313 | 73.289828 |
| Nonperiodic kinematic | 251.473022 | 154.503269 | 58.575737 | 10.985745 | -7.261959 | 75.137630 |
| Nonperiodic mixed | 250.471006 | 152.962724 | 60.600978 | 11.120126 | -7.624601 | 73.306269 |
| Nonperiodic static | 244.911568 | 150.853846 | 62.309177 | 12.065559 | -8.147671 | 72.654910 |

## Antiplane results

All periodic formulations give

\[
\mathbf K^{\mathrm{periodic}}=
\begin{bmatrix}
89.862222&8.691983\\
8.691983&58.459853
\end{bmatrix}.
\]

| Method | $K_{xx}$ | $K_{xy}$ | $K_{yy}$ |
|---|---:|---:|---:|
| Periodic kinematic | 89.862222 | 8.691983 | 58.459853 |
| Periodic mixed | 89.862222 | 8.691983 | 58.459853 |
| Periodic static | 89.862222 | 8.691983 | 58.459853 |
| Nonperiodic kinematic | 89.951985 | 8.773534 | 58.832010 |
| Nonperiodic mixed | 89.925208 | 8.819419 | 58.495398 |
| Nonperiodic static | 89.223985 | 8.966634 | 58.297387 |

## Consistency checks

The maximum difference among periodic formulations is $8.38\times10^{-12}$ in-plane and
$1.01\times10^{-12}$ antiplane. The kinematic upper and static lower bounds are positive definite:

\[
\mathbf C^{\mathrm{NK}}-\mathbf C^{\mathrm{periodic}}\succ0,
\qquad
\mathbf C^{\mathrm{periodic}}-\mathbf C^{\mathrm{NS}}\succ0.
\]

The corresponding in-plane eigenvalues are $(0.3914,0.8510,1.8874)$ and
$(0.3767,0.4652,8.7218)$. The antiplane eigenvalues are $(0.0679,0.3940)$ and
$(0.0370,0.7637)$.

## Circular versus square inclusion

The following differences use the square-inclusion periodic tensor as the reference. Constituent
properties and inclusion fraction are unchanged.

| Component | Square | Circular | Relative change |
|---|---:|---:|---:|
| $C_{11}$ | 252.564362 | 250.674663 | -0.748% |
| $C_{22}$ | 155.311598 | 154.019586 | -0.832% |
| $C_{12}$ | 57.577478 | 58.433306 | +1.486% |
| $C_{16}$ | 10.969277 | 11.115973 | +1.337% |
| $C_{26}$ | -7.339823 | -7.461313 | +1.655% in magnitude |
| $C_{66}$ | 73.175749 | 73.289828 | +0.156% |
| $K_{xx}$ | 90.523113 | 89.862222 | -0.730% |
| $K_{xy}$ | 8.435199 | 8.691983 | +3.044% |
| $K_{yy}$ | 58.713956 | 58.459853 | -0.433% |

The complete periodic matrices differ by 0.823% in relative Frobenius norm in-plane and 0.733%
in antiplane response.

