#include "ADHomoxAntiplaneStrain.h"

registerMooseObject("homoxApp", ADHomoxAntiplaneStrain);

InputParameters
ADHomoxAntiplaneStrain::validParams()
{
  InputParameters params = HomoxAntiplaneHomogenizationInterface<ADMaterial>::validParams();
  params.addClassDescription(
      "Computes the antiplane engineering-shear vector from a periodic fluctuation and a "
      "macroscopic gradient.");
  params.addRequiredCoupledVar("displacement", "Periodic antiplane displacement fluctuation");
  params.addRequiredCoupledVar("macro_gradient", "Scalar variable containing active macro gradients");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  return params;
}

ADHomoxAntiplaneStrain::ADHomoxAntiplaneStrain(const InputParameters & parameters)
  : HomoxAntiplaneHomogenizationInterface<ADMaterial>(parameters),
    _grad_displacement(adCoupledGradient("displacement")),
    _macro_gradient(adCoupledScalarValue("macro_gradient")),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _antiplane_strain(
        declareADProperty<RealVectorValue>(_base_name + "antiplane_strain"))
{
}

void
ADHomoxAntiplaneStrain::computeQpProperties()
{
  _antiplane_strain[_qp] = _grad_displacement[_qp];

  unsigned int scalar_component = 0;
  for (const auto & [component, constraint] : constraintMap())
    _antiplane_strain[_qp](component) += _macro_gradient[scalar_component++];
}
