# Nonperiodic mixed antiplane loading: x displacement jump with y faces traction-free.
!include ../nonperiodic_common.i
[BCs]
  [left]
    type = DirichletBC
    variable = disp_z
    boundary = left
    value = 0
    preset = true
  []
  [right]
    type = DirichletBC
    variable = disp_z
    boundary = right
    value = 0.02
    preset = true
  []
[]
