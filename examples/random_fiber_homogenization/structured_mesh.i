[Mesh]
  [random_fibers]
    type = HomoxRandomFiberMeshGenerator
    nx = 64
    ny = 64
    xmin = 0
    xmax = 1
    ymin = 0
    ymax = 1

    radius = 0.06
    volume_fraction = 0.25
    minimum_spacing = 0.01
    periodic_geometry = true
    seed = 1980
  []
[]

[Outputs]
  exodus = true
[]
