#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "libraries/Material/include/Material.hpp"
#include "libraries/Utility/include/AABB.hpp"
#include "libraries/Utility/include/Matrix.hpp"
#include <cstdint>

namespace raytracer::geometry {
using namespace utility;
using namespace material;

enum class ShapeType {
  Group,
  Sphere,
  Plane,
  Cube,
  Cylinder,
  Cone,
  Triangle,
  Mesh,
};

struct ShapeTypeTag {
  ShapeType type;
  int32_t dataIndex = -1;
};

struct WorldObject {
  ShapeTypeTag shapeTag;
  AABB boundingBox;
  Matrix<4, 4> transform = Matrix<4, 4>::identity();
  Matrix<4, 4> inverseTransform = Matrix<4, 4>::identity();
  int16_t parentIndex = -1;
  int16_t MaterialIndex = -1;
  bool hasShadow = true;
};

struct GroupData {
  std::vector<uint32_t> childerenIndices;
};

struct CircularSolidData {
  float minimum = 1.0f;
  float maximum = -1.0f;
  bool closed = true;
};

struct TriangleData {
  Tuple v0, v1, v2;         // vertex positions
  Tuple n0, n1, n2;         // per-vertex normals (for smooth shading)
  Tuple e0, e1;             // precomputed edges: v1 - v0 and v2 - v0
  float u = 0.0f, v = 0.0f; // texture coordinates from the object file
};

// A mesh is a contiguous range of triangles in the world's triangle data vector
struct MeshData {
  int32_t firstTriangleIndex = 0;
  int32_t triangleCount = 0;
};

void localIntersect(
    const Ray &objectSpaceRay, const WorldObject &object,
    Arena<Intersection> &intersections,
    const std::vector<CircularSolidData> &circularObjectData,
    const std::vector<TriangleData> &triObjectData,
    const std::vector<MeshData> &meshObjectData) noexcept;
Tuple normalAt(
    const WorldObject &object, const Tuple &point,
    const std::vector<CircularSolidData> &circularObjectData,
    const std::vector<TriangleData> &triObjectData, float u = 0.0f,
    float v = 0.0f, int32_t triangleIndex = -1) noexcept;
} // namespace raytracer::geometry

#endif // SHAPE_HPP
