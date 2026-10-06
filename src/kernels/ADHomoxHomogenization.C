#include "ADHomoxHomogenization.h"

#include "Assembly.h"
#include "MooseVariableScalar.h"
#include "libmesh/quadrature.h"

registerMooseObject("homoxApp", ADHomoxHomogenization);

InputParameters
ADHomoxHomogenization::validParams()
{
  InputParameters params = HomoxHomogenizationInterface<ADKernelScalarBase>::validParams();
  params.addClassDescription(
      "Linear-elastic small-strain stress divergence with volume-average strain/stress "
      "constraints. The elasticity_tensor material property must be the consistent derivative of "
      "stress with respect to mechanical strain.");
  params.addRequiredParam<unsigned int>("component", "Displacement equation component");
  params.addRequiredCoupledVar("displacements", "Displacement fluctuation variables");
  params.addParam<std::string>("base_name", "Optional material property prefix");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

ADHomoxHomogenization::ADHomoxHomogenization(const InputParameters & parameters)
  : HomoxHomogenizationInterface<ADKernelScalarBase>(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getADMaterialProperty<RankTwoTensor>(_base_name + "stress")),
    _mechanical_strain(getADMaterialProperty<RankTwoTensor>(_base_name + "mechanical_strain")),
    _elasticity_tensor(getADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _component(getParam<unsigned int>("component")),
    _ndisp(coupledComponents("displacements")),
    _disp_vars(_ndisp)
{
  if (_component >= _ndisp)
    paramError("component", "The component must be smaller than the displacement count.");
  if (_k_order != constraintMap().size())
    paramError("scalar_variable",
               "Its order must equal the number of active homogenization constraints (",
               constraintMap().size(),
               ").");

  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp_vars[i] = getVar("displacements", i);
}

std::set<std::string>
ADHomoxHomogenization::additionalROVariables()
{
  auto variables = ADKernelScalarBase::additionalROVariables();
  variables.insert(_kappa_var_ptr->name());
  return variables;
}

void
ADHomoxHomogenization::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError("ADHomoxHomogenization requires Cartesian coordinates.");
}

ADReal
ADHomoxHomogenization::computeQpResidual()
{
  return _stress[_qp].row(_component) * _grad_test[_i][_qp];
}

ADReal
ADHomoxHomogenization::computeScalarQpResidual()
{
  if (_component != 0)
    return 0.0;

  unsigned int component = 0;
  for (const auto & [indices, constraint] : constraintMap())
  {
    if (component++ != _h)
      continue;

    const auto [i, j] = indices;
    const auto [type, target] = constraint;
    const Real target_value = target->value(_t, _q_point[_qp]);
    if (type == HomoxHomogenization::ConstraintType::Stress)
      return _stress[_qp](i, j) - target_value;
    if (type == HomoxHomogenization::ConstraintType::Strain)
      return _mechanical_strain[_qp](i, j) - target_value;
  }

  mooseError("Scalar residual index lies outside the homogenization constraint map.");
}

void
ADHomoxHomogenization::computeResidual()
{
  if (_compute_field_residuals)
    ADKernel::computeResidual();
  if (_compute_scalar_residuals && _component == 0)
    computeScalarResidualManual();
}

void
ADHomoxHomogenization::computeJacobian()
{
  if (_compute_field_residuals)
    ADKernel::computeJacobian();
  if (_compute_scalar_residuals && _component == 0)
    computeScalarJacobianManual();
}

void
ADHomoxHomogenization::computeResidualAndJacobian()
{
  if (_compute_field_residuals)
    ADKernel::computeResidualAndJacobian();
  if (_compute_scalar_residuals && _component == 0)
  {
    computeScalarResidualManual();
    computeScalarJacobianManual();
  }
}

void
ADHomoxHomogenization::computeScalarResidualManual()
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
ADHomoxHomogenization::computeScalarJacobianManual()
{
  const auto & scalar_dofs = _kappa_var_ptr->dofIndices();

  for (unsigned int d = 0; d < _ndisp; ++d)
  {
    const auto & grad_phi = _disp_vars[d]->gradPhi();
    _local_ke.resize(_k_order, grad_phi.size());
    _local_ke.zero();

    for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
    {
      const Real dV = _JxW[_qp] * _coord[_qp];
      unsigned int row = 0;
      for (const auto & [indices, constraint] : constraintMap())
      {
        const auto [i, j] = indices;
        for (_j = 0; _j < grad_phi.size(); ++_j)
          if (constraint.first == HomoxHomogenization::ConstraintType::Stress)
            for (unsigned int n = 0; n < _ndisp; ++n)
              _local_ke(row, _j) +=
                  0.5 * dV *
                  raw_value(_elasticity_tensor[_qp](i, j, d, n) +
                            _elasticity_tensor[_qp](i, j, n, d)) *
                  grad_phi[_j][_qp](n);
          else
            _local_ke(row, _j) +=
                0.5 * dV *
                (Real(i == d) * grad_phi[_j][_qp](j) + Real(j == d) * grad_phi[_j][_qp](i));
        ++row;
      }
    }

    addJacobian(_assembly,
                _local_ke,
                scalar_dofs,
                _disp_vars[d]->dofIndices(),
                _kappa_var_ptr->scalingFactor());
  }

  _local_ke.resize(_k_order, _k_order);
  _local_ke.zero();
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    const Real dV = _JxW[_qp] * _coord[_qp];
    unsigned int row = 0;
    for (const auto & [row_indices, row_constraint] : constraintMap())
    {
      const auto [i, j] = row_indices;
      unsigned int column = 0;
      for (const auto & [column_indices, column_constraint] : constraintMap())
      {
        const auto [k, l] = column_indices;
        if (row_constraint.first == HomoxHomogenization::ConstraintType::Strain)
          _local_ke(row, column) +=
              dV * (Real(i == k && j == l) + Real(k != l && i == l && j == k));
        else
        {
          ADReal derivative = _elasticity_tensor[_qp](i, j, k, l);
          if (k != l)
            derivative += _elasticity_tensor[_qp](i, j, l, k);
          _local_ke(row, column) += dV * raw_value(derivative);
        }
        ++column;
      }
      ++row;
    }
  }

  addJacobian(_assembly, _local_ke, scalar_dofs, scalar_dofs, _kappa_var_ptr->scalingFactor());
}
