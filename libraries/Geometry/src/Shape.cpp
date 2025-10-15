#include "Shape.hpp"
#include "World.hpp"

namespace raytracer::geometry{

void setBoundingBox(const World& worldObjects, SceneNode& node) noexcept{
  switch(node.shapeTag.type) {
    case ShapeType::Sphere:
      node.boundingBox = {Point(-1, -1, -1), Point(1, 1, 1)};
      break;
    case ShapeType::Plane:
      node.boundingBox = {Point(-INFINITY, 0, -INFINITY), Point(INFINITY, 0, INFINITY)};
      break;
    case ShapeType::Cylinder:
      int32_t dataIndex = node.shapeTag.dataIndex;
      float min = worldObjects.circularSolidData[dataIndex].minimum;
      float max = worldObjects.circularSolidData[dataIndex].maximum;
      node.boundingBox = {Point(-1, min, -1), Point(1, max, 1)};
      break;
    case ShapeType::Cube:
      node.boundingBox = {Point(-1, -1, -1), Point(1, 1, 1)};
      break;
    case ShapeType::Cone:
      int32_t dataIndex = node.shapeTag.dataIndex;
      const float limit = std::max(std::abs(worldObjects.circularSolidData[dataIndex].minimum), std::abs(worldObjects.circularSolidData[dataIndex].maximum));
      node.boundingBox = {Point(-limit, worldObjects.circularSolidData[dataIndex].minimum, -limit),
                          Point(limit, worldObjects.circularSolidData[dataIndex].maximum, limit)};
      break;
    case ShapeType::Group:
      int32_t dataIndex = node.shapeTag.dataIndex;
      for(auto& childIndex: worldObjects.groupData[dataIndex].childerenIndices) {
        node.boundingBox.expandToInclude(worldObjects.sceneNodes[childIndex].boundingBox.transform(worldObjects.sceneNodes[childIndex].transform));
      }
      break;
  }
}

// Helper function that intersects the caps of a closed cylinder or cone
void intersectCircularCaps(const Tuple& orig, const Tuple& dir, const float min, const float max, 
                           const bool closed, Arena<Intersection>& intersections) noexcept{
  if (!closed || utility::floatNearlyEqual(dir.y, 0.f)) { return; }

  auto tLoCap = (min - orig.y) / dir.y;
  auto x = orig.x + tLoCap * dir.x;
  auto z = orig.z + tLoCap * dir.z;
  if((x*x + z*z) <= 1){ intersections.pushBack(Intersection{&object, tLoCap}); }

  auto tUpCap = (max - orig.y) / dir.y;
  x = orig.x + tUpCap * dir.x;
  z = orig.z + tUpCap * dir.z;
  if((x*x + z*z) <= 1){ intersections.pushBack(Intersection{&object, tUpCap}); }
}
// Helper function that computes the y coordinates of the intersection points of a ray with an infinite cylinder or cone
// We assume that the cylinder or cone is centered at the origin and aligned with the y axis
std::pair<float, float> computeCircularIntersectionYCoords(float a, float b, float c,
                                                           const Tuple& orig, const Tuple& dir) noexcept{
  auto disc = b*b - 4*a*c;
  if (!floatNearlyEqual(disc, 0.f) && disc < 0) { return; }
  auto t0 = (-b - utility::sqrt(disc)) / (2 * a);
  auto t1 = (-b + utility::sqrt(disc)) / (2 * a);

  auto y0 = orig.y + t0*dir.y;
  auto y1 = orig.y + t1*dir.y;
  return {y0, y1};
}

void localIntersect(const Ray& objectSpaceRay, const WorldObject& object, 
                    Arena<Intersection>& intersections, std::vector<circularSolidData>& circularObjectData) noexcept{
  auto& dir = objectSpaceRay.direction;
  auto& orig = objectSpaceRay.origin;
  switch(object.shapeTag.type) {
    case ShapeType::Sphere: 
      // For now we assume that the sphere is always at the origin
      const auto cetnerToRay = orig - Point(0, 0, 0);
      const auto a = dir.dot(dir);
      const auto b = 2 * dir.dot(cetnerToRay);
      const auto c = cetnerToRay.dot(cetnerToRay) - 1; // where 1 is the radius_ of the sphere

      const float disc = b*b - 4*a*c;
      if (disc < 0){return;}
      const auto dist1 = (-b - sqrt(disc)) / (2*a);
      const auto dist2 = (-b + sqrt(disc)) / (2*a);
      intersections.pushBack(Intersection{&object, dist1});
      intersections.pushBack(Intersection{&object, dist2});
      break;

    case ShapeType::Plane: 
      if (std::abs(dir.y) < 1e-6) {
        return;
      }
      const float dist = -orig.y / dir.y;
      intersections.pushBack(Intersection{&object, dist});
      break;

    case ShapeType::Cube: 
      float tx1 = (-1 - orig.x) / dir.x;
      float tx2 = (1 - orig.x) / dir.x;

      float tmin = std::min(tx1, tx2);
      float tmax = std::max(tx1, tx2);

      float ty1 = (-1 - orig.y) / dir.y;
      float ty2 = (1 - orig.y) / dir.y;

      tmin = std::max(tmin, std::min(ty1, ty2));
      tmax = std::min(tmax, std::max(ty1, ty2));

      float tz1 = (-1 - orig.z) / dir.z;
      float tz2 = (1 - orig.z) / dir.z;

      tmin = std::max(tmin, std::min(tz1, tz2));
      tmax = std::min(tmax, std::max(tz1, tz2));

      if (tmin > tmax) return;
      intersections.pushBack(Intersection{this, tmin});
      intersections.pushBack(Intersection{this, tmax});

    case ShapeType::Cylinder: 
      float min = circularObjectData[object.shapeTag.dataIndex].minimum;
      float max = circularObjectData[object.shapeTag.dataIndex].maximum;
      bool closed = circularObjectData[object.shapeTag.dataIndex].closed;
      auto a = dir.x * dir.x + dir.z*dir.z;
      auto b = 2*orig.x*dir.x + 2*orig.z*dir.z;
      auto c = orig.x*orig.x + orig.z*orig.z - 1;
      if(floatNearlyEqual(a, 0.f)){ // ray is parallel to the y axis
        intersectCircularCaps(orig, dir, min, max, closed, intersections);
        return;
      }

      auto [y0, y1] = computeCircularIntersectionYCoords(a, b, c, orig, dir);
      if(y0 > min && y0 < max){intersections.pushBack(Intersection{&object, t0});}
      if(y1 > min && y1 < max){intersections.pushBack(Intersection{&object, t1});}
      intersectCircularCaps(orig, dir, min, max, closed, intersections);
      break;

    case ShapeType::Cone:
      float min = circularObjectData[object.shapeTag.dataIndex].minimum;
      float max = circularObjectData[object.shapeTag.dataIndex].maximum;
      bool closed = circularObjectData[object.shapeTag.dataIndex].closed;
      auto a = dir.x*dir.x - dir.y*dir.y + dir.z*dir.z;
      auto b = 2*orig.x*dir.x - 2*orig.y*dir.y + 2*orig.z*dir.z;
      auto c = orig.x*orig.x - orig.y*orig.y + orig.z*orig.z;

      if(floatNearlyEqual(a, 0.f)){ // ray is parallel to one of the cone's halves
        intersectCircularCaps(orig, dir, min, max, closed, intersections);
        if(floatNearlyEqual(b, 0.f)){ return; }
        intersections.pushBack(Intersection{&object, -c / (2 * b)});
        return;
      }

      auto [y0, y1] = computeCircularIntersectionYCoords(a, b, c, orig, dir);
      if(y0 > min && y0 < max){intersections.pushBack(Intersection{&object, t0});}
      if(y1 > min && y1 < max){intersections.pushBack(Intersection{&object, t1});}
      intersectCircularCaps(orig, dir, min, max, closed, intersections);
      break;

    case ShapeType::Group:
      // TODO: have to go through all the shapes contained in the group
      // and intersect with all of them
      assert(false && "Group intersections have not been implemented yet");
      break;
  }
}

} // namespace raytracer::geometry