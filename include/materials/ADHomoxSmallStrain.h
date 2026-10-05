#pragma once

#include "ADMaterial.h"
#include "HomoxHomogenizationInterface.h"
#include "RankTwoTensor.h"

/** Computes infinitesimal strain from displacement fluctuations and a scalar macro strain. */
class ADHomoxSmallStrain : public HomoxHomogenizationInterface<ADMaterial>
{
public:
  static InputParameters validParams();

  ADHomoxSmallStrain(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

  const unsigned int _ndisp;
  std::vector<const ADVariableGradient *> _grad_displacements;
  const ADVariableValue & _macro_strain;
  const std::string _base_name;
  ADMaterialProperty<RankTwoTensor> & _total_strain;
  ADMaterialProperty<RankTwoTensor> & _mechanical_strain;
};
