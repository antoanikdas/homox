# Homox

Homox is a MOOSE application for small-strain elastic homogenization. The
required mechanics and homogenization objects are implemented directly in the
application without using the MOOSE Solid Mechanics module.

## Features

- In-plane, three-dimensional, and antiplane elasticity
- Isotropic and anisotropic materials
- Periodic kinematic, mixed, and static homogenization
- Nonperiodic kinematic, mixed, and static homogenization
- Two-phase and multi-inclusion examples

## Build

```bash
make -j4
```

## Run

```bash
./homox-opt -i <input_file.i>
```

## Test

```bash
./run_tests -j4
```
