[Mesh]
  [rve]
    type = HomoxRandomFiberRVEMeshGenerator
    mesh_type = structured
    inclusions_file = inclusions.csv
    xmin = 0
    xmax = 1
    ymin = 0
    ymax = 1
    periodic_geometry = true
    nx = 128
    ny = 128
    num_sectors = 8
    fiber_rings = 4
    matrix_rings = 2
    boundary_segments_x = 32
    boundary_segments_y = 32
    desired_area = 0.001
  []
[]
