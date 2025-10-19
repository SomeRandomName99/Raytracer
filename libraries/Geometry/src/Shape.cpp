#include <algorithm>
#include <cmath>
#include <tuple>

#include "Shape.hpp"
#include "World.hpp"

namespace raytracer::geometry{
using namespace utility;

constexpr float EPS = 1e-6f;

// Helper function that intersects the caps of a closed cylinder or cone
static inline void addCircularCapIntersections(const Tuple& orig, const Tuple& dir, const float min, const float max,
                                               const bool closed, const float upperRadiusSq, const float lowerRadiusSq,
                                               Arena<Intersection>& intersections, const WorldObject& object) noexcept{
  if (!closed || floatNearlyEqual(dir.y, 0.f)) { return; }

  auto tryCap = [&](float capY, float radiusSquared){
    float t  = (capY - orig.y) / dir.y;
    float x  = orig.x + t * dir.x;
    float z  = orig.z + t * dir.z;
    float r2 = x*x + z*z;
    if(r2 < radiusSquared * (1.0f + EPS)){ // within the radius
      intersections.pushBack(Intersection{&object, t}); 
    }  
  };

  tryCap(min, lowerRadiusSq);
  tryCap(max, upperRadiusSq);
}
// Helper function that computes the y coordinates of the intersection points of a ray with an infinite cylinder or cone
// We assume that the cylinder or cone is centered at the origin and aligned with the y axis
static inline void addCircularSideIntersections(float a, float b, float c, const Tuple& orig, const Tuple& dir, float min, float max, 
                                                const WorldObject& object, Arena<Intersection>& intersections) noexcept{
  auto disc = b*b - 4*a*c;
  if (!floatNearlyEqual(disc, 0.f) && disc < 0) { return; }
  auto t0 = (-b - utility::sqrt(disc)) / (2 * a);
  auto t1 = (-b + utility::sqrt(disc)) / (2 * a);

  auto y0 = orig.y + t0*dir.y;
  auto y1 = orig.y + t1*dir.y;
  if(y0 > min && y0 < max){intersections.pushBack(Intersection{&object, t0});}
  if(y1 > min && y1 < max){intersections.pushBack(Intersection{&object, t1});}
}

void localIntersect(const Ray& objectSpaceRay, const WorldObject& object, 
                    Arena<Intersection>& intersections, std::vector<circularSolidData>& circularObjectData) noexcept{
  auto& dir = objectSpaceRay.direction;
  auto& orig = objectSpaceRay.origin;
  switch(object.shapeTag.type) {
    case ShapeType::Sphere: {
      // For now we assume that the sphere is always at the origin
      const auto centerToRay = orig - Point(0, 0, 0);
      const auto a = dir.dot(dir);
      const auto b = 2 * dir.dot(centerToRay);
      const auto c = centerToRay.dot(centerToRay) - 1; // where 1 is the radius_ of the sphere

      const float disc = b*b - 4*a*c;
      if (disc < 0){return;}
      const auto dist1 = (-b - utility::sqrt(disc)) / (2*a);
      const auto dist2 = (-b + utility::sqrt(disc)) / (2*a);
      intersections.pushBack(Intersection{&object, dist1});
      intersections.pushBack(Intersection{&object, dist2});
      break;
    }

    case ShapeType::Plane: {
      if (std::fabsf(dir.y) < EPS) {
        return;
      }
      const float dist = -orig.y / dir.y;
      intersections.pushBack(Intersection{&object, dist});
      break;
    }

    case ShapeType::Cube: {
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
      intersections.pushBack(Intersection{&object, tmin});
      intersections.pushBack(Intersection{&object, tmax});
      break;
    }

    case ShapeType::Cylinder: {
      float min = circularObjectData[object.shapeTag.dataIndex].minimum;
      float max = circularObjectData[object.shapeTag.dataIndex].maximum;
      bool closed = circularObjectData[object.shapeTag.dataIndex].closed;
      float dx = dir.x, dz = dir.z;
      float ox = orig.x, oz = orig.z;
      float a = dx*dx + dz*dz;
      float b = 2*ox*dx + 2*oz*dz;
      float c = ox*ox + oz*oz - 1;

      float upperAndLowerCapRadiusSq = 1.f;
      addCircularCapIntersections(orig, dir, min, max, closed, upperAndLowerCapRadiusSq, upperAndLowerCapRadiusSq, intersections, object);
      if(floatNearlyEqual(a, 0.f)){ // ray is parallel to the y axis
        return;
      }

      addCircularSideIntersections(a, b, c, orig, dir, min, max, object, intersections);
      break;
    }

    case ShapeType::Cone: {
      float min = circularObjectData[object.shapeTag.dataIndex].minimum;
      float max = circularObjectData[object.shapeTag.dataIndex].maximum;
      bool closed = circularObjectData[object.shapeTag.dataIndex].closed;
      float dx = dir.x, dy = dir.y, dz = dir.z;
      float ox = orig.x, oy = orig.y, oz = orig.z;
      float a = dx*dx - dy*dy + dz*dz;
      float b = 2*ox*dx - 2*oy*dy + 2*oz*dz;
      float c = ox*ox - oy*oy + oz*oz;

      float upperCapRadiusSq = max*max;
      float lowerCapRadiusSq = min*min;
      addCircularCapIntersections(orig, dir, min, max, closed, upperCapRadiusSq, lowerCapRadiusSq, intersections, object);
      if(floatNearlyEqual(a, 0.f)){ // ray is parallel to one of the cone's halves
        if(floatNearlyEqual(b, 0.f)){ return; }
        intersections.pushBack(Intersection{&object, -c / (2 * b)});
        return;
      }

      addCircularSideIntersections(a, b, c, orig, dir, min, max, object, intersections);
      break;
    }

    case ShapeType::Group: {
      // TODO: have to go through all the shapes contained in the group
      // and intersect with all of them
      assert(false && "Group intersections have not been implemented yet");
      break;
    }

    default: {
      assert(false && "Unknown shape type in localIntersect");
      break;
    }
  }
}

Tuple normalAt(const WorldObject& object, const Tuple& point, std::vector<circularSolidData>& circularObjectData) noexcept{
  auto objectSpacePoint = object.inverseTransform * point;
  Tuple normal;
  switch(object.shapeTag.type) {
    case ShapeType::Sphere: {
      normal = objectSpacePoint - Point(0, 0, 0);
      break;
    }

    case ShapeType::Plane: {
      normal = Vector(0, 1, 0);
      break;
    }

    case ShapeType::Cube: {
      float ax = std::fabsf(objectSpacePoint.x);
      float ay = std::fabsf(objectSpacePoint.y);
      float az = std::fabsf(objectSpacePoint.z);
      if(ax >= ay && ax >= az){
        normal = Vector(objectSpacePoint.x, 0, 0);
      } else if (ay >= ax && ay >= az){
        normal = Vector(0, objectSpacePoint.y, 0);
      } else {
        normal = Vector(0, 0, objectSpacePoint.z);
      }
      break;
    }

    case ShapeType::Cylinder: {
      float min = circularObjectData[object.shapeTag.dataIndex].minimum;
      float max = circularObjectData[object.shapeTag.dataIndex].maximum;
      float dist = objectSpacePoint.x * objectSpacePoint.x + objectSpacePoint.z * objectSpacePoint.z;
      if(dist < 1 && objectSpacePoint.y >= max - EPS) {
        normal = Vector(0, 1, 0);
      } else if (dist < 1 && objectSpacePoint.y <= min + EPS) {
        normal = Vector(0, -1, 0);
      } else {
        normal = Vector(objectSpacePoint.x, 0, objectSpacePoint.z);
      }
      break;
    }

    case ShapeType::Cone: {
      float min = circularObjectData[object.shapeTag.dataIndex].minimum;
      float max = circularObjectData[object.shapeTag.dataIndex].maximum;
      float dist = objectSpacePoint.x * objectSpacePoint.x + objectSpacePoint.z * objectSpacePoint.z;
      if (objectSpacePoint.y <= min + EPS && dist <= min * min) {
        normal = Vector(0, -1, 0);  // Lower cap
      } else if (objectSpacePoint.y >= max - EPS && dist <= max * max) {
        normal = Vector(0, 1, 0);  // Upper cap
      } else {
        normal = Vector(objectSpacePoint.x, -objectSpacePoint.y, objectSpacePoint.z);
      }
      break;
    }

    case ShapeType::Group: {
      assert(false && "Group normal calculation has not been implemented yet");
      break;
    }
    
    default: {
      assert(false && "Unknown shape type in normalAt");
      break;
    }
  }
  normal.normalize();
  return object.transform * normal;
}

} // namespace raytracer::geometry