#pragma once

#include "ADMaterial.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

/** Supplies a constant stiffness tensor and computes sigma = C:epsilon. */
class ADHomoxLinearElasticMaterial : public ADMaterial
{
public:
  static InputParameters validParams();

  ADHomoxLinearElasticMaterial(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

  const std::string _base_name;
  RankFourTensor _stiffness;
  const ADMaterialProperty<RankTwoTensor> & _mechanical_strain;
  ADMaterialProperty<RankFourTensor> & _elasticity_tensor;
  ADMaterialProperty<RankTwoTensor> & _stress;
  ADMaterialProperty<Real> & _elastic_energy;
};
