[Mesh]
  [rve]
    type = HomoxRandomFiberRVEMeshGenerator
    mesh_type = concentric
    inclusions_file = gold/inclusions.csv
    periodic_geometry = true
    nx = 32
    ny = 32
    num_sectors = 4
    fiber_rings = 2
    matrix_rings = 1
    boundary_segments_x = 8
    boundary_segments_y = 8
    desired_area = 0.01
  []
[]
