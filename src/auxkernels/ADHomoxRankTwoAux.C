#include "ADHomoxRankTwoAux.h"

registerMooseObject("homoxApp", ADHomoxRankTwoAux);

InputParameters
ADHomoxRankTwoAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Outputs a component of an AD rank-two material property.");
  params.addRequiredParam<MaterialPropertyName>("tensor", "AD rank-two material property");
  params.addRequiredRangeCheckedParam<unsigned int>(
      "index_i", "index_i >= 0 & index_i <= 2", "First tensor index");
  params.addRequiredRangeCheckedParam<unsigned int>(
      "index_j", "index_j >= 0 & index_j <= 2", "Second tensor index");
  return params;
}

ADHomoxRankTwoAux::ADHomoxRankTwoAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _tensor(getADMaterialProperty<RankTwoTensor>("tensor")),
    _i(getParam<unsigned int>("index_i")),
    _j(getParam<unsigned int>("index_j"))
{
}

Real
ADHomoxRankTwoAux::computeValue()
{
  return raw_value(_tensor[_qp](_i, _j));
}
