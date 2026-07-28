#include "libraries/Scene/include/World.hpp"
#include "libraries/Geometry/include/Shape.hpp"

#include <cstddef>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "3rdParty/tiny_obj_loader.h"

namespace raytracer::scene {

void setBoundingBox(const World &world, WorldObject &node) noexcept {
  switch (node.shapeTag.type) {
    case ShapeType::Sphere: {
      node.boundingBox = {Point(-1, -1, -1), Point(1, 1, 1)};
      break;
    }
    case ShapeType::Plane: {
      node.boundingBox = {Point(-INFINITY, 0, -INFINITY), Point(INFINITY, 0, INFINITY)};
      break;
    }
    case ShapeType::Cylinder: {
      int32_t dataIndex = node.shapeTag.dataIndex;
      float min = world.circularSolidData[dataIndex].minimum;
      float max = world.circularSolidData[dataIndex].maximum;
      node.boundingBox = {Point(-1, min, -1), Point(1, max, 1)};
      break;
    }
    case ShapeType::Cube: {
      node.boundingBox = {Point(-1, -1, -1), Point(1, 1, 1)};
      break;
    }
    case ShapeType::Cone: {
      int32_t dataIndex = node.shapeTag.dataIndex;
      const float limit = std::max(std::abs(world.circularSolidData[dataIndex].minimum),
                                   std::abs(world.circularSolidData[dataIndex].maximum));
      node.boundingBox = {Point(-limit, world.circularSolidData[dataIndex].minimum, -limit),
                          Point(limit, world.circularSolidData[dataIndex].maximum, limit)};
      break;
    }
    case ShapeType::Triangle: {
      const int32_t dataIndex = node.shapeTag.dataIndex;
      const TriangleData &tri = world.triangleData[dataIndex];
      node.boundingBox = AABB(tri.v0, tri.v1);
      node.boundingBox.expandToInclude(tri.v2);
      break;
    }
    case ShapeType::Mesh: {
      const MeshData &mesh = world.meshData[node.shapeTag.dataIndex];
      for (int32_t i = 0; i < mesh.triangleCount; ++i) {
        const TriangleData &tri = world.triangleData[mesh.firstTriangleIndex + i];
        if (i == 0) {
          node.boundingBox = AABB(tri.v0, tri.v1);
        } else {
          node.boundingBox.expandToInclude(tri.v0);
          node.boundingBox.expandToInclude(tri.v1);
        }
        node.boundingBox.expandToInclude(tri.v2);
      }
      break;
    }
    case ShapeType::Group: {
      int32_t dataIndex = node.shapeTag.dataIndex;
      for (auto &childIndex : world.groupData[dataIndex].childerenIndices) {
        node.boundingBox.expandToInclude(
            world.objects[childIndex].boundingBox.transform(world.objects[childIndex].transform));
      }
      break;
    }
  }
}

size_t addMaterial(World &world, const Material &material) noexcept {
  world.materials.push_back(material);
  return world.materials.size() - 1;
}

size_t addPattern(World &world, const Pattern &pattern) noexcept {
  world.patterns.push_back(pattern);
  return world.patterns.size() - 1;
}

size_t addObject(World &world, const WorldObject &object) noexcept {
  // Make a local copy so we can ensure invariants (inverse transform, bounding box)
  WorldObject newObject = object;

  // Ensure inverseTransform matches transform
  newObject.inverseTransform = inverse(newObject.transform);

  // Initialize bounding box for the object based on its shape and world data
  setBoundingBox(world, newObject);

  world.objects.push_back(std::move(newObject));
  return world.objects.size() - 1;
}

size_t addTriangle(World &world, const utility::Tuple &v0, const utility::Tuple &v1, const utility::Tuple &v2,
                   const utility::Tuple &n0, const utility::Tuple &n1, const utility::Tuple &n2, float u,
                   float v) noexcept {
  TriangleData data;
  data.v0 = v0;
  data.v1 = v1;
  data.v2 = v2;
  data.n0 = n0;
  data.n1 = n1;
  data.n2 = n2;
  world.triangleData.push_back(data);
  const int32_t dataIndex = static_cast<int32_t>(world.triangleData.size() - 1);

  WorldObject object;
  object.shapeTag = ShapeTypeTag{ShapeType::Triangle, dataIndex};
  return addObject(world, object);
}

size_t addTriangle(World &world, const utility::Tuple &v0, const utility::Tuple &v1, const utility::Tuple &v2, float u,
                   float v) noexcept {
  // Derive the geometric (face) normal from the two edges and share it across all vertices.
  const utility::Tuple faceNormal = (v1 - v0).cross(v2 - v0).normalize();
  return addTriangle(world, v0, v1, v2, faceNormal, faceNormal, faceNormal, u, v);
}

size_t addLight(World &world, const PointLight &light) noexcept {
  world.lights.push_back(light);
  return world.lights.size() - 1;
}

size_t addObjectWithMaterial(World &world, const WorldObject &object, const Material &material,
                             const std::optional<Pattern> &pattern) noexcept {
  size_t materialIndex = addMaterial(world, material);
  WorldObject newObject = object;
  newObject.MaterialIndex = materialIndex;
  if (pattern.has_value()) {
    size_t patternIndex = addPattern(world, pattern.value());
    world.materials[materialIndex].patternIndex = patternIndex;
  }
  return addObject(world, newObject);
}

void addTransformToObject(World &world, const size_t objectIndex, const utility::Matrix<4, 4> &transform) noexcept {
  WorldObject &object = world.objects[objectIndex];
  object.transform = transform * object.transform;
  object.inverseTransform = object.inverseTransform * inverse(transform);
  setBoundingBox(world, object);
}

void setObjectShadow(World &world, const size_t objectIndex, const bool hasShadow) noexcept {
  WorldObject &object = world.objects[objectIndex];
  object.hasShadow = hasShadow;
}

std::optional<size_t> loadMeshFromObjFile(World &world, const std::string &inputFile) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> objMaterials;
  std::string err;

  // LoadObj triangulates polygonal faces by default, so every face comes back as three indices
  if (!tinyobj::LoadObj(&attrib, &shapes, &objMaterials, &err, inputFile.c_str())) {
    std::cerr << "Failed to load OBJ file '" << inputFile << "': " << err << '\n';
    return std::nullopt;
  }
  if (!err.empty()) {
    std::cerr << "Warnings while loading OBJ file '" << inputFile << "': " << err;
  }

  const auto readVertex = [&attrib](const int index) {
    return Point(attrib.vertices[3 * index], attrib.vertices[3 * index + 1], attrib.vertices[3 * index + 2]);
  };
  const auto readNormal = [&attrib](const int index) {
    return utility::Vector(attrib.normals[3 * index], attrib.normals[3 * index + 1], attrib.normals[3 * index + 2]);
  };

  const int32_t firstTriangleIndex = static_cast<int32_t>(world.triangleData.size());
  for (const auto &shape : shapes) {
    for (size_t i = 0; i + 2 < shape.mesh.indices.size(); i += 3) {
      const tinyobj::index_t &i0 = shape.mesh.indices[i];
      const tinyobj::index_t &i1 = shape.mesh.indices[i + 1];
      const tinyobj::index_t &i2 = shape.mesh.indices[i + 2];

      TriangleData tri;
      tri.v0 = readVertex(i0.vertex_index);
      tri.v1 = readVertex(i1.vertex_index);
      tri.v2 = readVertex(i2.vertex_index);
      Tuple e0 = tri.v1 - tri.v0;
      Tuple e1 = tri.v2 - tri.v0;
      if (i0.normal_index >= 0 && i1.normal_index >= 0 && i2.normal_index >= 0) {
        tri.n0 = readNormal(i0.normal_index);
        tri.n1 = readNormal(i1.normal_index);
        tri.n2 = readNormal(i2.normal_index);
      } else {
        // No vertex normals in the file, fall back to the geometric face normal
        const utility::Tuple faceNormal = e0.cross(e1).normalize();
        tri.n0 = faceNormal;
        tri.n1 = faceNormal;
        tri.n2 = faceNormal;
      }
      world.triangleData.push_back(tri);
    }
  }

  MeshData mesh;
  mesh.firstTriangleIndex = firstTriangleIndex;
  mesh.triangleCount = static_cast<int32_t>(world.triangleData.size()) - firstTriangleIndex;
  world.meshData.push_back(mesh);
  const int32_t meshIndex = static_cast<int32_t>(world.meshData.size() - 1);

  WorldObject object;
  object.shapeTag = ShapeTypeTag{ShapeType::Mesh, meshIndex};
  return addObject(world, object);
}

} // namespace raytracer::scene
