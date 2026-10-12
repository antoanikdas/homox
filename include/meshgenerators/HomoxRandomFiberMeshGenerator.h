#pragma once

#include "GeneratedMeshGenerator.h"

/**
 * Generates a structured two-dimensional RVE containing randomly positioned circular fibers.
 *
 * Fibers are represented by assigning structured elements to the fiber block according to their
 * centroids. When periodic geometry is enabled, minimum-image distances wrap fibers across
 * opposite RVE boundaries.
 */
class HomoxRandomFiberMeshGenerator : public GeneratedMeshGenerator
{
public:
  static InputParameters validParams();

  HomoxRandomFiberMeshGenerator(const InputParameters & parameters);

  std::unique_ptr<MeshBase> generate() override;

protected:
  std::vector<Point> generateFiberCenters() const;
  Real squaredDistance(const Point & first, const Point & second) const;

  const Real _radius;
  const Real _minimum_spacing;
  const bool _periodic_geometry;
  const unsigned int _seed;
  const unsigned int _max_placement_attempts;
  const SubdomainID _matrix_block_id;
  const SubdomainID _fiber_block_id;
  const SubdomainName _matrix_block_name;
  const SubdomainName _fiber_block_name;
  unsigned int _number_of_fibers;
};
