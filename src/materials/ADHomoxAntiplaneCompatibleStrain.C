#include "ADHomoxAntiplaneCompatibleStrain.h"

registerMooseObject("homoxApp", ADHomoxAntiplaneCompatibleStrain);

InputParameters
ADHomoxAntiplaneCompatibleStrain::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("Computes the compatible antiplane engineering-shear vector grad(u_z).");
  params.addRequiredCoupledVar("displacement", "Antiplane displacement variable");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  return params;
}

ADHomoxAntiplaneCompatibleStrain::ADHomoxAntiplaneCompatibleStrain(
    const InputParameters & parameters)
  : ADMaterial(parameters),
    _grad_displacement(adCoupledGradient("displacement")),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _antiplane_strain(
        declareADProperty<RealVectorValue>(_base_name + "antiplane_strain"))
{
}

void
ADHomoxAntiplaneCompatibleStrain::computeQpProperties()
{
  _antiplane_strain[_qp] = _grad_displacement[_qp];
}
