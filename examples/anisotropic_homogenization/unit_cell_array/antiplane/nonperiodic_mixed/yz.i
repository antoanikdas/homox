# Nonperiodic mixed antiplane loading: y displacement jump with x faces traction-free.
!include ../nonperiodic_common.i
[BCs]
  [bottom]
    type = DirichletBC
    variable = disp_z
    boundary = bottom
    value = 0
    preset = true
  []
  [top]
    type = DirichletBC
    variable = disp_z
    boundary = top
    value = 0.02
    preset = true
  []
[]
