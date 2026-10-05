#pragma once

#include "ADMaterial.h"
#include "HomoxAntiplaneHomogenizationInterface.h"

/** Computes grad(u_z) from a periodic fluctuation and a scalar macroscopic gradient. */
class ADHomoxAntiplaneStrain
  : public HomoxAntiplaneHomogenizationInterface<ADMaterial>
{
public:
  static InputParameters validParams();

  ADHomoxAntiplaneStrain(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

  const ADVariableGradient & _grad_displacement;
  const ADVariableValue & _macro_gradient;
  const std::string _base_name;
  ADMaterialProperty<RealVectorValue> & _antiplane_strain;
};
