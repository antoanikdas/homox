[Mesh]
  [fibers]
    type = HomoxRandomConcentricFiberMeshGenerator
    xmin = 0
    xmax = 1
    ymin = 0
    ymax = 1
    radius = 0.06
    number_of_fibers = 22
    periodic_geometry = true
    matrix_shell_thickness = 0.01
    minimum_spacing = 0.01
    boundary_buffer = 0.01
    num_sectors = 8
    fiber_rings = 4
    matrix_rings = 2
    boundary_segments_x = 32
    boundary_segments_y = 32
    desired_area = 0.001
    seed = 1980
  []
[]
