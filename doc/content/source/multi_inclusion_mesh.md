# Multiple-Inclusion Mesh

`HomoxMultiInclusionMeshGenerator` constructs a rectangular array of unit-square cells with one
centered circular inclusion per cell. It uses framework mesh generators internally, so it does not
require a physics module.

The central cell is centered at $(0,0)$. The directional cell counts determine the number of rows
and columns:

\[
N_x=n_{\mathrm{left}}+1+n_{\mathrm{right}},\qquad
N_y=n_{\mathrm{bottom}}+1+n_{\mathrm{top}}.
\]

The radius must satisfy $0<r<0.5$. The inclusion area fraction in every unit cell is $\pi r^2$.
The circular area is preserved by the conforming quadrilateral mesh.

```text
[Mesh]
  [multi_inclusion]
    type = HomoxMultiInclusionMeshGenerator
    radius = 0.2
    n_left = 1
    n_right = 2
    n_top = 1
    n_bottom = 0
    num_sectors = 8
    inclusion_rings = 6
    matrix_rings = 8
  []
[]
```

The output contains blocks named `inclusion` and `matrix`; outer boundaries named `left`, `right`,
`bottom`, and `top`; and corner node sets named `bottom_left` and `bottom_right`. The corner node
sets are recomputed from the directional counts, so displacement constraints do not need new
coordinates when the array dimensions change.

Complete in-plane and antiplane examples are available in
`examples/multiple_homogenization`.

