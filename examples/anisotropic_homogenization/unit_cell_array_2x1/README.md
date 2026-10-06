# Two-Unit-Cell Anisotropic Homogenization

The results in this file were reproduced from the refreshed CSV outputs for all 30 in-plane and
antiplane load cases. Every solve converged, and each effective matrix was reconstructed from the
final volume-averaged strain and stress values.

## Geometry

This study contains two identical circular-inclusion unit cells arranged as a $2\times1$ array:

```text
[unit_cells]
  type = HomoxMultiInclusionMeshGenerator
  radius = 0.2820947918
  n_left = 0
  n_right = 1
  n_top = 0
  n_bottom = 0
  num_sectors = 8
  inclusion_rings = 6
  matrix_rings = 8
[]
```

The assembled cell array is translated by $(0.5,0.5)$, giving the physical limits

\[
x_{\min}=0,\qquad x_{\max}=2,\qquad
y_{\min}=0,\qquad y_{\max}=1.
\]

The complete domain therefore has width two and height one, with inclusion centers at $(0.5,0.5)$
and $(1.5,0.5)$. Each unit cell contains a 25% circular inclusion. The anisotropic phase
properties are identical to the one-cell and $2\times2$ studies.

The x-directed mixed displacement jumps are doubled relative to a single cell because the domain
width is two. The y-directed and shear jumps retain the one-cell scaling because the domain height
is one.

## In-plane results

All periodic formulations recover

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
| Nonperiodic kinematic | 251.107409 | 154.445969 | 58.498228 | 10.962618 | -7.424148 | 74.462205 |
| Nonperiodic mixed | 249.897178 | 153.683080 | 59.954392 | 11.235804 | -7.541534 | 73.292315 |
| Nonperiodic static | 247.144425 | 151.555009 | 61.068115 | 11.764396 | -7.980947 | 72.804760 |

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
| Nonperiodic kinematic | 89.946982 | 8.743411 | 58.650129 |
| Nonperiodic mixed | 89.877362 | 8.772623 | 58.489271 |
| Nonperiodic static | 89.521061 | 8.857430 | 58.337549 |

## Cell-count comparison

The $2\times1$ periodic tensors match the one-cell circular tensors to numerical precision:

| Comparison | Maximum component difference |
|---|---:|
| In-plane $2\times1$ versus one cell | $4.97\times10^{-12}$ |
| Antiplane $2\times1$ versus one cell | $8.31\times10^{-13}$ |

The nonperiodic boundary-condition spread is measured by the Frobenius norm of the
kinematic-minus-static stiffness difference:

| Domain | Number of cells | In-plane spread | Antiplane spread |
|---|---:|---:|---:|
| $1\times1$ | 1 | 9.711487 | 0.943601 |
| $2\times1$ | 2 | 6.474905 | 0.552373 |
| $2\times2$ | 4 | 4.748153 | 0.471893 |

Relative to one cell, the $2\times1$ domain reduces the spread by 33.3% in-plane and 41.5%
antiplane. Enlarging both directions in the $2\times2$ case reduces it further.

The expected stiffness bounds remain positive definite. For in-plane loading, the eigenvalues of
$\mathbf C^{\mathrm{NK}}-\mathbf C^{\mathrm{periodic}}$ are
$(0.3421,0.4858,1.2036)$ and those of
$\mathbf C^{\mathrm{periodic}}-\mathbf C^{\mathrm{NS}}$ are $(0.3078,0.3570,5.8151)$.
The corresponding antiplane eigenvalues are $(0.0638,0.2112)$ and $(0.0334,0.4301)$.
