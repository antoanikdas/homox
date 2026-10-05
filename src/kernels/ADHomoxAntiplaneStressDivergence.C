#include "ADHomoxAntiplaneStressDivergence.h"

registerMooseObject("homoxApp", ADHomoxAntiplaneStressDivergence);

InputParameters
ADHomoxAntiplaneStressDivergence::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription("Enforces antiplane equilibrium using material-computed shear stress.");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

ADHomoxAntiplaneStressDivergence::ADHomoxAntiplaneStressDivergence(
    const InputParameters & parameters)
  : ADKernel(parameters),
    _antiplane_stress(getADMaterialProperty<RealVectorValue>(
        isParamValid("base_name") ? getParam<std::string>("base_name") + "_antiplane_stress"
                                  : "antiplane_stress"))
{
}

ADReal
ADHomoxAntiplaneStressDivergence::computeQpResidual()
{
  return _antiplane_stress[_qp] * _grad_test[_i][_qp];
}
