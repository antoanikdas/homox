#include "ADHomoxStressDivergence.h"

registerMooseObject("homoxApp", ADHomoxStressDivergence);

InputParameters
ADHomoxStressDivergence::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription("Small-strain stress-divergence kernel without Solid Mechanics.");
  params.addRequiredRangeCheckedParam<unsigned int>(
      "component", "component >= 0 & component <= 2", "Displacement equation component");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

ADHomoxStressDivergence::ADHomoxStressDivergence(const InputParameters & parameters)
  : ADKernel(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getADMaterialProperty<RankTwoTensor>(_base_name + "stress")),
    _component(getParam<unsigned int>("component"))
{
}

void
ADHomoxStressDivergence::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError("ADHomoxStressDivergence requires Cartesian coordinates.");
}

ADReal
ADHomoxStressDivergence::computeQpResidual()
{
  return _stress[_qp].row(_component) * _grad_test[_i][_qp];
}
