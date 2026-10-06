# Reproduced Example Results

The complete Homox example suite contains 219 runnable input files. All 219 cases were rerun using
the `moose-06-16` environment and the rebuilt `homox-opt` executable. Every solve converged, every
input produced its expected CSV output, and no MOOSE error or nonlinear-divergence signature was
reported.

Effective matrices were reconstructed from the final CSV rows. For in-plane cases,

\[
\mathbf C^{\mathrm{eff}}=\boldsymbol\Sigma\mathbf E^{-1},
\qquad
\mathbf E=
\begin{bmatrix}
\bar\epsilon_{xx}\\
\bar\epsilon_{yy}\\
2\bar\epsilon_{xy}
\end{bmatrix},
\]

and for antiplane cases,

\[
\mathbf K^{\mathrm{eff}}=\mathbf T\boldsymbol\Gamma^{-1}.
\]

The reconstructed values match the detailed result files at their reported six-decimal precision:

- [Homogeneous anisotropic verification](anisotropic_homogenization/README.md)
- [Square-inclusion anisotropic composite](anisotropic_homogenization/two_phase/README.md)
- [Circular-inclusion anisotropic composite](anisotropic_homogenization/circular_inclusion/README.md)
- [Anisotropic four-cell array](anisotropic_homogenization/unit_cell_array/README.md)
- [Anisotropic two-cell array](anisotropic_homogenization/unit_cell_array_2x1/README.md)
- [Square-inclusion in-plane composite](small_strain_homogenization/two_phase/C_MATRICES.md)
- [Circular-inclusion in-plane composite](small_strain_homogenization/circular_inclusion/C_MATRICES.md)
- [Square-inclusion antiplane composite](antiplane_homogenization/two_phase/K_MATRICES.md)
- [Circular-inclusion antiplane composite](antiplane_homogenization/circular_inclusion/K_MATRICES.md)
- [Multiple-inclusion generated-mesh study](multiple_homogenization/README.md)

The remaining four basic small-strain examples also converged and provide direct homogeneous and
nonperiodic formulation checks, but they do not define an additional effective-matrix study.

The automated regression suite additionally contains six independent three-dimensional periodic
load cases. A homogeneous material with `E = 100` and `nu = 0.25` recovers normal stiffness 120,
normal coupling 40, and all three engineering shear moduli equal to 40.
