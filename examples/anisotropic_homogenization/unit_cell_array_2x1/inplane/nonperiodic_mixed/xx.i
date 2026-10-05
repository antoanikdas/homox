# Nonperiodic mixed homogenization: prescribed x extension and traction-free transverse edges.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]
!include ../nonperiodic_common.i
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
    value = 0.02
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
