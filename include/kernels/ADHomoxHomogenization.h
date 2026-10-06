#pragma once

#include "ADKernelScalarBase.h"
#include "HomoxHomogenizationInterface.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

/** Linear-elastic equilibrium kernel with volume-average strain/stress constraints. */
class ADHomoxHomogenization : public HomoxHomogenizationInterface<ADKernelScalarBase>
{
public:
  static InputParameters validParams();

  ADHomoxHomogenization(const InputParameters & parameters);

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
  const ADMaterialProperty<RankTwoTensor> & _stress;
  const ADMaterialProperty<RankTwoTensor> & _mechanical_strain;
  /// Consistent derivative of stress with respect to mechanical strain
  const ADMaterialProperty<RankFourTensor> & _elasticity_tensor;
  const unsigned int _component;
  const unsigned int _ndisp;
  std::vector<MooseVariable *> _disp_vars;
};
