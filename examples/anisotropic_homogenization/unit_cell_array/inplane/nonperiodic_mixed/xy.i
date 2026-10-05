# Nonperiodic mixed shear: gamma_xy = 0.02, hence epsilon_xy = 0.01.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]
!include ../nonperiodic_common.i
[BCs]
  [bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom
    value = 0
    preset = true
  []
  [top_x]
    type = DirichletBC
    variable = disp_x
    boundary = top
    value = 0.04
    preset = true
  []
  [left_y]
    type = DirichletBC
    variable = disp_y
    boundary = left
    value = 0
    preset = true
  []
  [right_y]
    type = DirichletBC
    variable = disp_y
    boundary = right
    value = 0
    preset = true
  []
[]
