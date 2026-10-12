# Axial Homogenization Validation

This self-contained example verifies a homogeneous isotropic material
(E=100, nu=0.25) on a 4 x 4 unit-square cross-section using all six BC families.
Each directory contains xx, yy, xy, and zz loads. The zz strain is uniform;
the displacement fields and boundary conditions remain two-dimensional.

```bash
python run_validation.py
```

The script runs all 24 cases, verifies each against its analytical strain,
stress, and energy response, and reconstructs the generalized-plane-strain
4 x 4 stiffness block from the measured columns. See [RESULTS.md](RESULTS.md).

Periodic cases use the existing scalar homogenization equations with four
active components. Nonperiodic cases use the compatible strain material with
`out_of_plane_strain = 0.01` for the zz load and zero for the other loads.
This optional constant defaults to zero, retaining ordinary plane strain.

For axial epsilon_zz=0.01, kinematic restraint gives sigma_zz=1.2, mixed
restraint gives sigma_zz=1.066666667, and traction-free transverse relaxation
gives sigma_zz=1.0. The labels static and mixed describe transverse boundary
control; all zz cases prescribe axial strain because the 2D mesh has no
physical z-end faces.

All families recover C13=C23=40, C33=120, and C36=0 after reconstructing the
matrix from all four load cases. Dividing axial stress by axial strain alone
does not recover C33 when transverse strains are allowed to relax.
