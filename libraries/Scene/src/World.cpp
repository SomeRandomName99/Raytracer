#include "World.hpp"
#include "Shape.hpp"

namespace raytracer::scene {

void setBoundingBox(const World& world, WorldObject& node) noexcept{
  switch(node.shapeTag.type) {
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
      const float limit = std::max(std::abs(world.circularSolidData[dataIndex].minimum), std::abs(world.circularSolidData[dataIndex].maximum));
      node.boundingBox = {Point(-limit, world.circularSolidData[dataIndex].minimum, -limit),
                          Point(limit, world.circularSolidData[dataIndex].maximum, limit)};
      break;
    }
    case ShapeType::Group: {
      int32_t dataIndex = node.shapeTag.dataIndex;
      for(auto& childIndex: world.groupData[dataIndex].childerenIndices) {
        node.boundingBox.expandToInclude(world.objects[childIndex].boundingBox.transform(world.objects[childIndex].transform));
      }
      break;
    }
  }
}

size_t addMaterial(World& world, const Material& material) noexcept{
  world.materials.push_back(material);
  return world.materials.size() - 1;
}

size_t addPattern(World& world, const Pattern& pattern) noexcept{
  world.patterns.push_back(pattern);
  return world.patterns.size() - 1;
}

size_t addObject(World& world, const WorldObject& object) noexcept{
  // Make a local copy so we can ensure invariants (inverse transform, bounding box)
  WorldObject newObject = object;

  // Ensure inverseTransform matches transform
  newObject.inverseTransform = inverse(newObject.transform);

  // Initialize bounding box for the object based on its shape and world data
  setBoundingBox(world, newObject);

  world.objects.push_back(std::move(newObject));
  return world.objects.size() - 1;
}

size_t addLight(World& world, const PointLight& light) noexcept{
  world.lights.push_back(light);
  return world.lights.size() - 1;
}

size_t addObjectWithMaterial(World& world, const WorldObject& object, 
                             const Material& material, 
                             const std::optional<Pattern>& pattern) noexcept{
  size_t materialIndex = addMaterial(world, material);
  WorldObject newObject = object;
  newObject.MaterialIndex = materialIndex;
  if(pattern.has_value()){
    size_t patternIndex = addPattern(world, pattern.value());
    world.materials[materialIndex].patternIndex = patternIndex;
  }
  return addObject(world, newObject);
}

void addTransformToObject(World& world, const size_t objectIndex, const utility::Matrix<4,4>& transform) noexcept{
  WorldObject& object = world.objects[objectIndex];
  object.transform = transform * object.transform;
  object.inverseTransform = object.inverseTransform * inverse(transform);
  setBoundingBox(world, object);
}

void setObjectShadow(World& world, const size_t objectIndex, const bool hasShadow) noexcept{
  WorldObject& object = world.objects[objectIndex];
  object.hasShadow = hasShadow;
}

} // namespace raytracer::scene
