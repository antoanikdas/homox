#pragma once

#include "ADMaterial.h"
#include "RankTwoTensor.h"

/** Computes linear antiplane stress, tau = K grad(u_z), and elastic energy. */
class ADHomoxAntiplaneLinearElasticMaterial : public ADMaterial
{
public:
  static InputParameters validParams();

  ADHomoxAntiplaneLinearElasticMaterial(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

  const std::string _base_name;
  RankTwoTensor _stiffness;
  const ADMaterialProperty<RealVectorValue> & _antiplane_strain;
  ADMaterialProperty<RankTwoTensor> & _antiplane_stiffness;
  ADMaterialProperty<RealVectorValue> & _antiplane_stress;
  ADMaterialProperty<Real> & _antiplane_energy;
};
