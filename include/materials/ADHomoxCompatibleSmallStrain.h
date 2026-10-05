#pragma once

#include "ADMaterial.h"
#include "RankTwoTensor.h"

/** Computes infinitesimal strain directly from displacement variables. */
class ADHomoxCompatibleSmallStrain : public ADMaterial
{
public:
  static InputParameters validParams();

  ADHomoxCompatibleSmallStrain(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

  const unsigned int _ndisp;
  std::vector<const ADVariableGradient *> _grad_displacements;
  const std::string _base_name;
  ADMaterialProperty<RankTwoTensor> & _total_strain;
  ADMaterialProperty<RankTwoTensor> & _mechanical_strain;
};
