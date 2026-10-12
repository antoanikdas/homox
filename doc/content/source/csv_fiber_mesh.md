# CSV Fiber Meshes

`HomoxRandomFiberRVEMeshGenerator` accepts circular-inclusion centers and individual radii
from `inclusions_file`. This framework-only generator selects structured QUAD4 or fitted
all-TRI3 meshing of the same geometry; no physics module is required.

The CSV must have exactly the named columns `x`, `y`, and `radius`, in any order:

```text
x,y,radius
0.02,0.03,0.08
0.96,0.60,0.06
0.50,0.98,0.10
0.45,0.45,0.12
```

All coordinates and radii use the domain's length units. Each row specifies one inclusion.
The inclusion count is inferred from the file; do not also set `radius`, `number_of_fibers`,
`fiber_centers`, or `fiber_radii`. Without CSV input, the existing random-placement parameters
remain available.

```text
[Mesh]
  [rve]
    type = HomoxRandomFiberRVEMeshGenerator
    inclusions_file = inclusions.csv
    mesh_type = structured
    xmin = 0
    xmax = 1
    ymin = 0
    ymax = 1
    periodic_geometry = true
    nx = 128
    ny = 128
  []
[]
```

Set `mesh_type=concentric` for fitted TRI3 meshing. Its resolution is controlled by
`num_sectors`, `fiber_rings`, `matrix_rings`, `desired_area`, and the exterior boundary segment
counts. The shared RVE generator checks the actual radius of each inclusion, concentric shell
extent, and `minimum_spacing`, so both mesh choices admit the same geometry. Structured phase
assignment uses element centroids and therefore only approximates circular area; fitted meshing
preserves the nominal circular areas. The output phases are `fiber` (block 1) and `matrix` (block 2).

For periodic geometry, centers must lie in the half-open rectangle
`[xmin,xmax) x [ymin,ymax)`. Boundary-crossing inclusions wrap to opposite sides, and overlap
checks use minimum-image distances. Fitted periodic meshes match opposite exterior nodes.
This geometric option does not itself apply mechanical periodic boundary conditions.

For `periodic_geometry=false`, inclusions and their concentric shells must remain inside the
rectangle, respecting `boundary_buffer`. Invalid radii, out-of-domain centers, and overlapping
shells are rejected. Both direct generators, `HomoxRandomFiberMeshGenerator` and
`HomoxRandomConcentricFiberMeshGenerator`, also accept `inclusions_file`; the direct structured
generator checks fiber-surface spacing without concentric shell restrictions.

The wrapper provides `fiber_centers` and `fiber_radii` mesh metadata for checking or reporting
the exact geometry. The self-contained example is in `examples/csv_fiber_meshes`; it includes
mesh generation inputs, a CSV, and a verification script that saves `mesh_comparison.png`.
