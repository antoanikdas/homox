# Two-Phase Effective Stiffness Matrices

## Problem definition

The SVE is a unit square containing a centered square inclusion. The generated mesh has 256
equal-area elements: 192 matrix elements and 64 inclusion elements, giving an inclusion volume
fraction of $c_i=0.25$.

| Phase | Young's modulus | Poisson's ratio | Volume fraction |
|---|---:|---:|---:|
| Matrix | 100 | 0.25 | 0.75 |
| Inclusion | 1000 | 0.25 | 0.25 |

All values below use the engineering-shear Voigt convention

\[
\begin{bmatrix}
\bar\sigma_{xx}\\
\bar\sigma_{yy}\\
\bar\sigma_{xy}
\end{bmatrix}
=
\mathbf C^{\mathrm{eff}}
\begin{bmatrix}
\bar\epsilon_{xx}\\
\bar\epsilon_{yy}\\
\bar\gamma_{xy}
\end{bmatrix},
\qquad
\bar\gamma_{xy}=2\bar\epsilon_{xy}.
\]

Consequently, $C_{66}=G_{xy}$. Internally, `homox` uses a fourth-order tensor rather than this
Voigt matrix; the equivalent tensor components satisfy
$C_{1111}=C_{11}$, $C_{1122}=C_{12}$, $C_{2222}=C_{22}$, and
$C_{1212}=C_{66}$.

## Matrix extraction

For the kinematic cases, each imposed strain loading supplies one column of
$\mathbf C^{\mathrm{eff}}$. For the mixed and static cases, the three simulations first supply
the effective compliance columns,

\[
\bar{\boldsymbol\epsilon}=\mathbf S^{\mathrm{eff}}\bar{\boldsymbol\sigma},
\qquad
\mathbf C^{\mathrm{eff}}=(\mathbf S^{\mathrm{eff}})^{-1}.
\]

The reported matrices are symmetrized. Numerically negligible normal-shear coupling terms are
shown as zero.

## Periodic boundary conditions

### Periodic kinematic

\[
\mathbf C^{\mathrm{PK}}=
\begin{bmatrix}
176.687447 & 51.486817 & 0\\
51.486817 & 176.687447 & 0\\
0 & 0 & 54.221668
\end{bmatrix}.
\]

### Periodic mixed

\[
\mathbf C^{\mathrm{PM}}=
\begin{bmatrix}
176.687447 & 51.486817 & 0\\
51.486817 & 176.687447 & 0\\
0 & 0 & 54.221668
\end{bmatrix}.
\]

### Periodic static

\[
\mathbf C^{\mathrm{PS}}=
\begin{bmatrix}
176.687447 & 51.486817 & 0\\
51.486817 & 176.687447 & 0\\
0 & 0 & 54.221668
\end{bmatrix}.
\]

The three periodic formulations recover the same linear-elastic effective tensor to numerical
precision, as expected when the average constraints are imposed consistently.

## Nonperiodic boundary conditions

### Nonperiodic kinematic

\[
\mathbf C^{\mathrm{NK}}=
\begin{bmatrix}
177.455467 & 51.574028 & 0\\
51.574028 & 177.455467 & 0\\
0 & 0 & 58.239285
\end{bmatrix}.
\]

### Nonperiodic mixed

\[
\mathbf C^{\mathrm{NM}}=
\begin{bmatrix}
175.150517 & 54.696931 & 0\\
54.696931 & 175.150517 & 0\\
0 & 0 & 54.221668
\end{bmatrix}.
\]

### Nonperiodic static

\[
\mathbf C^{\mathrm{NS}}=
\begin{bmatrix}
169.806361 & 58.044116 & 0\\
58.044116 & 169.806361 & 0\\
0 & 0 & 53.232150
\end{bmatrix}.
\]

The nonperiodic matrices differ because the finite SVE does not satisfy the same admissible-field
space under full affine displacement, mixed displacement/traction, and uniform traction boundary
conditions. The equality of the $xx$ and $yy$ components follows from the square geometry.

## Mori-Tanaka mean-field reference

The Mori-Tanaka reference treats the square inclusion as an equivalent circular cylindrical
inclusion in an isotropic matrix under plane strain. It is therefore an analytical mean-field
reference, not an exact solution for the square-inclusion SVE.

For each phase,

\[
G_r=\frac{E_r}{2(1+\nu_r)},\qquad
\lambda_r=\frac{E_r\nu_r}{(1+\nu_r)(1-2\nu_r)},\qquad
K_r^{2D}=\lambda_r+G_r.
\]

For the matrix, $K_m^{2D}=80$, $G_m=40$, and the in-plane plane-strain Poisson ratio is
$\nu_m^{2D}=\nu_m/(1-\nu_m)=1/3$. The cylindrical-inclusion concentration factors are

\[
\alpha_K=\frac{1}{2(1-\nu_m^{2D})}=0.75,
\qquad
\alpha_G=\frac{3-4\nu_m^{2D}}{4(1-\nu_m^{2D})}=0.625.
\]

For $M=K^{2D}$ or $G$, the Mori-Tanaka estimate used here is

\[
\frac{M_{\mathrm{MT}}}{M_m}
=1+\frac{c_i}
{M_m/(M_i-M_m)+(1-c_i)\alpha_M}.
\]

This gives

\[
K_{\mathrm{MT}}^{2D}=109.690722,
\qquad
G_{\mathrm{MT}}=57.245509,
\]

and the expected mean-field stiffness is

\[
\boxed{
\mathbf C^{\mathrm{MT}}=
\begin{bmatrix}
166.936231 & 52.445213 & 0\\
52.445213 & 166.936231 & 0\\
0 & 0 & 57.245509
\end{bmatrix}}
\]

The corresponding in-plane constants are $E^{2D}_{\mathrm{MT}}=150.459877$ and
$\nu^{2D}_{\mathrm{MT}}=0.314163$.

## Numerical comparison

| Method | $C_{11}$ | $C_{12}$ | $C_{66}$ | Difference from MT, Frobenius norm |
|---|---:|---:|---:|---:|
| Mori-Tanaka reference | 166.936231 | 52.445213 | 57.245509 | 0% |
| Periodic kinematic | 176.687447 | 51.486817 | 54.221668 | 5.584% |
| Periodic mixed | 176.687447 | 51.486817 | 54.221668 | 5.584% |
| Periodic static | 176.687447 | 51.486817 | 54.221668 | 5.584% |
| Nonperiodic kinematic | 177.455467 | 51.574028 | 58.239285 | 5.890% |
| Nonperiodic mixed | 175.150517 | 54.696931 | 54.221668 | 4.889% |
| Nonperiodic static | 169.806361 | 58.044116 | 53.232150 | 3.843% |

The Mori-Tanaka discrepancy is expected because it represents an isolated circular cylindrical
inclusion embedded in a mean matrix field, whereas the numerical SVE contains a periodic or
finite-boundary array of square inclusions.

## Bulk and shear modulus comparison

For the in-plane plane-strain matrix, define the two-dimensional areal bulk modulus and in-plane
shear modulus as

\[
K^{2D}=\frac{C_{11}+C_{12}}{2},
\qquad
G_{xy}=C_{66}.
\]

Here, $K^{2D}$ relates the mean in-plane stress to the in-plane dilatation. It is not the
three-dimensional bulk modulus, which cannot be recovered from this in-plane matrix alone without
additional out-of-plane constitutive information.

| Method | $K^{2D}$ | $G_{xy}$ |
|---|---:|---:|
| Matrix phase | 80.000000 | 40.000000 |
| Inclusion phase | 800.000000 | 400.000000 |
| Mori-Tanaka reference | 109.690722 | 57.245509 |
| Periodic kinematic | 114.087132 | 54.221668 |
| Periodic mixed | 114.087132 | 54.221668 |
| Periodic static | 114.087132 | 54.221668 |
| Nonperiodic kinematic | 114.514747 | 58.239285 |
| Nonperiodic mixed | 114.923724 | 54.221668 |
| Nonperiodic static | 113.925239 | 53.232150 |

The antiplane study of the same extruded microstructure gives
$G_{xz}=G_{yz}=61.990661$ for all periodic formulations. This differs from
$G_{xy}=54.221668$, which is permissible because the extruded square-inclusion composite is
transversely isotropic rather than fully isotropic.

## Reference

The plane-strain cylindrical-inclusion concentration factors and Mori-Tanaka form follow
L. Shen and J. Li, *Homogenization of a fibre/sphere with an inhomogeneous interphase for the
effective elastic moduli of composites*, Proceedings of the Royal Society A 461 (2005),
[doi:10.1098/rspa.2005.1447](https://doi.org/10.1098/rspa.2005.1447).
