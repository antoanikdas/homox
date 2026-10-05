#include "ADHomoxAntiplaneLinearElasticMaterial.h"

registerMooseObject("homoxApp", ADHomoxAntiplaneLinearElasticMaterial);

InputParameters
ADHomoxAntiplaneLinearElasticMaterial::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription(
      "Computes linear antiplane stress and energy without a physics-module dependency.");
  params.addParam<Real>("shear_modulus", "Isotropic antiplane shear modulus");
  params.addParam<std::vector<Real>>(
      "antiplane_stiffness", "Symmetric entries K_xx, K_xy, and K_yy");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  return params;
}

ADHomoxAntiplaneLinearElasticMaterial::ADHomoxAntiplaneLinearElasticMaterial(
    const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _antiplane_strain(
        getADMaterialProperty<RealVectorValue>(_base_name + "antiplane_strain")),
    _antiplane_stiffness(
        declareADProperty<RankTwoTensor>(_base_name + "antiplane_stiffness")),
    _antiplane_stress(
        declareADProperty<RealVectorValue>(_base_name + "antiplane_stress")),
    _antiplane_energy(declareADProperty<Real>(_base_name + "antiplane_energy"))
{
  const bool has_isotropic = isParamValid("shear_modulus");
  const bool has_tensor = isParamValid("antiplane_stiffness");
  if (has_isotropic == has_tensor)
    mooseError(name(), ": specify either shear_modulus or antiplane_stiffness, but not both.");

  _stiffness.zero();
  if (has_isotropic)
  {
    const Real G = getParam<Real>("shear_modulus");
    if (G <= 0.0)
      paramError("shear_modulus", "The shear modulus must be positive.");
    _stiffness(0, 0) = G;
    _stiffness(1, 1) = G;
  }
  else
  {
    const auto & values = getParam<std::vector<Real>>("antiplane_stiffness");
    if (values.size() != 3)
      paramError("antiplane_stiffness", "Supply exactly three entries: K_xx K_xy K_yy.");
    if (values[0] <= 0.0 || values[2] <= 0.0 || values[0] * values[2] <= values[1] * values[1])
      paramError("antiplane_stiffness", "The symmetric 2-by-2 stiffness must be positive definite.");
    _stiffness(0, 0) = values[0];
    _stiffness(0, 1) = values[1];
    _stiffness(1, 0) = values[1];
    _stiffness(1, 1) = values[2];
  }
}

void
ADHomoxAntiplaneLinearElasticMaterial::computeQpProperties()
{
  _antiplane_stiffness[_qp] = _stiffness;
  _antiplane_stress[_qp] = _antiplane_stiffness[_qp] * _antiplane_strain[_qp];
  _antiplane_energy[_qp] =
      0.5 * (_antiplane_strain[_qp] * _antiplane_stress[_qp]);
}
