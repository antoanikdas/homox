# Concentric Random-Fiber Mesh

`HomoxRandomConcentricFiberMeshGenerator` places nonoverlapping random fibers
in a rectangular periodic domain. Each fiber and its surrounding matrix shell are
generated with MOOSE's `ConcentricCircleMeshGenerator`. MOOSE's
`XYDelaunayGenerator` triangulates the remaining matrix and stitches it to all
the shells. The concentric quadrilaterals are then split with MOOSE's
`ElementsToSimplicesConverter`, giving TRI3 elements throughout the domain.
Interfaces share nodes, so displacement continuity requires no contact or
coupling constraints.

```bash
cd examples/random_fiber_homogenization/concentric
../../../homox-opt -i mesh.i --mesh-only
python inspect_mesh.py
```

The outputs are `mesh_in.e` and `mesh.png`. The plotting script also checks
TRI3-only connectivity, positive element areas, shared internal edges, mesh connectivity, total area,
and matching opposite exterior boundary nodes and material segments. Periodic
fiber connectivity and total fiber area are checked as well.

## Controls

- `radius`, `number_of_fibers`, and `seed` specify the fiber realization.
- `periodic_geometry = true` enables minimum-image spacing and wrapped fibers.
  Its default is `false` to retain the interior-fiber construction.
- `fiber_centers` optionally specifies one explicit XYZ center per fiber for
  deterministic geometrical tests. Centers must lie in the XY domain.
- `xmin`, `xmax`, `ymin`, and `ymax` specify the rectangular domain.
- `num_sectors` is the even number of sectors per quadrant.
- `fiber_rings` and `matrix_rings` control radial resolution before splitting
  the concentric quadrilaterals into triangles.
- `matrix_shell_thickness` controls the shell thickness around each fiber.
- `minimum_spacing` is the minimum gap between outer shells, not fiber surfaces.
- `boundary_buffer` controls the matrix margin only for nonperiodic geometry.
  In periodic mode it is padding for the temporary larger construction domain;
  it does not exclude fibers from the final RVE boundary.
- `desired_area` requests the maximum area of matrix triangles. Fixed boundary
  discretizations can limit refinement near constrained edges.
- `boundary_segments_x` and `boundary_segments_y` control the matching exterior
  segments in nonperiodic mode. In periodic mode these control the temporary
  exterior contour; final boundary nodes come from cutting and reconciliation.

The generated blocks are `fiber` and `matrix`, both containing only TRI3
elements. The concentric matrix shells and outer matrix use one matrix block:

```text
[Materials]
  [matrix]
    type = ADHomoxLinearElasticMaterial
    block = matrix
    youngs_modulus = 100
    poissons_ratio = 0.25
  []
[]
```

The exterior boundaries are `left`, `right`, `bottom`, and `top`, with
`bottom_left` and `bottom_right` corner node sets.

## Geometry and Periodicity

With `periodic_geometry = true`, centers are sampled throughout the full RVE.
Spacing is checked using minimum-image distances, including the outer shell's
volume-corrected polygon circumradius. Copies shifted by the domain width and
height are generated in a 3 x 3 neighborhood. After matrix triangulation and
stitching, four `XYMeshLineCutter` steps clip the mesh to the requested rectangle.
Fibers at corners therefore appear in all relevant opposite corners.

Opposite cut edges can have different matrix triangulation nodes. The generator
uses the union of those node positions on both sides and subdivides adjacent
triangles to obtain a conforming, node-matched exterior. This can create small
triangles near a cut or closely spaced boundary nodes. The final mesh remains
entirely TRI3 and preserves fiber area. Periodic mechanical BCs are still
specified separately in the homogenization input.

The nonperiodic mode keeps closed fiber shells inside the rectangle. Neither
mode guarantees the same fiber realization as the structured generator for
the same seed because shell spacing and random-number ordering differ.

With linear elements, circular interfaces are fitted polygonal boundaries,
not exact curved arcs. `preserve_volumes = true` is used internally to preserve
the nominal disk area despite this polygonal approximation. For equal radii,
the nominal fiber fraction is `number_of_fibers * pi * radius^2 / domain_area`.

The supplied unit-square example contains 22 fibers of radius 0.06 and has
nominal fiber fraction 0.2488141382 (24.8814%).

The verified periodic mesh contains 13,710 TRI3 elements and 7,198 nodes.
There are 157 nodes on each of left/right and 187 nodes on each of bottom/top.
Material intervals also match: each of left/right has 36 fiber segments, and
each of bottom/top has 53 fiber segments. The area and periodic connectivity
checks recover all 22 full fibers after accounting for their wrapped pieces.

Regression tests include explicit edge- and corner-crossing fibers, periodic
overlap rejection, the interior-fiber mode, and a periodic elastic recovery
case. With identical phase properties E=100 and nu=0.25, epsilon_xx=0.01
recovers sigma_xx=1.2 and sigma_yy=0.4 in one Newton iteration.

## Two-Phase Homogenization

The [homogenization results](homogenization/RESULTS.md) use this saved periodic
mesh with matrix E=100 and fiber E=1000, both with nu=0.25. Three plane-strain
loads and two antiplane shear loads reconstruct the effective matrices. Run
`python homogenization/run_homogenization.py` to reproduce the solves and report.
