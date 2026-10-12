# CSV Inclusion Meshes

Use one row per circular inclusion, with exactly the named columns `x,y,radius`.
Column order may vary. Centers and radii use the same length units as the domain.
The CSV determines the inclusion count; do not also set `radius`, `number_of_fibers`,
`fiber_centers`, or `fiber_radii`.

From this directory, using the `moose-06-16` environment:

```bash
../../homox-opt -i mesh.i --mesh-only structured.e
../../homox-opt -i mesh.i Mesh/rve/mesh_type=concentric --mesh-only triangular.e
python verify_meshes.py
```

Both meshes use the identical CSV geometry. `structured` generates QUAD4 elements
and assigns phases by element centroid; `concentric` produces a fitted all-TRI3 mesh.
Mesh density is controlled by `nx,ny` or by the sector/ring counts and `desired_area`,
respectively. The phase names are `fiber` (block 1) and `matrix` (block 2).

With `periodic_geometry=true`, centers must lie in the half-open domain
`[xmin,xmax) x [ymin,ymax)`. Inclusions crossing its boundaries wrap to the opposite
sides, and triangular boundary nodes are matched. Set `periodic_geometry=false`
for fully interior inclusions, allowing room for their matrix shells and boundary buffer.
Overlap checks include shells and `minimum_spacing` in the shared RVE generator.
Each fiber diameter must be smaller than both domain dimensions.

The direct `HomoxRandomFiberMeshGenerator` and
`HomoxRandomConcentricFiberMeshGenerator` also accept `inclusions_file`.
The direct structured generator checks fiber spacing rather than fitted shell spacing.
Existing random-placement inputs remain supported without CSV.
