[Mesh]
  [random_fibers]
    type = HomoxRandomFiberMeshGenerator
    nx = 32
    ny = 32
    xmin = 0
    xmax = 1
    ymin = 0
    ymax = 1
    radius = 0.09
    volume_fraction = 0.20
    minimum_spacing = 0.01
    periodic_geometry = true
    seed = 1234
  []
[]

[Outputs]
  exodus = true
[]
