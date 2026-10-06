#include "ADHomoxAntiplaneHomogenization.h"

#include "Assembly.h"
#include "MooseVariableScalar.h"
#include "libmesh/quadrature.h"

registerMooseObject("homoxApp", ADHomoxAntiplaneHomogenization);

InputParameters
ADHomoxAntiplaneHomogenization::validParams()
{
  InputParameters params =
      HomoxAntiplaneHomogenizationInterface<ADKernelScalarBase>::validParams();
  params.addClassDescription(
      "Linear-elastic antiplane stress divergence with volume-average gradient/stress constraints. "
      "The antiplane_stiffness material property must be the consistent derivative of stress with "
      "respect to strain.");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

ADHomoxAntiplaneHomogenization::ADHomoxAntiplaneHomogenization(
    const InputParameters & parameters)
  : HomoxAntiplaneHomogenizationInterface<ADKernelScalarBase>(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _antiplane_stress(
        getADMaterialProperty<RealVectorValue>(_base_name + "antiplane_stress")),
    _antiplane_strain(
        getADMaterialProperty<RealVectorValue>(_base_name + "antiplane_strain")),
    _antiplane_stiffness(
        getADMaterialProperty<RankTwoTensor>(_base_name + "antiplane_stiffness"))
{
  if (_k_order != constraintMap().size())
    paramError("scalar_variable",
               "Its order must equal the number of active antiplane constraints (",
               constraintMap().size(),
               ").");
}

std::set<std::string>
ADHomoxAntiplaneHomogenization::additionalROVariables()
{
  auto variables = ADKernelScalarBase::additionalROVariables();
  variables.insert(_kappa_var_ptr->name());
  return variables;
}

void
ADHomoxAntiplaneHomogenization::initialSetup()
{
  if (_mesh.dimension() != 2 || getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError("ADHomoxAntiplaneHomogenization requires a two-dimensional Cartesian mesh.");
}

ADReal
ADHomoxAntiplaneHomogenization::computeQpResidual()
{
  return _antiplane_stress[_qp] * _grad_test[_i][_qp];
}

ADReal
ADHomoxAntiplaneHomogenization::computeScalarQpResidual()
{
  unsigned int row = 0;
  for (const auto & [component, constraint] : constraintMap())
  {
    if (row++ != _h)
      continue;

    const auto [type, target] = constraint;
    const Real target_value = target->value(_t, _q_point[_qp]);
    if (type == HomoxAntiplaneHomogenization::ConstraintType::Stress)
      return _antiplane_stress[_qp](component) - target_value;
    if (type == HomoxAntiplaneHomogenization::ConstraintType::Strain)
      return _antiplane_strain[_qp](component) - target_value;
  }

  mooseError("Scalar residual index lies outside the antiplane constraint map.");
}

void
ADHomoxAntiplaneHomogenization::computeResidual()
{
  if (_compute_field_residuals)
    ADKernel::computeResidual();
  if (_compute_scalar_residuals)
    computeScalarResidualManual();
}

void
ADHomoxAntiplaneHomogenization::computeJacobian()
{
  if (_compute_field_residuals)
    ADKernel::computeJacobian();
  if (_compute_scalar_residuals)
    computeScalarJacobianManual();
}

void
ADHomoxAntiplaneHomogenization::computeResidualAndJacobian()
{
  if (_compute_field_residuals)
    ADKernel::computeResidualAndJacobian();
  if (_compute_scalar_residuals)
  {
    computeScalarResidualManual();
    computeScalarJacobianManual();
  }
}

void
ADHomoxAntiplaneHomogenization::computeScalarResidualManual()
{
  std::vector<Real> residual(_k_order);
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    const Real dV = _JxW[_qp] * _coord[_qp];
    for (_h = 0; _h < _k_order; ++_h)
      residual[_h] += dV * raw_value(computeScalarQpResidual());
  }
  addResiduals(_assembly, residual, _kappa_var_ptr->dofIndices(), _kappa_var_ptr->scalingFactor());
}

void
ADHomoxAntiplaneHomogenization::computeScalarJacobianManual()
{
  const auto & scalar_dofs = _kappa_var_ptr->dofIndices();
  const auto & grad_phi = _var.gradPhi();

  _local_ke.resize(_k_order, grad_phi.size());
  _local_ke.zero();
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    const Real dV = _JxW[_qp] * _coord[_qp];
    unsigned int row = 0;
    for (const auto & [component, constraint] : constraintMap())
    {
      for (_j = 0; _j < grad_phi.size(); ++_j)
        if (constraint.first == HomoxAntiplaneHomogenization::ConstraintType::Stress)
          for (const auto direction : make_range(2))
            _local_ke(row, _j) += dV * raw_value(_antiplane_stiffness[_qp](component, direction)) *
                                  grad_phi[_j][_qp](direction);
        else
          _local_ke(row, _j) += dV * grad_phi[_j][_qp](component);
      ++row;
    }
  }
  addJacobian(_assembly,
              _local_ke,
              scalar_dofs,
              _var.dofIndices(),
              _kappa_var_ptr->scalingFactor());

  _local_ke.resize(_k_order, _k_order);
  _local_ke.zero();
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    const Real dV = _JxW[_qp] * _coord[_qp];
    unsigned int row = 0;
    for (const auto & [row_component, row_constraint] : constraintMap())
    {
      unsigned int column = 0;
      for (const auto & [column_component, column_constraint] : constraintMap())
      {
        if (row_constraint.first == HomoxAntiplaneHomogenization::ConstraintType::Strain)
          _local_ke(row, column) += dV * Real(row_component == column_component);
        else
          _local_ke(row, column) +=
              dV * raw_value(_antiplane_stiffness[_qp](row_component, column_component));
        ++column;
      }
      ++row;
    }
  }
  addJacobian(_assembly, _local_ke, scalar_dofs, scalar_dofs, _kappa_var_ptr->scalingFactor());
}
