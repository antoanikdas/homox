# Multiple-Inclusion Homogenization

`HomoxMultiInclusionMeshGenerator` creates a rectangular array of unit-square cells. Every cell
contains one centered, conforming circular inclusion. The central cell remains centered at the
coordinate origin.

```text
[Mesh]
  [multi_inclusion]
    type = HomoxMultiInclusionMeshGenerator
    radius = 0.2
    n_left = 1
    n_right = 1
    n_top = 1
    n_bottom = 1
  []
[]
```

The example above produces a $3\times3$ array. In general,

\[
N_x=n_{\mathrm{left}}+1+n_{\mathrm{right}},\qquad
N_y=n_{\mathrm{bottom}}+1+n_{\mathrm{top}}.
\]

The bounds are

\[
x\in[-n_{\mathrm{left}}-0.5,\ n_{\mathrm{right}}+0.5],\qquad
y\in[-n_{\mathrm{bottom}}-0.5,\ n_{\mathrm{top}}+0.5].
\]

The generator provides `inclusion` and `matrix` blocks, the outer `left`, `right`, `bottom`, and
`top` boundaries, and the `bottom_left` and `bottom_right` node sets. The optional
`num_sectors`, `inclusion_rings`, and `matrix_rings` parameters control mesh resolution.

The `inplane` directory contains `xx`, `yy`, and `xy` periodic homogenization cases. The
`antiplane` directory contains the corresponding `xz` and `yz` cases. Change the geometry once in
the relevant `common.i`; every load case then uses the same generated mesh.

## Verified 3x3 result

For the supplied radius $r=0.2$, the inclusion fraction in every cell is
$c_i=\pi r^2=0.125664$. The periodic results are

\[
\mathbf C^{\mathrm{eff}}=
\begin{bmatrix}
142.459039 & 46.566841 & 0\\
46.566841 & 142.459039 & 0\\
0 & 0 & 46.738138
\end{bmatrix},
\qquad
\mathbf K^{\mathrm{eff}}=
\begin{bmatrix}
49.196121 & 0\\
0 & 49.196121
\end{bmatrix}.
\]

Here the in-plane matrix uses engineering shear, so $C_{66}=G_{xy}$. The $3\times3$ array and a
single-cell run give the same reported components to machine precision, as expected for identical
periodically repeated cells. An asymmetric $3\times2$ array with `n_left = 2`, `n_right = 0`,
`n_top = 1`, and `n_bottom = 0` was also run successfully.

