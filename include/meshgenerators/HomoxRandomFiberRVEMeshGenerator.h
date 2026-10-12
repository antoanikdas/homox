#pragma once

#include "MeshGenerator.h"

/// One fiber realization with a selectable concentric TRI3 or structured QUAD4 representation.
class HomoxRandomFiberRVEMeshGenerator : public MeshGenerator
{
public:
  static InputParameters validParams();
  HomoxRandomFiberRVEMeshGenerator(const InputParameters & parameters);
  std::unique_ptr<MeshBase> generate() override;

protected:
  std::unique_ptr<MeshBase> * _build_mesh;
};
