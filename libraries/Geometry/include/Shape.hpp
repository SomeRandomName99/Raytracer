#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <cstdint>
#include "Matrix.hpp"
#include "AABB.hpp"
#include "Material.hpp"

namespace raytracer::geometry {
using namespace utility;
using namespace material;

enum class ShapeType{
  Group,
  Sphere,
  Plane,
  Cube,
  Cylinder,
  Cone,
};

struct ShapeTypeTag{
  ShapeType type;
  int32_t dataIndex = -1;
};

struct WorldObject {
  ShapeTypeTag shapeTag;
  AABB boundingBox;
  Matrix<4,4> transform = Matrix<4,4>::identity();
  Matrix<4,4> inverseTransform = Matrix<4,4>::identity();
  int16_t parentIndex = -1;
  int16_t MaterialIndex = -1;
  bool hasShadow = true;
};

struct GroupData{
  std::vector<uint32_t> childerenIndices;
};

struct CircularSolidData {
  float minimum = 1.0f;
  float maximum = -1.0f;
  bool closed = true;
};

void localIntersect(const Ray& objectSpaceRay, const WorldObject& object, Arena<Intersection>& intersections, const std::vector<CircularSolidData>& circularObjectData) noexcept; 
Tuple normalAt(const WorldObject& object, const Tuple& point, const std::vector<CircularSolidData>& circularObjectData) noexcept;
} // namespace raytracer::geometry

#endif // SHAPE_HPP