# Four-Unit-Cell Anisotropic Homogenization

The results in this file were reproduced from the refreshed CSV outputs for all 30 in-plane and
antiplane load cases. Every solve converged, and each effective matrix was reconstructed from the
final volume-averaged strain and stress values.

## Geometry

This study contains four identical circular-inclusion unit cells arranged as a $2\times2$ array.
The mesh is generated directly with

```text
[unit_cells]
  type = HomoxMultiInclusionMeshGenerator
  radius = 0.2820947918
  n_left = 0
  n_right = 1
  n_top = 1
  n_bottom = 0
  num_sectors = 8
  inclusion_rings = 6
  matrix_rings = 8
[]
```

Each unit cell has side length one and a 25% circular-inclusion area fraction. The complete domain
has dimensions $2\times2$ and contains four inclusions. The anisotropic matrix and inclusion
properties are identical to those in the one-cell circular-inclusion study:

\[
\mathbf C_m=\begin{bmatrix}200&50&12\\50&120&-8\\12&-8&60\end{bmatrix},
\qquad
\mathbf C_i=\begin{bmatrix}600&100&-20\\100&400&15\\-20&15&180\end{bmatrix},
\]

\[
\mathbf K_m=\begin{bmatrix}70&10\\10&45\end{bmatrix},
\qquad
\mathbf K_i=\begin{bmatrix}220&-12\\-12&140\end{bmatrix}.
\]

The nonperiodic mixed displacement jumps are doubled relative to the one-cell cases because the
domain width and height are two. This preserves the same nominal applied strains.

## In-plane results

All three periodic formulations recover

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
| Nonperiodic kinematic | 251.066057 | 154.254460 | 58.487582 | 11.071145 | -7.367487 | 74.127182 |
| Nonperiodic mixed | 250.589927 | 153.459086 | 59.521854 | 11.152553 | -7.592850 | 73.283398 |
| Nonperiodic static | 247.809094 | 152.505537 | 60.293316 | 11.600767 | -7.806809 | 72.939723 |

## Antiplane results

All periodic formulations recover

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
| Nonperiodic kinematic | 89.906429 | 8.730530 | 58.645883 |
| Nonperiodic mixed | 89.880619 | 8.769058 | 58.458470 |
| Nonperiodic static | 89.541929 | 8.827953 | 58.379725 |

## Unit-cell comparison

The periodic $2\times2$ tensors match the corresponding one-cell circular tensors to numerical
precision:

| Comparison | Maximum component difference |
|---|---:|
| In-plane $2\times2$ versus one cell | $5.37\times10^{-12}$ |
| Antiplane $2\times2$ versus one cell | $1.29\times10^{-12}$ |

This invariance is expected because the $2\times2$ domain is an integer repetition of the same
periodic unit cell.

The nonperiodic boundary-condition spread becomes smaller as the represented domain grows. Using
the Frobenius norm of the kinematic-minus-static difference:

| Formulation | One cell | Four cells, $2\times2$ | Reduction |
|---|---:|---:|---:|
| In-plane | 9.711487 | 4.748153 | 51.1% |
| Antiplane | 0.943601 | 0.471893 | 50.0% |

The expected stiffness bounds remain positive definite. For the $2\times2$ in-plane case, the
eigenvalues of $\mathbf C^{\mathrm{NK}}-\mathbf C^{\mathrm{periodic}}$ are
$(0.2012,0.4079,0.8545)$, while those of
$\mathbf C^{\mathrm{periodic}}-\mathbf C^{\mathrm{NS}}$ are $(0.2104,0.2599,4.2594)$.
The corresponding antiplane eigenvalues are $(0.0344,0.1958)$ and $(0.0188,0.3816)$.
