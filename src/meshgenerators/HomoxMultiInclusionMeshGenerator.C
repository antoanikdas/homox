#include "HomoxMultiInclusionMeshGenerator.h"

registerMooseObject("homoxApp", HomoxMultiInclusionMeshGenerator);

InputParameters
HomoxMultiInclusionMeshGenerator::validParams()
{
  InputParameters params = MeshGenerator::validParams();
  params.addRequiredParam<Real>("radius", "Radius of the centered inclusion in each unit cell");
  params.addParam<unsigned int>("n_left", 0, "Number of cells to the left of the central cell");
  params.addParam<unsigned int>("n_right", 0, "Number of cells to the right of the central cell");
  params.addParam<unsigned int>("n_top", 0, "Number of cells above the central cell");
  params.addParam<unsigned int>("n_bottom", 0, "Number of cells below the central cell");
  params.addParam<unsigned int>(
      "num_sectors", 8, "Number of azimuthal sectors per quadrant; must be positive and even");
  params.addParam<unsigned int>(
      "inclusion_rings", 6, "Number of radial element rings inside the inclusion");
  params.addParam<unsigned int>(
      "matrix_rings", 8, "Number of radial element rings between the inclusion and cell boundary");
  params.addClassDescription(
      "Creates a rectangular array of unit-square cells with conforming centered circular "
      "inclusions and a central cell fixed at the coordinate origin.");
  return params;
}

HomoxMultiInclusionMeshGenerator::HomoxMultiInclusionMeshGenerator(
    const InputParameters & parameters)
  : MeshGenerator(parameters),
    _radius(getParam<Real>("radius")),
    _n_left(getParam<unsigned int>("n_left")),
    _n_right(getParam<unsigned int>("n_right")),
    _n_top(getParam<unsigned int>("n_top")),
    _n_bottom(getParam<unsigned int>("n_bottom")),
    _num_sectors(getParam<unsigned int>("num_sectors")),
    _inclusion_rings(getParam<unsigned int>("inclusion_rings")),
    _matrix_rings(getParam<unsigned int>("matrix_rings"))
{
  if (_radius <= 0.0 || _radius >= 0.5)
    paramError("radius", "The radius must be greater than zero and smaller than 0.5.");
  if (_num_sectors == 0 || _num_sectors % 2 != 0)
    paramError("num_sectors", "The number of sectors must be positive and even.");
  if (_inclusion_rings == 0)
    paramError("inclusion_rings", "At least one inclusion ring is required.");
  if (_matrix_rings == 0)
    paramError("matrix_rings", "At least one matrix ring is required.");

  const MeshGeneratorName tile_name = name() + "_tile";
  const MeshGeneratorName pattern_name = name() + "_pattern";
  const MeshGeneratorName translated_name = name() + "_translated";
  const MeshGeneratorName blocks_name = name() + "_blocks";
  const MeshGeneratorName bottom_left_name = name() + "_bottom_left";
  const MeshGeneratorName bottom_right_name = name() + "_bottom_right";

  {
    auto params = _app.getFactory().getValidParams("ConcentricCircleMeshGenerator");
    params.set<unsigned int>("num_sectors") = _num_sectors;
    params.set<std::vector<Real>>("radii") = {_radius};
    params.set<std::vector<unsigned int>>("rings") = {_inclusion_rings, _matrix_rings};
    params.set<bool>("has_outer_square") = true;
    params.set<Real>("pitch") = 1.0;
    params.set<bool>("preserve_volumes") = true;
    params.set<unsigned int>("smoothing_max_it") = 1;
    addMeshSubgenerator("ConcentricCircleMeshGenerator", tile_name, params);
  }

  {
    auto params = _app.getFactory().getValidParams("PatternedMeshGenerator");
    params.set<std::vector<MeshGeneratorName>>("inputs") = {tile_name};
    params.set<std::vector<std::vector<unsigned int>>>("pattern") =
        std::vector<std::vector<unsigned int>>(
            _n_top + _n_bottom + 1, std::vector<unsigned int>(_n_left + _n_right + 1, 0));
    params.set<Real>("x_width") = 1.0;
    params.set<Real>("y_width") = 1.0;
    addMeshSubgenerator("PatternedMeshGenerator", pattern_name, params);
  }

  {
    auto params = _app.getFactory().getValidParams("TransformGenerator");
    params.set<MeshGeneratorName>("input") = pattern_name;
    params.set<MooseEnum>("transform") = "TRANSLATE";
    params.set<RealVectorValue>("vector_value") =
        RealVectorValue(-static_cast<Real>(_n_left), static_cast<Real>(_n_top), 0.0);
    addMeshSubgenerator("TransformGenerator", translated_name, params);
  }

  {
    auto params = _app.getFactory().getValidParams("RenameBlockGenerator");
    params.set<MeshGeneratorName>("input") = translated_name;
    params.set<std::vector<SubdomainName>>("old_block") = {"1", "2"};
    params.set<std::vector<SubdomainName>>("new_block") = {"inclusion", "matrix"};
    addMeshSubgenerator("RenameBlockGenerator", blocks_name, params);
  }

  const Real x_min = -static_cast<Real>(_n_left) - 0.5;
  const Real x_max = static_cast<Real>(_n_right) + 0.5;
  const Real y_min = -static_cast<Real>(_n_bottom) - 0.5;
  {
    auto params = _app.getFactory().getValidParams("ExtraNodesetGenerator");
    params.set<MeshGeneratorName>("input") = blocks_name;
    params.set<std::vector<BoundaryName>>("new_boundary") = {"bottom_left"};
    params.set<std::vector<std::vector<Real>>>("coord") = {{x_min, y_min, 0.0}};
    addMeshSubgenerator("ExtraNodesetGenerator", bottom_left_name, params);
  }

  {
    auto params = _app.getFactory().getValidParams("ExtraNodesetGenerator");
    params.set<MeshGeneratorName>("input") = bottom_left_name;
    params.set<std::vector<BoundaryName>>("new_boundary") = {"bottom_right"};
    params.set<std::vector<std::vector<Real>>>("coord") = {{x_max, y_min, 0.0}};
    addMeshSubgenerator("ExtraNodesetGenerator", bottom_right_name, params);
    _build_mesh = &getMeshByName(bottom_right_name);
  }
}

std::unique_ptr<MeshBase>
HomoxMultiInclusionMeshGenerator::generate()
{
  return std::move(*_build_mesh);
}

