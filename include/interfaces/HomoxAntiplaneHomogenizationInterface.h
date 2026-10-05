#pragma once

#include "Function.h"
#include "InputParameters.h"

namespace HomoxAntiplaneHomogenization
{
const MultiMooseEnum constraint_types("strain stress none");

enum class ConstraintType
{
  Strain,
  Stress,
  None
};

using ConstraintMap =
    std::map<unsigned int, std::pair<ConstraintType, const Function *>>;
}

/** Adds two-component average antiplane strain/stress constraint parsing to a MOOSE object. */
template <class T>
class HomoxAntiplaneHomogenizationInterface : public T
{
public:
  static InputParameters validParams();

  HomoxAntiplaneHomogenizationInterface(const InputParameters & parameters);

protected:
  const HomoxAntiplaneHomogenization::ConstraintMap & constraintMap() const
  {
    return _constraint_map;
  }

private:
  HomoxAntiplaneHomogenization::ConstraintMap _constraint_map;
};

template <class T>
InputParameters
HomoxAntiplaneHomogenizationInterface<T>::validParams()
{
  InputParameters params = T::validParams();
  params.addRequiredParam<MultiMooseEnum>(
      "constraint_types",
      HomoxAntiplaneHomogenization::constraint_types,
      "Constraint type for gamma_xz and gamma_yz, in that order. Supply two entries.");
  params.addRequiredParam<std::vector<FunctionName>>(
      "targets", "Target functions for all non-none constraints, in component order.");
  return params;
}

template <class T>
HomoxAntiplaneHomogenizationInterface<T>::HomoxAntiplaneHomogenizationInterface(
    const InputParameters & parameters)
  : T(parameters)
{
  const auto types = parameters.get<MultiMooseEnum>("constraint_types");
  if (types.size() != 2)
    this->paramError("constraint_types", "Expected two entries, but received ", types.size(), ".");

  const auto & target_names = parameters.get<std::vector<FunctionName>>("targets");
  unsigned int target = 0;

  for (const auto component : make_range(2))
  {
    const auto type = static_cast<HomoxAntiplaneHomogenization::ConstraintType>(types.get(component));
    if (type == HomoxAntiplaneHomogenization::ConstraintType::None)
      continue;

    if (target >= target_names.size())
      this->paramError("targets", "A target is required for every non-none constraint.");

    _constraint_map[component] = {type, &this->getFunctionByName(target_names[target++])};
  }

  if (target != target_names.size())
    this->paramError("targets",
                     "Received ",
                     target_names.size(),
                     " targets, but only ",
                     target,
                     " constraints use them.");
}
