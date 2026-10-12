# Selectable Random-Fiber Mesh

`HomoxRandomFiberRVEMeshGenerator` provides one geometry with two mesh choices:

```text
[Mesh]
  [rve]
    type = HomoxRandomFiberRVEMeshGenerator
    mesh_type = concentric
    radius = 0.06
    number_of_fibers = 22
    periodic_geometry = true
    seed = 1980
    nx = 128
    ny = 128
  []
[]
```

Change only `mesh_type` to `structured` to mesh the same fiber centers and
radii on a uniform QUAD4 grid. The concentric option is entirely TRI3 with
fitted polygonal interfaces; the structured option uses centroid phase
assignment with stair-stepped interfaces. Both use `fiber` (ID 1) and `matrix`
(ID 2), matching exterior nodes, and the `bottom_left`/`bottom_right` node sets.
Mechanical periodic boundary conditions must still be specified separately.

## Generate

From this directory:

```bash
../../../homox-opt -i mesh.i --mesh-only concentric.e
../../../homox-opt -i mesh.i --mesh-only structured.e Mesh/rve/mesh_type=structured
```

`nx` and `ny` control structured resolution. `num_sectors`, `fiber_rings`,
`matrix_rings`, and `desired_area` control concentric resolution.

Both choices use the same shared placement routine. Its spacing includes the
concentric matrix shell and polygon circumradius so the sampled geometry can
be meshed by either option. Keep radius, number of fibers, seed, bounds,
periodicity, shell thickness, spacing, buffer, and sector count fixed when
comparing representations. Changing sectors can slightly change the shell
exclusion distance and hence the sampled realization. Alternatively, specify
`fiber_centers` explicitly to fix the centers independently of the seed.

## Verify and Plot

Export the fiber centers using MOOSE's mesh metadata reporter:

```bash
../../../homox-opt -i export_geometry.i Outputs/file_base=concentric_geometry
../../../homox-opt -i export_geometry.i Mesh/rve/mesh_type=structured Outputs/file_base=structured_geometry
python compare_meshes.py
```

`compare_meshes.py` checks exact equality of the 22 sampled centers, homogeneous
element types, positive element areas, total domain area, and the structured
phase assignment against the shared periodic circles. It writes
`mesh_comparison.png`.

The verified example has:

| Representation | Element type | Elements | Fiber fraction |
| --- | --- | ---: | ---: |
| Concentric | TRI3 | 13,710 | 24.88141382% |
| Structured 128 x 128 | QUAD4 | 16,384 | 24.81079102% |

The ideal fiber geometry is identical, but the represented area differs because
of the discretizations. The concentric generator preserves nominal disk area;
structured centroid assignment approximates that area. Increasing `nx` and
`ny` improves the structured representation.

Both generated meshes can be used with the existing in-plane and antiplane
homogenization inputs by changing `Mesh/rve/file` to the selected saved mesh.
The standalone generator names remain available for existing input files.

## Homogenization Comparison

[Results for both representations](homogenization/RESULTS.md) contain the three
plane-strain and two antiplane periodic load cases, their full effective
matrices, energy comparisons, and convergence statistics. Matrix E=100 and
fiber E=1000, with nu=0.25 for both phases. Reproduce with:

```bash
python homogenization/run_comparison.py
```

At the supplied resolution, the structured matrix differs from the concentric
matrix by 0.246188% in the in-plane Frobenius norm and 0.157290% in the antiplane
Frobenius norm. Both matrices are symmetric to numerical precision and positive
definite, and all ten cases converge in one Newton iteration.
