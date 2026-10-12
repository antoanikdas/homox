#include "HomoxRandomFiberMeshGenerator.h"

#include "MooseRandom.h"

#include "libmesh/boundary_info.h"
#include "libmesh/elem.h"
#include "libmesh/mesh_base.h"

#include <algorithm>
#include <cmath>

registerMooseObject("homoxApp", HomoxRandomFiberMeshGenerator);

InputParameters
HomoxRandomFiberMeshGenerator::validParams()
{
  InputParameters params = GeneratedMeshGenerator::validParams();
  params.set<MooseEnum>("dim") = "2";
  params.set<MooseEnum>("elem_type") = "QUAD4";

  params.addRequiredRangeCheckedParam<Real>(
      "radius", "radius > 0", "Common radius of the circular fibers");
  params.addParam<unsigned int>(
      "number_of_fibers", "Number of fibers; specify this or volume_fraction");
  params.addParam<Real>(
      "volume_fraction", "Nominal fiber area fraction; specify this or number_of_fibers");
  params.addParam<std::vector<Point>>("fiber_centers", {},
                                     "Explicit fiber centers; overrides random placement");
  params.addRangeCheckedParam<Real>("minimum_spacing",
                                    0.0,
                                    "minimum_spacing >= 0",
                                    "Minimum matrix spacing between neighboring fiber surfaces");
  params.addParam<bool>("periodic_geometry",
                        true,
                        "Wrap fiber geometry across opposite RVE boundaries");
  params.addParam<unsigned int>("seed", 1980, "Seed controlling the random fiber positions");
  params.addRangeCheckedParam<unsigned int>(
      "max_placement_attempts",
      100000,
      "max_placement_attempts > 0",
      "Maximum number of candidates tested while placing all fibers");
  params.addParam<SubdomainID>("matrix_block_id", 0, "Subdomain ID assigned to matrix elements");
  params.addParam<SubdomainID>("fiber_block_id", 1, "Subdomain ID assigned to fiber elements");
  params.addParam<SubdomainName>("matrix_block_name", "matrix", "Matrix subdomain name");
  params.addParam<SubdomainName>("fiber_block_name", "fiber", "Fiber subdomain name");
  params.addClassDescription(
      "Generates a structured two-dimensional RVE with reproducible, nonoverlapping random "
      "circular fibers and optional periodic geometric wrapping.");
  return params;
}

HomoxRandomFiberMeshGenerator::HomoxRandomFiberMeshGenerator(
    const InputParameters & parameters)
  : GeneratedMeshGenerator(parameters),
    _radius(getParam<Real>("radius")),
    _minimum_spacing(getParam<Real>("minimum_spacing")),
    _periodic_geometry(getParam<bool>("periodic_geometry")),
    _seed(getParam<unsigned int>("seed")),
    _max_placement_attempts(getParam<unsigned int>("max_placement_attempts")),
    _matrix_block_id(getParam<SubdomainID>("matrix_block_id")),
    _fiber_block_id(getParam<SubdomainID>("fiber_block_id")),
    _matrix_block_name(getParam<SubdomainName>("matrix_block_name")),
    _fiber_block_name(getParam<SubdomainName>("fiber_block_name")),
    _number_of_fibers(0)
{
  if (_dim != 2)
    paramError("dim", "HomoxRandomFiberMeshGenerator only supports two-dimensional meshes.");
  if (_matrix_block_id == _fiber_block_id)
    paramError("fiber_block_id", "The matrix and fiber block IDs must be different.");

  const Real width = _xmax - _xmin;
  const Real height = _ymax - _ymin;
  if (width <= 0.0 || height <= 0.0)
    paramError("xmax", "The RVE must have positive width and height.");
  if (2.0 * _radius >= std::min(width, height))
    paramError("radius", "The fiber diameter must be smaller than both RVE dimensions.");

  const bool has_number = isParamValid("number_of_fibers");
  const bool has_fraction = isParamValid("volume_fraction");
  if (has_number == has_fraction)
    mooseError("Specify exactly one of number_of_fibers or volume_fraction in ", name(), ".");

  if (has_number)
  {
    _number_of_fibers = getParam<unsigned int>("number_of_fibers");
    if (_number_of_fibers == 0)
      paramError("number_of_fibers", "At least one fiber is required.");
  }
  else
  {
    const Real volume_fraction = getParam<Real>("volume_fraction");
    if (volume_fraction <= 0.0 || volume_fraction >= 1.0)
      paramError("volume_fraction", "The volume fraction must lie strictly between zero and one.");
    _number_of_fibers = static_cast<unsigned int>(
        std::round(volume_fraction * width * height / (libMesh::pi * _radius * _radius)));
    if (_number_of_fibers == 0)
      paramError("volume_fraction",
                 "The requested volume fraction and radius produce fewer than one fiber.");
  }
}

Real
HomoxRandomFiberMeshGenerator::squaredDistance(const Point & first, const Point & second) const
{
  Real dx = std::abs(first(0) - second(0));
  Real dy = std::abs(first(1) - second(1));
  if (_periodic_geometry)
  {
    dx = std::min(dx, (_xmax - _xmin) - dx);
    dy = std::min(dy, (_ymax - _ymin) - dy);
  }
  return dx * dx + dy * dy;
}

std::vector<Point>
HomoxRandomFiberMeshGenerator::generateFiberCenters() const
{
  const auto & explicit_centers = getParam<std::vector<Point>>("fiber_centers");
  if (!explicit_centers.empty())
  {
    if (explicit_centers.size() != _number_of_fibers)
      paramError("fiber_centers", "Supply one center per requested fiber.");
    const Real minimum_distance = 2.0 * _radius + _minimum_spacing;
    for (unsigned int i = 0; i < explicit_centers.size(); ++i)
    {
      const auto & center = explicit_centers[i];
      const Real margin = _periodic_geometry ? 0.0 : _radius;
      if (!std::isfinite(center(0)) || !std::isfinite(center(1)) || center(2) != 0 ||
          center(0) < _xmin + margin || center(0) >= _xmax - margin ||
          center(1) < _ymin + margin || center(1) >= _ymax - margin)
        paramError("fiber_centers", "Centers must lie in the admissible XY domain.");
      for (unsigned int j = 0; j < i; ++j)
        if (squaredDistance(center, explicit_centers[j]) < minimum_distance * minimum_distance)
          paramError("fiber_centers", "Fibers overlap or violate minimum_spacing.");
    }
    return explicit_centers;
  }
  MooseRandom random;
  random.seed(0, _seed);

  const Real x_lower = _periodic_geometry ? _xmin : _xmin + _radius;
  const Real x_upper = _periodic_geometry ? _xmax : _xmax - _radius;
  const Real y_lower = _periodic_geometry ? _ymin : _ymin + _radius;
  const Real y_upper = _periodic_geometry ? _ymax : _ymax - _radius;
  const Real minimum_distance = 2.0 * _radius + _minimum_spacing;
  const Real minimum_distance_squared = minimum_distance * minimum_distance;

  std::vector<Point> centers;
  centers.reserve(_number_of_fibers);
  unsigned int attempts = 0;
  while (centers.size() < _number_of_fibers && attempts < _max_placement_attempts)
  {
    ++attempts;
    const Point candidate(x_lower + (x_upper - x_lower) * random.rand(0),
                          y_lower + (y_upper - y_lower) * random.rand(0),
                          0.0);

    bool overlaps = false;
    for (const auto & center : centers)
      if (squaredDistance(candidate, center) < minimum_distance_squared)
      {
        overlaps = true;
        break;
      }

    if (!overlaps)
      centers.push_back(candidate);
  }

  if (centers.size() != _number_of_fibers)
    mooseError("Unable to place ",
               _number_of_fibers,
               " nonoverlapping fibers after ",
               _max_placement_attempts,
               " attempts. Reduce radius, volume fraction, or minimum_spacing.");

  return centers;
}

std::unique_ptr<MeshBase>
HomoxRandomFiberMeshGenerator::generate()
{
  auto mesh = GeneratedMeshGenerator::generate();
  const auto centers = generateFiberCenters();
  unsigned int fiber_elements = 0;

  for (auto & element : mesh->element_ptr_range())
  {
    const Point centroid = element->vertex_average();
    bool in_fiber = false;
    for (const auto & center : centers)
      if (squaredDistance(centroid, center) <= _radius * _radius)
      {
        in_fiber = true;
        break;
      }

    element->subdomain_id() = in_fiber ? _fiber_block_id : _matrix_block_id;
    fiber_elements += in_fiber;
  }

  if (fiber_elements == 0)
    mooseError("The structured mesh does not resolve any fiber elements. Increase nx and ny or "
               "increase the fiber radius.");

  mesh->subdomain_name(_matrix_block_id) = _matrix_block_name;
  mesh->subdomain_name(_fiber_block_id) = _fiber_block_name;

  auto & boundary_info = mesh->get_boundary_info();
  constexpr boundary_id_type bottom_left_id = 100;
  constexpr boundary_id_type bottom_right_id = 101;
  boundary_info.nodeset_name(bottom_left_id) = "bottom_left";
  boundary_info.nodeset_name(bottom_right_id) = "bottom_right";

  const Real tolerance = 1e-12 * std::max(_xmax - _xmin, _ymax - _ymin);
  for (auto & node : mesh->node_ptr_range())
    if (std::abs((*node)(1) - _ymin) <= tolerance)
    {
      if (std::abs((*node)(0) - _xmin) <= tolerance)
        boundary_info.add_node(node, bottom_left_id);
      else if (std::abs((*node)(0) - _xmax) <= tolerance)
        boundary_info.add_node(node, bottom_right_id);
    }

  return mesh;
}
