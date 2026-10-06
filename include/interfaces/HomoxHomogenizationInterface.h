#pragma once

#include "Function.h"
#include "InputParameters.h"

namespace HomoxHomogenization
{
const MultiMooseEnum constraint_types("strain stress none");

enum class ConstraintType
{
  Strain,
  Stress,
  None
};

using ConstraintMap =
    std::map<std::pair<unsigned int, unsigned int>, std::pair<ConstraintType, const Function *>>;
}

/** Adds average strain/stress constraint parsing to a MOOSE object. */
template <class T>
class HomoxHomogenizationInterface : public T
{
public:
  static InputParameters validParams();

  HomoxHomogenizationInterface(const InputParameters & parameters);

protected:
  const HomoxHomogenization::ConstraintMap & constraintMap() const { return _constraint_map; }

private:
  HomoxHomogenization::ConstraintMap _constraint_map;
};

template <class T>
InputParameters
HomoxHomogenizationInterface<T>::validParams()
{
  InputParameters params = T::validParams();
  params.addRequiredParam<MultiMooseEnum>(
      "constraint_types",
      HomoxHomogenization::constraint_types,
      "Constraint type for each tensor component in column-major order. Supply nine entries.");
  params.addRequiredParam<std::vector<FunctionName>>(
      "targets", "Target functions for all non-none constraints, in constraint order.");
  return params;
}

template <class T>
HomoxHomogenizationInterface<T>::HomoxHomogenizationInterface(const InputParameters & parameters)
  : T(parameters)
{
  const auto types = parameters.get<MultiMooseEnum>("constraint_types");
  if (types.size() != Moose::dim * Moose::dim)
    this->paramError("constraint_types",
                     "Expected ",
                     Moose::dim * Moose::dim,
                     " entries, but received ",
                     types.size(),
                     ".");

  const auto & target_names = parameters.get<std::vector<FunctionName>>("targets");
  unsigned int target = 0;

  for (const auto j : make_range(Moose::dim))
    for (const auto i : make_range(Moose::dim))
    {
      const auto type =
          static_cast<HomoxHomogenization::ConstraintType>(types.get(i + Moose::dim * j));
      if (type == HomoxHomogenization::ConstraintType::None)
        continue;

      if (target >= target_names.size())
        this->paramError("targets", "A target is required for every non-none constraint.");

      if (i != j && _constraint_map.count({j, i}))
        this->paramError(
            "constraint_types",
            "Tensor components (",
            i,
            ", ",
            j,
            ") and (",
            j,
            ", ",
            i,
            ") cannot both be constrained because strain and stress are symmetric.");

      _constraint_map[{i, j}] = {type, &this->getFunctionByName(target_names[target++])};
    }

  if (target != target_names.size())
    this->paramError("targets",
                     "Received ",
                     target_names.size(),
                     " targets, but only ",
                     target,
                     " constraints use them.");
}
