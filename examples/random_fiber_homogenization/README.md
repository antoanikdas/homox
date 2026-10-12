# Structured Random-Fiber RVE

This example generates a structured periodic RVE containing reproducible,
nonoverlapping circular fibers. Elements are assigned to the `matrix` or
`fiber` block using their centroids, so the circular interfaces are
stair-stepped at the mesh resolution.

```bash
cd examples/random_fiber_homogenization
../../homox-opt -i structured_mesh.i --mesh-only
```

`periodic_geometry = true` wraps every fiber through opposite boundaries using
the minimum-image convention. The generated left/right and bottom/top meshes
therefore remain compatible with periodic boundary conditions.

For a choice of concentric TRI3 or structured QUAD4 meshes using exactly the
same sampled fiber centers, see [selectable_mesh](selectable_mesh/README.md).

Set `nx` and `ny` to control mesh resolution, `radius` for the common fiber
radius, and `seed` to choose a reproducible realization. Specify exactly one
of `volume_fraction` or `number_of_fibers`. The nominal volume fraction is
converted to the nearest integer fiber count; centroid assignment introduces
an additional discretization error in the achieved fraction.

The supplied 64 x 64 example creates 22 fibers with radius 0.06 in a unit
square. Its mesh contains 1014 fiber elements and 3082 matrix elements,
giving a represented fiber fraction of 0.24755859375. The requested fraction
is 0.25. Fibers crossing a boundary wrap to the opposite side.

The output is `structured_mesh_in.e` in this directory. Geometry wrapping
does not impose mechanical periodic boundary conditions; those must be added
in a homogenization input.
