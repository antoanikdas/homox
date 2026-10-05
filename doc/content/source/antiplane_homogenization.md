# Antiplane Homogenization

`homox` implements two-dimensional antiplane elasticity and homogenization using only MOOSE
framework objects. No Solid Mechanics or other physics module is linked.

## Formulation

For antiplane deformation, the only displacement is

\[
u_z=w(x,y), \qquad u_x=u_y=0.
\]

The engineering-shear and shear-stress vectors are

\[
\boldsymbol\gamma=
\begin{bmatrix}\gamma_{xz}\\\gamma_{yz}\end{bmatrix}
=
\begin{bmatrix}w_{,x}\\w_{,y}\end{bmatrix},
\qquad
\boldsymbol\tau=
\begin{bmatrix}\tau_{xz}\\\tau_{yz}\end{bmatrix}
=\mathbf K\boldsymbol\gamma.
\]

The strong and weak equilibrium equations are

\[
-\nabla\cdot\boldsymbol\tau=0,
\qquad
\int_\Omega \boldsymbol\tau\cdot\nabla\delta w\,dV=0.
\]

For periodic homogenization,

\[
w(\mathbf x)=\bar{\boldsymbol\gamma}\cdot\mathbf x+\widetilde w(\mathbf x),
\]

where \(\widetilde w\) is periodic and the two components of
\(\bar{\boldsymbol\gamma}\) are scalar degrees of freedom. Each active scalar equation enforces
either

\[
\langle\gamma_i\rangle=\gamma_i^{\mathrm{target}}
\quad\text{or}\quad
\langle\tau_i\rangle=\tau_i^{\mathrm{target}}.
\]

## Application objects

- `ADHomoxAntiplaneStrain`: periodic fluctuation gradient plus macroscopic gradient;
- `ADHomoxAntiplaneCompatibleStrain`: direct compatible gradient for nonperiodic cases;
- `ADHomoxAntiplaneLinearElasticMaterial`: symmetric 2-by-2 stiffness, stress, and energy;
- `ADHomoxAntiplaneHomogenization`: periodic equilibrium and average constraints;
- `ADHomoxAntiplaneStressDivergence`: direct nonperiodic equilibrium.

The material accepts either an isotropic `shear_modulus` or symmetric entries
`antiplane_stiffness = 'K_xx K_xy K_yy'`. The latter represents

\[
\mathbf K=\begin{bmatrix}K_{xx}&K_{xy}\\K_{xy}&K_{yy}\end{bmatrix}.
\]

## Boundary-condition families

The examples provide both `xz` and `yz` loading for all six families:

- **Periodic kinematic:** both average shear components are prescribed.
- **Periodic mixed:** the loaded average shear is prescribed and the transverse average stress is
  zero.
- **Periodic static:** both average stresses are prescribed.
- **Nonperiodic kinematic:** the complete affine field is prescribed on the exterior boundary.
- **Nonperiodic mixed:** the displacement jump is prescribed on the loaded face pair and the other
  face pair remains traction-free.
- **Nonperiodic static:** uniform opposing tractions are applied and one displacement is pinned.

The complete two-phase study is in `examples/antiplane_homogenization/two_phase`. It uses the same
16-by-16 centered-inclusion geometry as the in-plane example, with matrix and inclusion shear
moduli 40 and 400. The periodic cases recover

\[
\mathbf K^{\mathrm{eff}}=
\begin{bmatrix}
61.990661 & 0\\
0 & 61.990661
\end{bmatrix}.
\]
