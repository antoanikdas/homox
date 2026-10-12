#include "HomoxRandomFiberRVEMeshGenerator.h"
#include "HomoxRandomConcentricFiberMeshGenerator.h"
#include "HomoxFiberPlacement.h"
#include "libmesh/mesh_base.h"

registerMooseObject("homoxApp", HomoxRandomFiberRVEMeshGenerator);

InputParameters
HomoxRandomFiberRVEMeshGenerator::validParams()
{
  InputParameters params = HomoxRandomConcentricFiberMeshGenerator::validParams();
  params.addParam<MooseEnum>("mesh_type", MooseEnum("concentric structured", "concentric"),
                             "Concentric all-TRI3 or structured QUAD4 mesh of the same fibers");
  params.addRangeCheckedParam<unsigned int>("nx", 128, "nx > 0",
                                           "Structured elements in the x direction");
  params.addRangeCheckedParam<unsigned int>("ny", 128, "ny > 0",
                                           "Structured elements in the y direction");
  params.addClassDescription(
      "Selects fitted concentric or centroid-assigned structured meshing while preserving "
      "identical fiber centers, radii, spacing rules, and periodic geometry.");
  return params;
}

HomoxRandomFiberRVEMeshGenerator::HomoxRandomFiberRVEMeshGenerator(
    const InputParameters & parameters)
  : MeshGenerator(parameters)
{
  declareMeshProperty("use_distributed_mesh", false);
  const auto centers = HomoxFiberPlacement::concentricCenters(parameters);
  declareMeshProperty("fiber_centers", centers);
  const bool structured = getParam<MooseEnum>("mesh_type") == "structured";
  const std::string generator_type = structured ? "HomoxRandomFiberMeshGenerator"
                                               : "HomoxRandomConcentricFiberMeshGenerator";
  auto child = _app.getFactory().getValidParams(generator_type);
  child.applyParameters(parameters);
  child.set<std::vector<Point>>("fiber_centers") = centers;
  if (structured)
  {
    child.set<SubdomainID>("matrix_block_id") = 2;
    child.set<SubdomainID>("fiber_block_id") = 1;
  }
  const MeshGeneratorName mesh = name() + "_" + std::string(getParam<MooseEnum>("mesh_type"));
  addMeshSubgenerator(generator_type, mesh, child);
  _build_mesh = &getMeshByName(mesh);
}

std::unique_ptr<MeshBase>
HomoxRandomFiberRVEMeshGenerator::generate()
{
  return std::move(*_build_mesh);
}
