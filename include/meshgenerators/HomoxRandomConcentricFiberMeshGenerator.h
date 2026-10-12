#pragma once

#include "MeshGenerator.h"

/// Random fiber disks with concentric rings and a conforming all-TRI3 matrix.
class HomoxRandomConcentricFiberMeshGenerator : public MeshGenerator
{
public:
  static InputParameters validParams();
  HomoxRandomConcentricFiberMeshGenerator(const InputParameters & parameters);
  std::unique_ptr<MeshBase> generate() override;

protected:
  std::unique_ptr<MeshBase> matchPeriodicBoundaries(const MeshBase & input);
  std::unique_ptr<MeshBase> * _build_mesh;
};
