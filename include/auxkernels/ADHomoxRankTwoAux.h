#pragma once

#include "AuxKernel.h"
#include "RankTwoTensor.h"

/** Outputs one component of an AD RankTwoTensor material property. */
class ADHomoxRankTwoAux : public AuxKernel
{
public:
  static InputParameters validParams();

  ADHomoxRankTwoAux(const InputParameters & parameters);

protected:
  Real computeValue() override;

  const ADMaterialProperty<RankTwoTensor> & _tensor;
  const unsigned int _i;
  const unsigned int _j;
};
