# Nonperiodic mixed homogenization: prescribed y extension and traction-free transverse edges.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]
!include ../nonperiodic_common.i
[BCs]
  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
    preset = true
  []
  [top_y]
    type = DirichletBC
    variable = disp_y
    boundary = top
    value = 0.01
    preset = true
  []
  [pin_x]
    type = DirichletBC
    variable = disp_x
    boundary = pin_origin
    value = 0
    preset = true
  []
[]
