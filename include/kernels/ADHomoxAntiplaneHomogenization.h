#pragma once

#include "ADKernelScalarBase.h"
#include "HomoxAntiplaneHomogenizationInterface.h"
#include "RankTwoTensor.h"

/** Antiplane equilibrium kernel with volume-average gradient/stress constraints. */
class ADHomoxAntiplaneHomogenization
  : public HomoxAntiplaneHomogenizationInterface<ADKernelScalarBase>
{
public:
  static InputParameters validParams();

  ADHomoxAntiplaneHomogenization(const InputParameters & parameters);

  std::set<std::string> additionalROVariables() override;
  void computeResidual() override;
  void computeJacobian() override;
  void computeResidualAndJacobian() override;

protected:
  void initialSetup() override;
  ADReal computeQpResidual() override;
  ADReal computeScalarQpResidual() override;
  void computeScalarResidualManual();
  void computeScalarJacobianManual();

  const std::string _base_name;
  const ADMaterialProperty<RealVectorValue> & _antiplane_stress;
  const ADMaterialProperty<RealVectorValue> & _antiplane_strain;
  const ADMaterialProperty<RankTwoTensor> & _antiplane_stiffness;
};
