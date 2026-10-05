#include "ADHomoxLinearElasticMaterial.h"

registerMooseObject("homoxApp", ADHomoxLinearElasticMaterial);

InputParameters
ADHomoxLinearElasticMaterial::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription(
      "Computes small-strain linear-elastic stress without the Solid Mechanics module.");
  params.addParam<Real>("youngs_modulus", "Young's modulus for isotropic elasticity");
  params.addParam<Real>("poissons_ratio", "Poisson's ratio for isotropic elasticity");
  params.addParam<std::vector<Real>>("C_ijkl", "Entries of a general stiffness tensor");
  params.addParam<MooseEnum>(
      "fill_method", RankFourTensor::fillMethodEnum() = "symmetric9", "C_ijkl fill method");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  return params;
}

ADHomoxLinearElasticMaterial::ADHomoxLinearElasticMaterial(const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _mechanical_strain(getADMaterialProperty<RankTwoTensor>(_base_name + "mechanical_strain")),
    _elasticity_tensor(declareADProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _stress(declareADProperty<RankTwoTensor>(_base_name + "stress")),
    _elastic_energy(declareADProperty<Real>(_base_name + "elastic_energy"))
{
  const bool has_isotropic = isParamValid("youngs_modulus") || isParamValid("poissons_ratio");
  const bool has_tensor = isParamValid("C_ijkl");

  if (has_isotropic == has_tensor)
    mooseError(name(),
               ": specify either youngs_modulus with poissons_ratio, or C_ijkl, but not both.");

  if (has_isotropic)
  {
    if (!isParamValid("youngs_modulus") || !isParamValid("poissons_ratio"))
      mooseError(name(), ": both youngs_modulus and poissons_ratio are required.");

    const Real E = getParam<Real>("youngs_modulus");
    const Real nu = getParam<Real>("poissons_ratio");
    if (E <= 0.0)
      paramError("youngs_modulus", "Young's modulus must be positive.");
    if (nu <= -1.0 || nu >= 0.5)
      paramError("poissons_ratio", "Poisson's ratio must be in (-1, 0.5).");
    _stiffness.fillSymmetricIsotropicEandNu(E, nu);
  }
  else
    _stiffness.fillFromInputVector(
        getParam<std::vector<Real>>("C_ijkl"),
        static_cast<RankFourTensor::FillMethod>(int(getParam<MooseEnum>("fill_method"))));
}

void
ADHomoxLinearElasticMaterial::computeQpProperties()
{
  _elasticity_tensor[_qp] = _stiffness;
  _stress[_qp] = _elasticity_tensor[_qp] * _mechanical_strain[_qp];
  _elastic_energy[_qp] = 0.5 * _stress[_qp].doubleContraction(_mechanical_strain[_qp]);
}
