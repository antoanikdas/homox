!config navigation breadcrumbs=False scrollspy=False

# homoxApp

`homox` contains framework-only implementations of
[in-plane small-strain homogenization](source/homogenization.md) and
[antiplane homogenization](source/antiplane_homogenization.md). The optional MOOSE Solid
Mechanics module remains disabled.

The [multiple-inclusion mesh generator](source/multi_inclusion_mesh.md) creates configurable
rectangular arrays of conforming circular-inclusion cells for either formulation.

[CSV fiber meshing](source/csv_fiber_mesh.md) reads inclusion centers and individual radii and
generates either a structured QUAD4 or fitted TRI3 mesh, with optional periodic wrapping.
