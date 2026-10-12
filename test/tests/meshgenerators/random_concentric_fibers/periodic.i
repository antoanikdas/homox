[Mesh]
  [fibers]
    type = HomoxRandomConcentricFiberMeshGenerator
    periodic_geometry = true
    radius = 0.08
    number_of_fibers = 3
    fiber_centers = '0.02 0.03 0
                    0.96 0.60 0
                    0.50 0.98 0'
    num_sectors = 4
    fiber_rings = 2
    matrix_rings = 1
    boundary_segments_x = 8
    boundary_segments_y = 8
    desired_area = 0.01
  []
[]
