# Homogeneous Anisotropic Homogenization Verification

These cases verify that all periodic and nonperiodic formulations recover a prescribed homogeneous
anisotropic stiffness. The material is deliberately coupled rather than merely diagonal.

## Prescribed stiffness

The in-plane plane-strain relation uses engineering shear:

\[
\begin{bmatrix}
\sigma_{xx}\\
\sigma_{yy}\\
\sigma_{xy}
\end{bmatrix}
=
\underbrace{
\begin{bmatrix}
200 & 50 & 12\\
50 & 120 & -8\\
12 & -8 & 60
\end{bmatrix}}_{\mathbf C_{2D}}
\begin{bmatrix}
\epsilon_{xx}\\
\epsilon_{yy}\\
\gamma_{xy}
\end{bmatrix},
\qquad \gamma_{xy}=2\epsilon_{xy}.
\]

Thus, the verification includes nonzero $C_{16}=12$ and $C_{26}=-8$ normal-shear coupling.
The full positive-definite tensor supplied through `symmetric21` is

```text
# 1111 1122 1133 1123 1113 1112 2222 2233 2223 2213 2212
# 3333 3323 3313 3312 2323 2313 2312 1313 1312 1212
C_ijkl = '200 50 40 0 0 12 120 30 0 0 -8 150 0 0 0 45 10 0 70 0 60'
```

The corresponding antiplane block is

\[
\begin{bmatrix}\tau_{xz}\\\tau_{yz}\end{bmatrix}
=
\underbrace{\begin{bmatrix}70&10\\10&45\end{bmatrix}}_{\mathbf K}
\begin{bmatrix}\gamma_{xz}\\\gamma_{yz}\end{bmatrix}.
\]

It is supplied directly as

```text
antiplane_stiffness = '70 10 45'
```

Both stiffness matrices are positive definite.

## Cases

The `inplane` directory contains `xx`, `yy`, and `xy` load cases for:

- periodic kinematic, mixed, and static constraints;
- nonperiodic kinematic, mixed, and static boundary conditions.

The `antiplane` directory contains corresponding `xz` and `yz` cases for the same six methods.

For each method, collect the average strain and stress columns from the independent load cases:

\[
\mathbf E=
\begin{bmatrix}
\bar\epsilon_{xx}^{(1)}&\bar\epsilon_{xx}^{(2)}&\bar\epsilon_{xx}^{(3)}\\
\bar\epsilon_{yy}^{(1)}&\bar\epsilon_{yy}^{(2)}&\bar\epsilon_{yy}^{(3)}\\
\bar\gamma_{xy}^{(1)}&\bar\gamma_{xy}^{(2)}&\bar\gamma_{xy}^{(3)}
\end{bmatrix},
\qquad
\boldsymbol\Sigma=
\begin{bmatrix}
\bar\sigma_{xx}^{(1)}&\bar\sigma_{xx}^{(2)}&\bar\sigma_{xx}^{(3)}\\
\bar\sigma_{yy}^{(1)}&\bar\sigma_{yy}^{(2)}&\bar\sigma_{yy}^{(3)}\\
\bar\sigma_{xy}^{(1)}&\bar\sigma_{xy}^{(2)}&\bar\sigma_{xy}^{(3)}
\end{bmatrix}.
\]

The recovered stiffness is

\[
\mathbf C^{\mathrm{recovered}}=\boldsymbol\Sigma\mathbf E^{-1}.
\]

The same construction with two shear components gives
$\mathbf K^{\mathrm{recovered}}=\mathbf T\boldsymbol\Gamma^{-1}$ for antiplane loading. This
general extraction is required for mixed loading because the anisotropic coupling produces strain
components other than the directly prescribed component.

## Results

All methods recover the prescribed matrices to floating-point precision.

| Method | In-plane maximum absolute error | In-plane relative Frobenius error | Antiplane maximum absolute error | Antiplane relative Frobenius error |
|---|---:|---:|---:|---:|
| Periodic kinematic | 0 | 0 | 0 | 0 |
| Periodic mixed | $5.66\times10^{-12}$ | $2.60\times10^{-14}$ | $3.98\times10^{-13}$ | $5.48\times10^{-15}$ |
| Periodic static | $1.99\times10^{-12}$ | $1.08\times10^{-14}$ | $1.22\times10^{-12}$ | $1.49\times10^{-14}$ |
| Nonperiodic kinematic | $6.22\times10^{-15}$ | $3.56\times10^{-17}$ | $3.55\times10^{-15}$ | $5.95\times10^{-17}$ |
| Nonperiodic mixed | $5.46\times10^{-12}$ | $2.51\times10^{-14}$ | $2.13\times10^{-13}$ | $2.94\times10^{-15}$ |
| Nonperiodic static | $1.96\times10^{-12}$ | $1.08\times10^{-14}$ | $1.22\times10^{-12}$ | $1.49\times10^{-14}$ |

The agreement confirms that the in-plane fourth-order tensor path and the antiplane symmetric
$2\times2$ tensor path both preserve anisotropy and coupling under every implemented boundary
formulation.

