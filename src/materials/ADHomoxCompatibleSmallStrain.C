#include "ADHomoxCompatibleSmallStrain.h"

registerMooseObject("homoxApp", ADHomoxCompatibleSmallStrain);

InputParameters
ADHomoxCompatibleSmallStrain::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("Computes compatible infinitesimal strain without Solid Mechanics.");
  params.addRequiredCoupledVar("displacements", "Displacement variables");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  return params;
}

ADHomoxCompatibleSmallStrain::ADHomoxCompatibleSmallStrain(const InputParameters & parameters)
  : ADMaterial(parameters),
    _ndisp(coupledComponents("displacements")),
    _grad_displacements(_ndisp),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _total_strain(declareADProperty<RankTwoTensor>(_base_name + "total_strain")),
    _mechanical_strain(declareADProperty<RankTwoTensor>(_base_name + "mechanical_strain"))
{
  if (_ndisp == 0 || _ndisp > Moose::dim)
    paramError("displacements", "Supply between one and three displacement variables.");

  for (unsigned int i = 0; i < _ndisp; ++i)
    _grad_displacements[i] = &adCoupledGradient("displacements", i);
}

void
ADHomoxCompatibleSmallStrain::computeQpProperties()
{
  _total_strain[_qp].zero();
  for (unsigned int i = 0; i < _ndisp; ++i)
    for (unsigned int j = 0; j < _ndisp; ++j)
      _total_strain[_qp](i, j) =
          0.5 * ((*_grad_displacements[i])[_qp](j) + (*_grad_displacements[j])[_qp](i));

  _mechanical_strain[_qp] = _total_strain[_qp];
}
