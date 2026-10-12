#include "HomoxRandomConcentricFiberMeshGenerator.h"
#include "HomoxFiberPlacement.h"
#include "libmesh/mesh_base.h"
#include "libmesh/replicated_mesh.h"
#include "libmesh/elem.h"
#include "libmesh/boundary_info.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <array>

registerMooseObject("homoxApp", HomoxRandomConcentricFiberMeshGenerator);

InputParameters
HomoxRandomConcentricFiberMeshGenerator::validParams()
{
  InputParameters params = MeshGenerator::validParams();
  params.addRangeCheckedParam<Real>("radius", "radius > 0", "Common fiber radius without CSV input");
  params.addRangeCheckedParam<unsigned int>(
      "number_of_fibers", "number_of_fibers > 0", "Number of fibers in one RVE");
  params.addParam<FileName>("inclusions_file", "CSV file with x,y,radius headers; replaces random placement");
  params.addParam<std::vector<Real>>("fiber_radii", {}, "Optional individual radii for explicit centers");
  params.addParam<Real>("xmin", 0, "Minimum x coordinate");
  params.addParam<Real>("xmax", 1, "Maximum x coordinate");
  params.addParam<Real>("ymin", 0, "Minimum y coordinate");
  params.addParam<Real>("ymax", 1, "Maximum y coordinate");
  params.addParam<bool>("periodic_geometry", false,
                        "Wrap fibers across boundaries and match opposite exterior nodes");
  params.addParam<std::vector<Point>>("fiber_centers", {},
                                     "Optional explicit centers for verification; overrides random placement");
  params.addRangeCheckedParam<Real>("matrix_shell_thickness", 0.01,
                                    "matrix_shell_thickness > 0",
                                    "Thickness of the concentric matrix shell around each fiber");
  params.addRangeCheckedParam<Real>("minimum_spacing", 0.01, "minimum_spacing > 0",
                                    "Minimum gap between the outer concentric shells");
  params.addRangeCheckedParam<Real>("boundary_buffer", 0.01, "boundary_buffer > 0",
                                    "Minimum gap from outer shells to the rectangular boundary");
  params.addRangeCheckedParam<unsigned int>("num_sectors", 8, "num_sectors > 0",
                                           "Even number of sectors per quadrant");
  params.addRangeCheckedParam<unsigned int>("fiber_rings", 4, "fiber_rings > 0",
                                           "Number of concentric rings within each fiber");
  params.addRangeCheckedParam<unsigned int>("matrix_rings", 2, "matrix_rings > 0",
                                           "Number of concentric rings in each matrix shell");
  params.addRangeCheckedParam<unsigned int>("boundary_segments_x", 32,
                                           "boundary_segments_x > 0",
                                           "Matching segment counts on bottom and top");
  params.addRangeCheckedParam<unsigned int>("boundary_segments_y", 32,
                                           "boundary_segments_y > 0",
                                           "Matching segment counts on left and right");
  params.addRangeCheckedParam<Real>("desired_area", 0.001, "desired_area > 0",
                                    "Maximum area requested for matrix triangles");
  params.addParam<unsigned int>("seed", 1980, "Seed for reproducible fiber positions");
  params.addRangeCheckedParam<unsigned int>("max_placement_attempts", 100000,
                                           "max_placement_attempts > 0",
                                           "Maximum number of random placement candidates");
  params.addClassDescription(
      "Generates an all-TRI3 mesh of random concentric fibers and a triangulated matrix, "
      "with optional periodic geometric wrapping and matching exterior nodes.");
  return params;
}

HomoxRandomConcentricFiberMeshGenerator::HomoxRandomConcentricFiberMeshGenerator(
    const InputParameters & parameters)
  : MeshGenerator(parameters)
{
  declareMeshProperty("use_distributed_mesh", false);
  const auto geometry_parameters = HomoxFiberPlacement::geometryParameters(parameters, validParams());
  const auto centers = HomoxFiberPlacement::concentricCenters(geometry_parameters);
  const auto radii = HomoxFiberPlacement::radii(geometry_parameters);
  const Real radius = *std::max_element(radii.begin(), radii.end());
  const bool periodic = getParam<bool>("periodic_geometry");
  const Real xmin = getParam<Real>("xmin"), xmax = getParam<Real>("xmax");
  const Real ymin = getParam<Real>("ymin"), ymax = getParam<Real>("ymax");
  const unsigned int sectors = getParam<unsigned int>("num_sectors");
  const Real outer_radius = radius + getParam<Real>("matrix_shell_thickness");
  const Real angle = libMesh::pi / (2.0 * sectors);
  const Real shell_extent = outer_radius * std::sqrt(angle / std::sin(angle));
  const Real padding = shell_extent + getParam<Real>("boundary_buffer");

  const MeshGeneratorName boundary = name() + "_boundary";
  const Real width = xmax - xmin, height = ymax - ymin;
  const Real extended_xmin = periodic ? xmin - width - padding : xmin;
  const Real extended_xmax = periodic ? xmax + width + padding : xmax;
  const Real extended_ymin = periodic ? ymin - height - padding : ymin;
  const Real extended_ymax = periodic ? ymax + height + padding : ymax;
  auto boundary_params = _app.getFactory().getValidParams("PolyLineMeshGenerator");
  boundary_params.set<std::vector<Point>>("points") =
      {Point(extended_xmin, extended_ymin), Point(extended_xmax, extended_ymin),
       Point(extended_xmax, extended_ymax), Point(extended_xmin, extended_ymax)};
  boundary_params.set<bool>("loop") = true;
  const auto nx = getParam<unsigned int>("boundary_segments_x");
  const auto ny = getParam<unsigned int>("boundary_segments_y");
  boundary_params.set<std::vector<unsigned int>>("nums_edges_between_points") = {nx, ny, nx, ny};
  addMeshSubgenerator("PolyLineMeshGenerator", boundary, boundary_params);

  std::vector<MeshGeneratorName> holes;
  std::vector<Point> image_centers;
  for (int ix = periodic ? -1 : 0; ix <= (periodic ? 1 : 0); ++ix)
    for (int iy = periodic ? -1 : 0; iy <= (periodic ? 1 : 0); ++iy)
      for (const auto & center : centers)
        image_centers.push_back(center + Point(ix * width, iy * height));
  for (unsigned int i = 0; i < image_centers.size(); ++i)
  {
    const Real fiber_radius = radii[i % centers.size()];
    const MeshGeneratorName disk = name() + "_disk_" + std::to_string(i);
    auto disk_params = _app.getFactory().getValidParams("ConcentricCircleMeshGenerator");
    disk_params.set<unsigned int>("num_sectors") = sectors;
    disk_params.set<std::vector<Real>>("radii") =
        {fiber_radius, fiber_radius + getParam<Real>("matrix_shell_thickness")};
    disk_params.set<std::vector<unsigned int>>("rings") =
        {getParam<unsigned int>("fiber_rings"), getParam<unsigned int>("matrix_rings")};
    disk_params.set<bool>("has_outer_square") = false;
    disk_params.set<bool>("preserve_volumes") = true;
    disk_params.set<unsigned int>("smoothing_max_it") = 0;
    addMeshSubgenerator("ConcentricCircleMeshGenerator", disk, disk_params);

    const MeshGeneratorName translated = disk + "_translated";
    auto translation = _app.getFactory().getValidParams("TransformGenerator");
    translation.set<MeshGeneratorName>("input") = disk;
    translation.set<MooseEnum>("transform") = "TRANSLATE";
    translation.set<RealVectorValue>("vector_value") = RealVectorValue(image_centers[i]);
    addMeshSubgenerator("TransformGenerator", translated, translation);
    holes.push_back(translated);
  }

  const MeshGeneratorName matrix = name() + "_matrix";
  auto matrix_params = _app.getFactory().getValidParams("XYDelaunayGenerator");
  matrix_params.set<MeshGeneratorName>("boundary") = boundary;
  matrix_params.set<std::vector<MeshGeneratorName>>("holes") = holes;
  matrix_params.set<std::vector<bool>>("stitch_holes") = std::vector<bool>(holes.size(), true);
  matrix_params.set<std::vector<bool>>("refine_holes") = std::vector<bool>(holes.size(), false);
  matrix_params.set<bool>("refine_boundary") = false;
  // Periodic image separation is checked analytically, including the polygon circumradius.
  // Avoid ray-casting ambiguity between translated holes with aligned vertices.
  matrix_params.set<bool>("verify_holes") = !periodic;
  matrix_params.set<Real>("desired_area") = getParam<Real>("desired_area");
  matrix_params.set<SubdomainID>("output_subdomain_id") = 3;
  matrix_params.set<MooseEnum>("tri_element_type") = "TRI3";
  addMeshSubgenerator("XYDelaunayGenerator", matrix, matrix_params);

  const MeshGeneratorName triangles = name() + "_triangles";
  auto triangle_params = _app.getFactory().getValidParams("ElementsToSimplicesConverter");
  triangle_params.set<MeshGeneratorName>("input") = matrix;
  addMeshSubgenerator("ElementsToSimplicesConverter", triangles, triangle_params);

  const MeshGeneratorName blocks = name() + "_blocks";
  auto block_params = _app.getFactory().getValidParams("RenameBlockGenerator");
  block_params.set<MeshGeneratorName>("input") = triangles;
  block_params.set<std::vector<SubdomainName>>("old_block") = {"1", "2", "3"};
  block_params.set<std::vector<SubdomainName>>("new_block") = {"fiber", "matrix", "matrix"};
  addMeshSubgenerator("RenameBlockGenerator", blocks, block_params);

  if (periodic)
  {
    MeshGeneratorName previous = blocks;
    const std::vector<std::vector<Real>> lines =
        {{-1, 0, xmin}, {1, 0, -xmax}, {0, -1, ymin}, {0, 1, -ymax}};
    for (unsigned int i = 0; i < lines.size(); ++i)
    {
      const MeshGeneratorName cut = name() + "_cut_" + std::to_string(i);
      auto cut_params = _app.getFactory().getValidParams("XYMeshLineCutter");
      cut_params.set<MeshGeneratorName>("input") = previous;
      cut_params.set<std::vector<Real>>("cut_line_params") = lines[i];
      cut_params.set<boundary_id_type>("new_boundary_id") = 1000 + i;
      addMeshSubgenerator("XYMeshLineCutter", cut, cut_params);
      previous = cut;
    }
    _build_mesh = &getMeshByName(previous);
    return;
  }

  const MeshGeneratorName sides = name() + "_sides";
  auto side_params = _app.getFactory().getValidParams("SideSetsFromNormalsGenerator");
  side_params.set<MeshGeneratorName>("input") = blocks;
  side_params.set<std::vector<BoundaryName>>("new_boundary") = {"left", "right", "bottom", "top"};
  side_params.set<std::vector<Point>>("normals") =
      {Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)};
  addMeshSubgenerator("SideSetsFromNormalsGenerator", sides, side_params);

  const MeshGeneratorName pins = name() + "_pins";
  auto pin_params = _app.getFactory().getValidParams("ExtraNodesetGenerator");
  pin_params.set<MeshGeneratorName>("input") = sides;
  pin_params.set<std::vector<BoundaryName>>("new_boundary") = {"bottom_left"};
  pin_params.set<std::vector<std::vector<Real>>>("coord") = {{xmin, ymin, 0}};
  addMeshSubgenerator("ExtraNodesetGenerator", pins, pin_params);

  const MeshGeneratorName final = name() + "_final";
  pin_params.set<MeshGeneratorName>("input") = pins;
  pin_params.set<std::vector<BoundaryName>>("new_boundary") = {"bottom_right"};
  pin_params.set<std::vector<std::vector<Real>>>("coord") = {{xmax, ymin, 0}};
  addMeshSubgenerator("ExtraNodesetGenerator", final, pin_params);
  _build_mesh = &getMeshByName(final);
}

std::unique_ptr<MeshBase>
HomoxRandomConcentricFiberMeshGenerator::matchPeriodicBoundaries(const MeshBase & input)
{
  const std::array<Real, 2> low = {getParam<Real>("xmin"), getParam<Real>("ymin")};
  const std::array<Real, 2> high = {getParam<Real>("xmax"), getParam<Real>("ymax")};
  const Real tolerance = 1e-11 * std::min(high[0] - low[0], high[1] - low[1]);
  std::array<std::vector<Real>, 2> breaks;
  for (const auto * node : input.node_ptr_range())
    for (unsigned int axis = 0; axis < 2; ++axis)
      if (std::abs((*node)(axis) - low[axis]) < tolerance ||
          std::abs((*node)(axis) - high[axis]) < tolerance)
        breaks[axis].push_back((*node)(1 - axis));

  // Use the union of cut-edge nodes on both sides; refine only boundary-adjacent triangles.
  for (auto & coordinates : breaks)
  {
    std::sort(coordinates.begin(), coordinates.end());
    std::vector<Real> unique;
    for (const Real coordinate : coordinates)
      if (unique.empty() || coordinate - unique.back() > tolerance)
        unique.push_back(coordinate);
    coordinates = std::move(unique);
  }

  auto mesh = buildReplicatedMesh(2);
  std::map<std::pair<long long, long long>, Node *> nodes;
  auto get_node = [&](Point point) -> Node *
  {
    for (unsigned int axis = 0; axis < 2; ++axis)
      for (const Real bound : {low[axis], high[axis]})
        if (std::abs(point(axis) - bound) < tolerance)
        {
          point(axis) = bound;
          const auto & coordinates = breaks[axis];
          auto position = std::lower_bound(coordinates.begin(), coordinates.end(),
                                           point(1 - axis) - tolerance);
          if (position != coordinates.end() && std::abs(*position - point(1 - axis)) <= tolerance)
            point(1 - axis) = *position;
        }
    const auto key = std::make_pair(std::llround((point(0) - low[0]) / tolerance),
                                    std::llround((point(1) - low[1]) / tolerance));
    auto found = nodes.find(key);
    if (found != nodes.end())
      return found->second;
    Node * node = mesh->add_point(point);
    nodes.emplace(key, node);
    return node;
  };
  auto add_triangle = [&](Node * first, Node * second, Node * third, SubdomainID block)
  {
    const Real signed_area = ((*second - *first).cross(*third - *first))(2);
    if (signed_area == 0 || first == second || first == third || second == third)
      mooseError("Degenerate triangle encountered while matching periodic boundaries.");
    auto triangle = Elem::build(TRI3);
    triangle->set_node(0) = first;
    triangle->set_node(1) = signed_area > 0 ? second : third;
    triangle->set_node(2) = signed_area > 0 ? third : second;
    triangle->subdomain_id() = block;
    mesh->add_elem(std::move(triangle));
  };

  for (const auto * element : input.active_element_ptr_range())
  {
    if (element->type() != TRI3)
      mooseError("Periodic concentric mesh clipping requires TRI3 elements.");
    std::vector<Node *> polygon;
    for (unsigned int side = 0; side < 3; ++side)
    {
      Node * first = get_node(element->point(side));
      Node * second = get_node(element->point((side + 1) % 3));
      polygon.push_back(first);
      for (unsigned int axis = 0; axis < 2; ++axis)
        for (const Real bound : {low[axis], high[axis]})
          if (std::abs((*first)(axis) - bound) < tolerance &&
              std::abs((*second)(axis) - bound) < tolerance)
          {
            const Real start = (*first)(1 - axis), end = (*second)(1 - axis);
            std::vector<Real> inside;
            const auto & coordinates = breaks[axis];
            for (auto pos = std::upper_bound(coordinates.begin(), coordinates.end(),
                                              std::min(start, end) + tolerance);
                 pos != coordinates.end() && *pos < std::max(start, end) - tolerance; ++pos)
              inside.push_back(*pos);
            if (end < start)
              std::reverse(inside.begin(), inside.end());
            for (const Real coordinate : inside)
            {
              Point point;
              point(axis) = bound;
              point(1 - axis) = coordinate;
              polygon.push_back(get_node(point));
            }
          }
    }
    if (polygon.size() == 3)
      add_triangle(polygon[0], polygon[1], polygon[2], element->subdomain_id());
    else
    {
      Node * center = get_node(element->vertex_average());
      for (unsigned int i = 0; i < polygon.size(); ++i)
        add_triangle(center, polygon[i], polygon[(i + 1) % polygon.size()], element->subdomain_id());
    }
  }
  mesh->subdomain_name(1) = "fiber";
  mesh->subdomain_name(2) = "matrix";
  mesh->prepare_for_use();
  auto & info = mesh->get_boundary_info();
  const std::array<std::string, 4> names = {"left", "right", "bottom", "top"};
  for (unsigned int id = 0; id < names.size(); ++id)
  {
    info.sideset_name(id) = names[id];
    info.nodeset_name(id) = names[id];
  }
  for (const auto * element : mesh->active_element_ptr_range())
    for (unsigned int side = 0; side < 3; ++side)
      if (!element->neighbor_ptr(side))
      {
        const Point & first = element->point(side);
        const Point & second = element->point((side + 1) % 3);
        bool found = false;
        for (unsigned int axis = 0; axis < 2; ++axis)
          for (unsigned int upper = 0; upper < 2; ++upper)
          {
            const Real bound = upper ? high[axis] : low[axis];
            if (std::abs(first(axis) - bound) < tolerance &&
                std::abs(second(axis) - bound) < tolerance)
            {
              info.add_side(element, side, 2 * axis + upper);
              found = true;
            }
          }
        if (!found)
          mooseError("Unstitched internal edge detected in periodic concentric mesh.");
      }
  info.nodeset_name(100) = "bottom_left";
  info.nodeset_name(101) = "bottom_right";
  for (const auto * node : mesh->node_ptr_range())
    if (std::abs((*node)(1) - low[1]) < tolerance)
    {
      if (std::abs((*node)(0) - low[0]) < tolerance)
        info.add_node(node, 100);
      if (std::abs((*node)(0) - high[0]) < tolerance)
        info.add_node(node, 101);
    }
  return mesh;
}

std::unique_ptr<MeshBase>
HomoxRandomConcentricFiberMeshGenerator::generate()
{
  if (getParam<bool>("periodic_geometry"))
  {
    auto input = std::move(*_build_mesh);
    return matchPeriodicBoundaries(*input);
  }
  return std::move(*_build_mesh);
}
