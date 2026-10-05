#pragma once

#include "ADKernel.h"
#include "RankTwoTensor.h"

/** Small-strain equilibrium kernel using an AD stress material property. */
class ADHomoxStressDivergence : public ADKernel
{
public:
  static InputParameters validParams();

  ADHomoxStressDivergence(const InputParameters & parameters);

protected:
  void initialSetup() override;
  ADReal computeQpResidual() override;

  const std::string _base_name;
  const ADMaterialProperty<RankTwoTensor> & _stress;
  const unsigned int _component;
};
