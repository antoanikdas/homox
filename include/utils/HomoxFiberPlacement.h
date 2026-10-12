#pragma once

#include "InputParameters.h"
#include "MooseRandom.h"
#include <algorithm>
#include <cmath>

namespace HomoxFiberPlacement
{
/// Shared geometry for fitted and structured representations of the same random-fiber RVE.
inline std::vector<Point>
concentricCenters(const InputParameters & parameters)
{
  const Real radius = parameters.get<Real>("radius");
  const bool periodic = parameters.get<bool>("periodic_geometry");
  const Real xmin = parameters.get<Real>("xmin"), xmax = parameters.get<Real>("xmax");
  const Real ymin = parameters.get<Real>("ymin"), ymax = parameters.get<Real>("ymax");
  const unsigned int sectors = parameters.get<unsigned int>("num_sectors");
  if (sectors % 2)
    ::mooseError("num_sectors", "The number of sectors must be even.");
  if (!std::isfinite(radius) || !std::isfinite(xmin) || !std::isfinite(xmax) ||
      !std::isfinite(ymin) || !std::isfinite(ymax) || xmax <= xmin || ymax <= ymin)
    ::mooseError("xmax", "Supply finite coordinates and a finite radius with positive RVE dimensions.");

  const Real outer_radius = radius + parameters.get<Real>("matrix_shell_thickness");
  // Volume preservation expands the polygon circumradius slightly beyond its nominal radius.
  const Real angle = libMesh::pi / (2.0 * sectors);
  const Real shell_extent = outer_radius * std::sqrt(angle / std::sin(angle));
  const Real padding = shell_extent + parameters.get<Real>("boundary_buffer");
  const Real margin = periodic ? 0.0 : padding;
  const Real distance = 2.0 * shell_extent + parameters.get<Real>("minimum_spacing");
  if (2.0 * (periodic ? shell_extent : margin) >= std::min(xmax - xmin, ymax - ymin))
    ::mooseError("radius", "The concentric shell and boundary buffer do not fit inside the RVE.");

  MooseRandom random;
  random.seed(0, parameters.get<unsigned int>("seed"));
  std::vector<Point> centers;
  const auto count = parameters.get<unsigned int>("number_of_fibers");
  const auto max_attempts = parameters.get<unsigned int>("max_placement_attempts");
  const auto & explicit_centers = parameters.get<std::vector<Point>>("fiber_centers");
  auto separation_squared = [&](const Point & first, const Point & second)
  {
    Real dx = std::abs(first(0) - second(0));
    Real dy = std::abs(first(1) - second(1));
    if (periodic)
    {
      dx = std::min(dx, xmax - xmin - dx);
      dy = std::min(dy, ymax - ymin - dy);
    }
    return dx * dx + dy * dy;
  };
  if (!explicit_centers.empty())
  {
    if (explicit_centers.size() != count)
      ::mooseError("fiber_centers", "Supply one center per requested fiber.");
    for (const auto & center : explicit_centers)
    {
      if (!std::isfinite(center(0)) || !std::isfinite(center(1)) || center(2) != 0 ||
          center(0) < xmin + margin || center(0) >= xmax - margin ||
          center(1) < ymin + margin || center(1) >= ymax - margin)
        ::mooseError("fiber_centers", "Centers must lie in the admissible XY domain.");
      for (const auto & previous : centers)
        if (separation_squared(center, previous) < distance * distance)
          ::mooseError("fiber_centers", "Concentric shells overlap or violate minimum_spacing.");
      centers.push_back(center);
    }
  }
  for (unsigned int attempt = 0; attempt < max_attempts && centers.size() < count; ++attempt)
  {
    const Real x = xmin + margin + (xmax - xmin - 2.0 * margin) * random.rand(0);
    const Real y = ymin + margin + (ymax - ymin - 2.0 * margin) * random.rand(0);
    const Point candidate(x, y, 0);
    bool overlaps = false;
    for (const auto & center : centers)
      if (separation_squared(candidate, center) < distance * distance)
      {
        overlaps = true;
        break;
      }
    if (!overlaps)
      centers.push_back(candidate);
  }
  if (centers.size() != count)
    ::mooseError("Unable to place requested concentric fibers. Placed ", centers.size(), " of ", count,
               ". Reduce fiber count, radius, shell thickness, or spacing.");

  return centers;
}
}
