#include "ADHomoxSmallStrain.h"

registerMooseObject("homoxApp", ADHomoxSmallStrain);

InputParameters
ADHomoxSmallStrain::validParams()
{
  InputParameters params = HomoxHomogenizationInterface<ADMaterial>::validParams();
  params.addClassDescription(
      "Computes small strain from periodic displacement fluctuations and scalar macro strain.");
  params.addRequiredCoupledVar("displacements", "Displacement fluctuation variables");
  params.addRequiredCoupledVar("macro_strain", "Scalar variable containing the macro strain");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  return params;
}

ADHomoxSmallStrain::ADHomoxSmallStrain(const InputParameters & parameters)
  : HomoxHomogenizationInterface<ADMaterial>(parameters),
    _ndisp(coupledComponents("displacements")),
    _grad_displacements(_ndisp),
    _macro_strain(adCoupledScalarValue("macro_strain")),
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
ADHomoxSmallStrain::computeQpProperties()
{
  _total_strain[_qp].zero();

  for (unsigned int i = 0; i < _ndisp; ++i)
    for (unsigned int j = 0; j < _ndisp; ++j)
      _total_strain[_qp](i, j) =
          0.5 * ((*_grad_displacements[i])[_qp](j) + (*_grad_displacements[j])[_qp](i));

  unsigned int scalar_component = 0;
  for (const auto & [indices, constraint] : constraintMap())
  {
    const auto [i, j] = indices;
    _total_strain[_qp](i, j) += _macro_strain[scalar_component];
    if (i != j)
      _total_strain[_qp](j, i) += _macro_strain[scalar_component];
    ++scalar_component;
  }

  _mechanical_strain[_qp] = _total_strain[_qp];
}
