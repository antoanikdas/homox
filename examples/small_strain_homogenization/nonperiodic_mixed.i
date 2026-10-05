!include nonperiodic_common.i

# Mixed boundary condition: prescribe x extension, leave transverse boundaries traction-free.
[BCs]
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
    preset = true
  []
  [right_x]
    type = DirichletBC
    variable = disp_x
    boundary = right
    value = 0.01
    preset = true
  []
  [pin_y]
    type = DirichletBC
    variable = disp_y
    boundary = pin_origin
    value = 0
    preset = true
  []
[]
