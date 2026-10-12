!include mesh.i
[Problem]
  solve = false
[]
[Executioner]
  type = Steady
[]
[Reporters]
  [geometry]
    type = MeshMetaDataReporter
    include = '*rve/fiber_centers *rve/fiber_radii'
    entries = value
  []
[]
[Outputs]
  [geometry]
    type = JSON
    execute_on = initial
    execute_system_information_on = none
  []
[]
