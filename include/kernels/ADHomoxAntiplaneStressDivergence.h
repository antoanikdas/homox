#pragma once

#include "ADKernel.h"

/** Antiplane equilibrium kernel for direct boundary-value problems. */
class ADHomoxAntiplaneStressDivergence : public ADKernel
{
public:
  static InputParameters validParams();

  ADHomoxAntiplaneStressDivergence(const InputParameters & parameters);

protected:
  ADReal computeQpResidual() override;

  const ADMaterialProperty<RealVectorValue> & _antiplane_stress;
};
