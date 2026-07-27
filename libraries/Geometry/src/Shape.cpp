#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>

#include "libraries/Geometry/include/Shape.hpp"
#include "libraries/Scene/include/World.hpp"
#include "libraries/Utility/include/FloatUtils.hpp"

namespace raytracer::geometry {
using namespace utility;

constexpr float EPS = 1e-6f;

// Helper function that intersects the caps of a closed cylinder or cone
static inline void addCircularCapIntersections(const Tuple &orig, const Tuple &dir, const float min, const float max,
                                               const bool closed, const float upperRadiusSq, const float lowerRadiusSq,
                                               Arena<Intersection> &intersections, const WorldObject &object) noexcept {
  if (!closed || floatNearlyEqual(dir.y, 0.f)) {
    return;
  }

  auto tryCap = [&](float capY, float radiusSquared) {
    float t = (capY - orig.y) / dir.y;
    float x = orig.x + t * dir.x;
    float z = orig.z + t * dir.z;
    float r2 = x * x + z * z;
    if (r2 < radiusSquared * (1.0f + EPS)) { // within the radius
      intersections.pushBack(Intersection{&object, t});
    }
  };

  tryCap(min, lowerRadiusSq);
  tryCap(max, upperRadiusSq);
}
// Helper function that computes the y coordinates of the intersection points of a ray with an infinite cylinder or cone
// We assume that the cylinder or cone is centered at the origin and aligned with the y axis
static inline void addCircularSideIntersections(float a, float b, float c, const Tuple &orig, const Tuple &dir,
                                                float min, float max, const WorldObject &object,
                                                Arena<Intersection> &intersections) noexcept {
  auto disc = b * b - 4 * a * c;
  if (!floatNearlyEqual(disc, 0.f) && disc < 0) {
    return;
  }
  auto t0 = (-b - utility::sqrt(disc)) / (2 * a);
  auto t1 = (-b + utility::sqrt(disc)) / (2 * a);

  auto y0 = orig.y + t0 * dir.y;
  auto y1 = orig.y + t1 * dir.y;
  if (y0 > min && y0 < max) {
    intersections.pushBack(Intersection{&object, t0});
  }
  if (y1 > min && y1 < max) {
    intersections.pushBack(Intersection{&object, t1});
  }
}

// This part implements the Möller Trumbore algorithm. Scratch Pixel was used as a reference
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle//moller-trumbore-ray-triangle-intersection.html
// The main gist is that cramer's rule is used to solve a system of equations where the coordinates are in the
// barycentric system
static inline void addTriangleIntersection(const TriangleData &tri, const Tuple &orig, const Tuple &dir,
                                           const WorldObject &object, const int32_t triangleIndex,
                                           Arena<Intersection> &intersections) noexcept {
  const Tuple perpVec = dir.cross(tri.e1); // perpendicular to dir and edge2
  const float det = tri.e0.dot(perpVec);
  if (fabs(det) < EPSILON<float> * EPSILON<float>)
    return;

  // Replace the middle column vector by O - A
  const float invDet = 1.0f / det;
  const Tuple v0ToOrig = orig - tri.v0;
  const float u = invDet * v0ToOrig.dot(perpVec);
  if (u < 0.0f || u > 1.0f)
    return;

  // replace the last column vector by O - A
  const Tuple origCrossEdge1 = v0ToOrig.cross(tri.e0);
  const float v = invDet * dir.dot(origCrossEdge1);
  if (v < 0.0f || u + v > 1.0f)
    return;

  // Replace the first column by vector O-A
  const float t = invDet * tri.e1.dot(origCrossEdge1);
  if (t > EPSILON<float>) {
    // Record the barycentric coordinates so the normal can be interpolated
    intersections.pushBack(Intersection{&object, t, u, v, triangleIndex});
  }
}

void localIntersect(const Ray &objectSpaceRay, const WorldObject &object, Arena<Intersection> &intersections,
                    const std::vector<CircularSolidData> &circularObjectData,
                    const std::vector<TriangleData> &triObjectData,
                    const std::vector<MeshData> &meshObjectData) noexcept {
  const Tuple &dir = objectSpaceRay.direction;
  const Tuple &orig = objectSpaceRay.origin;
  const int32_t dataIdx = object.shapeTag.dataIndex;
  switch (object.shapeTag.type) {
    case ShapeType::Sphere: {
      // For now we assume that the sphere is always at the origin
      const auto centerToRay = orig - Point(0, 0, 0);
      const auto a = dir.dot(dir);
      const auto b = 2 * dir.dot(centerToRay);
      const auto c = centerToRay.dot(centerToRay) - 1; // where 1 is the radius of the sphere

      const float disc = b * b - 4 * a * c;
      if (disc < 0) {
        return;
      }
      const auto dist1 = (-b - utility::sqrt(disc)) / (2 * a);
      const auto dist2 = (-b + utility::sqrt(disc)) / (2 * a);
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

      if (tmin > tmax)
        return;
      intersections.pushBack(Intersection{&object, tmin});
      intersections.pushBack(Intersection{&object, tmax});
      break;
    }

    case ShapeType::Cylinder: {
      float min = circularObjectData[dataIdx].minimum;
      float max = circularObjectData[dataIdx].maximum;
      bool closed = circularObjectData[dataIdx].closed;
      float dx = dir.x, dz = dir.z;
      float ox = orig.x, oz = orig.z;
      float a = dx * dx + dz * dz;
      float b = 2 * ox * dx + 2 * oz * dz;
      float c = ox * ox + oz * oz - 1;

      float upperAndLowerCapRadiusSq = 1.f;
      addCircularCapIntersections(orig, dir, min, max, closed, upperAndLowerCapRadiusSq, upperAndLowerCapRadiusSq,
                                  intersections, object);
      if (floatNearlyEqual(a, 0.f)) { // ray is parallel to the y axis
        return;
      }

      addCircularSideIntersections(a, b, c, orig, dir, min, max, object, intersections);
      break;
    }

    case ShapeType::Cone: {
      float min = circularObjectData[dataIdx].minimum;
      float max = circularObjectData[dataIdx].maximum;
      bool closed = circularObjectData[dataIdx].closed;
      float dx = dir.x, dy = dir.y, dz = dir.z;
      float ox = orig.x, oy = orig.y, oz = orig.z;
      float a = dx * dx - dy * dy + dz * dz;
      float b = 2 * ox * dx - 2 * oy * dy + 2 * oz * dz;
      float c = ox * ox - oy * oy + oz * oz;

      float upperCapRadiusSq = max * max;
      float lowerCapRadiusSq = min * min;
      addCircularCapIntersections(orig, dir, min, max, closed, upperCapRadiusSq, lowerCapRadiusSq, intersections,
                                  object);
      if (floatNearlyEqual(a, 0.f)) { // ray is parallel to one of the cone's halves
        if (floatNearlyEqual(b, 0.f)) {
          return;
        }
        intersections.pushBack(Intersection{&object, -c / (2 * b)});
        return;
      }

      addCircularSideIntersections(a, b, c, orig, dir, min, max, object, intersections);
      break;
    }

    case ShapeType::Triangle: {
      addTriangleIntersection(triObjectData[dataIdx], orig, dir, object, dataIdx, intersections);
      break;
    }

    case ShapeType::Mesh: {
      const MeshData &mesh = meshObjectData[dataIdx];
      for (int32_t i = 0; i < mesh.triangleCount; ++i) {
        const int32_t triangleIndex = mesh.firstTriangleIndex + i;
        addTriangleIntersection(triObjectData[triangleIndex], orig, dir, object, triangleIndex, intersections);
      }
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

Tuple normalAt(const WorldObject &object, const Tuple &point, const std::vector<CircularSolidData> &circularObjectData,
               const std::vector<TriangleData> &triObjectData, float u, float v, int32_t triangleIndex) noexcept {
  auto objectSpacePoint = object.inverseTransform * point;
  const int32_t dataIdx = object.shapeTag.dataIndex;
  Tuple normal;
  switch (object.shapeTag.type) {
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
      if (ax >= ay && ax >= az) {
        normal = Vector(objectSpacePoint.x, 0, 0);
      } else if (ay >= ax && ay >= az) {
        normal = Vector(0, objectSpacePoint.y, 0);
      } else {
        normal = Vector(0, 0, objectSpacePoint.z);
      }
      break;
    }

    case ShapeType::Cylinder: {
      float min = circularObjectData[dataIdx].minimum;
      float max = circularObjectData[dataIdx].maximum;
      float dist = objectSpacePoint.x * objectSpacePoint.x + objectSpacePoint.z * objectSpacePoint.z;
      if (dist < 1 && objectSpacePoint.y >= max - EPS) {
        normal = Vector(0, 1, 0);
      } else if (dist < 1 && objectSpacePoint.y <= min + EPS) {
        normal = Vector(0, -1, 0);
      } else {
        normal = Vector(objectSpacePoint.x, 0, objectSpacePoint.z);
      }
      break;
    }

    case ShapeType::Cone: {
      float min = circularObjectData[dataIdx].minimum;
      float max = circularObjectData[dataIdx].maximum;
      float dist = objectSpacePoint.x * objectSpacePoint.x + objectSpacePoint.z * objectSpacePoint.z;
      if (objectSpacePoint.y <= min + EPS && dist <= min * min) {
        normal = Vector(0, -1, 0); // Lower cap
      } else if (objectSpacePoint.y >= max - EPS && dist <= max * max) {
        normal = Vector(0, 1, 0); // Upper cap
      } else {
        normal = Vector(objectSpacePoint.x, -objectSpacePoint.y, objectSpacePoint.z);
      }
      break;
    }

    case ShapeType::Triangle: {
      // Interpolate the three vertex normals using the barycentric coordinates
      // of the hit. u weights vertex 1, v weights vertex 2, and the remainder
      // (1 - u - v) weights vertex 0.
      const TriangleData &tri = triObjectData[dataIdx];
      normal = tri.n1 * u + tri.n2 * v + tri.n0 * (1.0f - u - v);
      break;
    }

    case ShapeType::Mesh: {
      // The intersection recorded which triangle of the mesh was hit
      const TriangleData &tri = triObjectData[triangleIndex];
      normal = tri.n1 * u + tri.n2 * v + tri.n0 * (1.0f - u - v);
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
