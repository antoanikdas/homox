#pragma once

#include "InputParameters.h"
#include "MooseRandom.h"
#include "DelimitedFileReader.h"
#include <algorithm>
#include <cmath>

namespace HomoxFiberPlacement
{
struct Geometry
{
  std::vector<Point> centers;
  std::vector<Real> radii;
};

inline Geometry
readCSV(const FileName & filename)
{
  MooseUtils::DelimitedFileReader file(filename);
  file.setDelimiter(",");
  file.setHeaderFlag(MooseUtils::DelimitedFileReader::HeaderFlag::ON);
  file.read();
  const auto & names = file.getNames();
  if (names.size() != 3 || std::count(names.begin(), names.end(), "x") != 1 ||
      std::count(names.begin(), names.end(), "y") != 1 ||
      std::count(names.begin(), names.end(), "radius") != 1)
    ::mooseError("inclusions_file: CSV must have exactly the headers x,y,radius.");
  const auto & x = file.getData("x");
  const auto & y = file.getData("y");
  const auto & radius = file.getData("radius");
  if (x.empty())
    ::mooseError("inclusions_file: CSV must contain at least one inclusion.");
  Geometry geometry;
  for (std::size_t i = 0; i < x.size(); ++i)
  {
    if (!std::isfinite(x[i]) || !std::isfinite(y[i]) || !std::isfinite(radius[i]) || radius[i] <= 0)
      ::mooseError("inclusions_file: Centers must be finite and radii finite and positive (row ",
                   i + 2, ").");
    geometry.centers.emplace_back(x[i], y[i], 0);
    geometry.radii.push_back(radius[i]);
  }
  return geometry;
}

inline InputParameters
geometryParameters(const InputParameters & parameters, InputParameters result)
{
  result.applyParameters(parameters);
  if (parameters.isParamValid("inclusions_file") &&
      !parameters.get<FileName>("inclusions_file").empty())
  {
    if (parameters.isParamValid("radius") || parameters.isParamValid("number_of_fibers") ||
        !parameters.get<std::vector<Point>>("fiber_centers").empty() ||
        !parameters.get<std::vector<Real>>("fiber_radii").empty())
      ::mooseError("inclusions_file: Do not also specify radius, number_of_fibers, fiber_centers, or fiber_radii.");
    const auto geometry = readCSV(parameters.get<FileName>("inclusions_file"));
    result.set<std::vector<Point>>("fiber_centers") = geometry.centers;
    result.set<std::vector<Real>>("fiber_radii") = geometry.radii;
    result.set<unsigned int>("number_of_fibers") = geometry.centers.size();
    result.set<Real>("radius") = *std::max_element(geometry.radii.begin(), geometry.radii.end());
  }
  if (!result.isParamValid("radius") || !result.isParamValid("number_of_fibers"))
    ::mooseError("Supply inclusions_file or both radius and number_of_fibers.");
  return result;
}

inline std::vector<Real>
radii(const InputParameters & parameters)
{
  const auto count = parameters.get<unsigned int>("number_of_fibers");
  auto values = parameters.get<std::vector<Real>>("fiber_radii");
  if (values.empty())
    values.assign(count, parameters.get<Real>("radius"));
  if (values.size() != count)
    ::mooseError("fiber_radii: Supply one radius per fiber.");
  for (const Real value : values)
    if (!std::isfinite(value) || value <= 0)
      ::mooseError("fiber_radii: Radii must be finite and positive.");
  return values;
}

/// Shared geometry for fitted and structured representations of the same random-fiber RVE.
inline std::vector<Point>
concentricCenters(const InputParameters & parameters)
{
  const auto fiber_radii = radii(parameters);
  if (fiber_radii.empty())
    ::mooseError("number_of_fibers: At least one fiber is required.");
  const Real radius = *std::max_element(fiber_radii.begin(), fiber_radii.end());
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
  if (explicit_centers.empty() && !parameters.get<std::vector<Real>>("fiber_radii").empty())
    ::mooseError("fiber_radii: Individual radii require explicit fiber_centers or inclusions_file.");
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
    for (std::size_t i = 0; i < explicit_centers.size(); ++i)
    {
      const auto & center = explicit_centers[i];
      const Real extent = (fiber_radii[i] + parameters.get<Real>("matrix_shell_thickness")) *
                          std::sqrt(angle / std::sin(angle));
      const Real local_margin = periodic ? 0 : extent + parameters.get<Real>("boundary_buffer");
      if (!std::isfinite(center(0)) || !std::isfinite(center(1)) || center(2) != 0 ||
          center(0) < xmin + local_margin || center(0) >= xmax - local_margin ||
          center(1) < ymin + local_margin || center(1) >= ymax - local_margin)
        ::mooseError("fiber_centers", "Centers must lie in the admissible XY domain.");
      for (std::size_t j = 0; j < centers.size(); ++j)
      {
        const Real pair_distance = extent +
            (fiber_radii[j] + parameters.get<Real>("matrix_shell_thickness")) *
            std::sqrt(angle / std::sin(angle)) + parameters.get<Real>("minimum_spacing");
        if (separation_squared(center, centers[j]) < pair_distance * pair_distance)
          ::mooseError("fiber_centers", "Concentric shells overlap or violate minimum_spacing.");
      }
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
