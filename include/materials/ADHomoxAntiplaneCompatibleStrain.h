#pragma once

#include "ADMaterial.h"

/** Computes the compatible antiplane engineering-shear vector grad(u_z). */
class ADHomoxAntiplaneCompatibleStrain : public ADMaterial
{
public:
  static InputParameters validParams();

  ADHomoxAntiplaneCompatibleStrain(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

  const ADVariableGradient & _grad_displacement;
  const std::string _base_name;
  ADMaterialProperty<RealVectorValue> & _antiplane_strain;
};
