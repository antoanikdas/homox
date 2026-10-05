#pragma once

#include "MeshGenerator.h"

/**
 * Builds a rectangular array of unit-square cells with one centered circular inclusion per cell.
 *
 * The central cell remains centered at the origin. The four directional counts specify how many
 * additional cells extend from that central cell.
 */
class HomoxMultiInclusionMeshGenerator : public MeshGenerator
{
public:
  static InputParameters validParams();

  HomoxMultiInclusionMeshGenerator(const InputParameters & parameters);

  std::unique_ptr<MeshBase> generate() override;

protected:
  const Real _radius;
  const unsigned int _n_left;
  const unsigned int _n_right;
  const unsigned int _n_top;
  const unsigned int _n_bottom;
  const unsigned int _num_sectors;
  const unsigned int _inclusion_rings;
  const unsigned int _matrix_rings;

  /// Final mesh produced by the internal framework mesh-generator chain.
  std::unique_ptr<MeshBase> * _build_mesh;
};

