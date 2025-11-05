#include <iostream>
#include <cmath>
#include <string>
#include <numbers>

#include "Tuple.hpp"
#include "Canvas.hpp"
#include "Transformations.hpp"
#include "Ray.hpp"
#include "Intersections.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Pattern.hpp"
#include "World.hpp"
#include "Shape.hpp"
#include "Renderer.hpp"

using namespace raytracer;
using namespace material;
using namespace geometry;
using namespace scene;
using namespace utility;

// Simple test framework
int totalTests = 0;
int passedTests = 0;
int failedTests = 0;

#define TEST(name) \
  void test_##name(); \
  struct TestRegistrar_##name { \
    TestRegistrar_##name() { \
      std::cout << "\n[TEST] " << #name << std::endl; \
      totalTests++; \
      try { \
        test_##name(); \
        passedTests++; \
        std::cout << "  ✓ PASSED" << std::endl; \
      } catch (const std::exception& e) { \
        failedTests++; \
        std::cout << "  ✗ FAILED: " << e.what() << std::endl; \
      } \
    } \
  } testRegistrar_##name; \
  void test_##name()

#define ASSERT_TRUE(condition) \
  if (!(condition)) { \
    throw std::runtime_error("Assertion failed: " #condition); \
  }

#define ASSERT_FALSE(condition) \
  if (condition) { \
    throw std::runtime_error("Assertion failed: !" #condition); \
  }

#define ASSERT_EQ(a, b) \
  if ((a) != (b)) { \
    throw std::runtime_error("Assertion failed: " #a " == " #b); \
  }

#define ASSERT_NE(a, b) \
  if ((a) == (b)) { \
    throw std::runtime_error("Assertion failed: " #a " != " #b); \
  }

#define ASSERT_NEAR(a, b, epsilon) \
  if (std::abs((a) - (b)) > (epsilon)) { \
    throw std::runtime_error("Assertion failed: " #a " ≈ " #b); \
  }

#define ASSERT_GT(a, b) \
  if (!((a) > (b))) { \
    throw std::runtime_error("Assertion failed: " #a " > " #b); \
  }

#define ASSERT_LT(a, b) \
  if (!((a) < (b))) { \
    throw std::runtime_error("Assertion failed: " #a " < " #b); \
  }

#define ASSERT_COLOR_EQ(c1, c2) \
  if (std::abs((c1).red() - (c2).red()) > 0.0001 || \
      std::abs((c1).green() - (c2).green()) > 0.0001 || \
      std::abs((c1).blue() - (c2).blue()) > 0.0001) { \
    throw std::runtime_error("Colors not equal"); \
  }

// =================== Basic Ray-Sphere Intersection Tests ===================

TEST(RayIntersectsSphereAtTwoPoints) {
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 1.0;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  auto xs = colorAt(ray, world, 5);
  // If it intersects, color should not be black
  ASSERT_NE(xs, Color(0, 0, 0));
}

TEST(RayIntersectsSphereAtTangent) {
  Ray ray(Point(0, 1, -5), Vector(0, 0, 1));
  
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 1.0; // Make it fully lit
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  auto color = colorAt(ray, world, 5);
  // Should hit at tangent
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(RayMissesSphere) {
  Ray ray(Point(0, 2, -5), Vector(0, 0, 1));
  
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_EQ(color, Color(0, 0, 0));
}

TEST(RayOriginatesInsideSphere) {
  Ray ray(Point(0, 0, 0), Vector(0, 0, 1));
  
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 1.0;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(0,0,0)));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(SphereBehindRay) {
  Ray ray(Point(0, 0, 5), Vector(0, 0, 1));
  
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_EQ(color, Color(0, 0, 0));
}

// =================== Transformed Sphere Tests ===================

TEST(IntersectingScaledSphere) {
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 1.0;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));

  addTransformToObject(world, 0, transformations::scaling(2, 2, 2));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(IntersectingTranslatedSphere) {
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 1.0;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  addTransformToObject(world, 0, transformations::translation(5, 0, 0));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_EQ(color, Color(0, 0, 0)); // Sphere moved out of ray's path
}

// =================== Plane Intersection Tests ===================

TEST(RayIntersectsPlaneFromAbove) {
  Ray ray(Point(0, 1, 0), Vector(0, -1, 0));
  
  World world;
  WorldObject plane{ShapeTypeTag{ShapeType::Plane}};
  auto material = createDefaultMaterial();
  material.ambient = 1.0;
  plane.MaterialIndex = addMaterial(world, material);
  addObject(world, plane);
  addLight(world, PointLight(Color(1,1,1), Point(0,10,0)));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(RayParallelToPlane) {
  Ray ray(Point(0, 10, 0), Vector(0, 0, 1));
  
  World world;
  WorldObject plane{ShapeTypeTag{ShapeType::Plane}};
  auto material = createDefaultMaterial();
  plane.MaterialIndex = addMaterial(world, material);
  addObject(world, plane);
  addLight(world, PointLight(Color(1,1,1), Point(0,10,0)));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_EQ(color, Color(0, 0, 0));
}

TEST(RayIntersectsPlaneFromBelow) {
  Ray ray(Point(0, -1, 0), Vector(0, 1, 0));
  
  World world;
  WorldObject plane{ShapeTypeTag{ShapeType::Plane}};
  auto material = createDefaultMaterial();
  material.ambient = 1.0;
  plane.MaterialIndex = addMaterial(world, material);
  addObject(world, plane);
  addLight(world, PointLight(Color(1,1,1), Point(0,10,0)));
  
  auto color = colorAt(ray, world, 5);
  ASSERT_NE(color, Color(0, 0, 0));
}

// =================== Lighting Tests ===================

TEST(LightingWithEyeBetweenLightAndSurface) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 0.1;
  material.diffuse = 0.9;
  material.specular = 0.9;
  material.shininess = 200;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(0,0,-10)));
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should be bright with specular highlight
  ASSERT_TRUE(color.red() > 1.0);
}

TEST(LightingWithEyeOppositeLight) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 0.1;
  material.diffuse = 0.9;
  material.specular = 0.9;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(0,0,10)));
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should only have ambient lighting
  ASSERT_NEAR(color.red(), 0.1, 0.01);
}

TEST(LightingWithSurfaceInShadow) {
  World world;
  
  // First sphere blocks light
  WorldObject sphere1{ShapeTypeTag{ShapeType::Sphere}};
  sphere1.MaterialIndex = addMaterial(world, createDefaultMaterial());
  auto idx_blocker = addObject(world, sphere1);
  
  // Second sphere behind first
  WorldObject sphere2{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.ambient = 0.1;
  sphere2.MaterialIndex = addMaterial(world, material);
  auto idx_shadow = addObject(world, sphere2);
  addTransformToObject(world, idx_shadow, transformations::translation(0, 0, 10));
  
  addLight(world, PointLight(Color(1,1,1), Point(0,0,-10)));
  
  Ray ray(Point(0, 0, 5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should only have ambient component
  ASSERT_NEAR(color.red(), 0.1, 0.01);
}

// =================== Reflection Tests ===================

TEST(ReflectingOffNonReflectiveSurface) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.reflectance = 0.0;
  material.ambient = 1.0;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should just be surface color, no reflections
  ASSERT_TRUE(color.red() < 1.5);
}

TEST(ReflectingOffReflectiveSurface) {
  World world;
  
  // Reflective plane
  WorldObject plane{ShapeTypeTag{ShapeType::Plane}};
  auto planeMaterial = createDefaultMaterial();
  planeMaterial.reflectance = 0.5;
  plane.MaterialIndex = addMaterial(world, planeMaterial);
  auto idx_plane_ref = addObject(world, plane);
  addTransformToObject(world, idx_plane_ref, transformations::translation(0, -1, 0));
  
  // Sphere above plane
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto sphereMaterial = createDefaultMaterial();
  sphereMaterial.surfaceColor = Color(1, 0, 0);
  sphere.MaterialIndex = addMaterial(world, sphereMaterial);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  Ray ray(Point(0, 0, -3), Vector(0, -std::sqrt(2)/2, std::sqrt(2)/2));
  auto color = colorAt(ray, world, 5);
  
  // Should have some color from reflection
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(ReflectionRecursionTermination) {
  World world;

  // Two parallel mirrors
  WorldObject lower{ShapeTypeTag{ShapeType::Plane}};
  auto material1 = createDefaultMaterial();
  material1.reflectance = 1.0;
  lower.MaterialIndex = addMaterial(world, material1);
  auto idx_lower = addObject(world, lower);
  addTransformToObject(world, idx_lower, transformations::translation(0, -1, 0));

  WorldObject upper{ShapeTypeTag{ShapeType::Plane}};
  auto material2 = createDefaultMaterial();
  material2.reflectance = 1.0;
  upper.MaterialIndex = addMaterial(world, material2);
  auto idx_upper = addObject(world, upper);
  addTransformToObject(world, idx_upper, transformations::translation(0, 1, 0));

  addLight(world, PointLight(Color(1,1,1), Point(0,0,0)));

  Ray ray(Point(0, 0, 0), Vector(0, 1, 0));
  auto color = colorAt(ray, world, 5);

  // If the recursion terminates correctly in a lit scene, the result
  // should not be black. This proves the function ran and computed a color.
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(ReflectionAtMaxRecursionDepth) {
  World world;
  WorldObject plane{ShapeTypeTag{ShapeType::Plane}};
  auto material = createDefaultMaterial();
  material.reflectance = 1.0;
  plane.MaterialIndex = addMaterial(world, material);
  auto idx_plane_max = addObject(world, plane);
  addTransformToObject(world, idx_plane_max, transformations::translation(0, -1, 0));
  
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  Ray ray(Point(0, 0, 0), Vector(0, -1, 0));
  auto color = colorAt(ray, world, 0); // Zero recursion
  
  ASSERT_EQ(color, Color(0, 0, 0));
}

// =================== Refraction Tests ===================

TEST(RefractedColorForOpaqueSurface) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.transparency = 0.0;
  material.ambient = 1.0;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should render without transparency
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(RefractedColorAtMaxRecursionDepth) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.transparency = 1.0;
  material.refractiveIndex = 1.5;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 0); // Zero recursion
  
  ASSERT_EQ(color, Color(0, 0, 0));
}

TEST(RefractedColorUnderTotalInternalReflection) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.transparency = 1.0;
  material.refractiveIndex = 1.5;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));

  // This ray is inside the sphere and strikes the edge at the critical angle,
  // which should trigger Total Internal Reflection.
  Ray ray(Point(0, 0, std::sqrt(2)/2), Vector(0, 1, 0));
  auto color = colorAt(ray, world, 5);

  // Under TIR, the refracted color is black. Since there are no other
  // objects in the scene, the reflected color is also black.
  ASSERT_EQ(color, Color(0, 0, 0));
}

TEST(RefractedColorWithRefractedRay) {
  World world;
  
  // Glass sphere
  WorldObject glassSphere{ShapeTypeTag{ShapeType::Sphere}};
  auto glassMaterial = createDefaultMaterial();
  glassMaterial.transparency = 0.9;
  glassMaterial.refractiveIndex = 1.5;
  glassMaterial.ambient = 0.0;
  glassMaterial.diffuse = 0.0;
  glassSphere.MaterialIndex = addMaterial(world, glassMaterial);
  auto idx_glass = addObject(world, glassSphere);
  
  // Colored sphere behind
  WorldObject coloredSphere{ShapeTypeTag{ShapeType::Sphere}};
  auto coloredMaterial = createDefaultMaterial();
  coloredMaterial.surfaceColor = Color(1, 0, 0);
  coloredMaterial.ambient = 1.0;
  coloredSphere.MaterialIndex = addMaterial(world, coloredMaterial);
  auto idx_colored = addObject(world, coloredSphere);
  addTransformToObject(world, idx_colored, transformations::translation(0, 0, 2));
  
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should show some red through glass
  ASSERT_GT(color.red(), 0.0);
}

// =================== Fresnel Effect Tests ===================

TEST(SchlickApproximationUnderTotalInternalReflection) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.transparency = 1.0;
  material.reflectance = 1.0; // Material is fully reflective/transparent
  material.refractiveIndex = 1.5;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));

  Ray ray(Point(0, 0, std::sqrt(2)/2), Vector(0, 1, 0));
  auto color = colorAt(ray, world, 5);

  // The Schlick approximation for TIR should result in a reflectance of 1.0.
  // This means the resulting color should be purely reflective. With an empty
  // scene, the reflected color is black.
  ASSERT_EQ(color, Color(0, 0, 0));
}

TEST(SchlickApproximationWithPerpendicularAngle) {
  World world;
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.transparency = 1.0;
  material.reflectance = 1.0;
  material.refractiveIndex = 1.5;
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  Ray ray(Point(0, 0, 0), Vector(0, 1, 0));
  auto color = colorAt(ray, world, 5);
  
  ASSERT_NE(color, Color(0, 0, 0));
}

TEST(SchlickApproximationWithSmallAngle) {
  World world;

  // A reflective, transparent sphere
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  material.transparency = 1.0;
  material.reflectance = 1.0;
  material.refractiveIndex = 1.5;
  sphere.MaterialIndex = addMaterial(world, material);
  auto idx_sch_sphere = addObject(world, sphere);

  // A green plane for the ray to reflect
  WorldObject plane{ShapeTypeTag{ShapeType::Plane}};
  auto plane_mat = createDefaultMaterial();
  plane_mat.surfaceColor = Color(0, 1, 0);
  plane_mat.ambient = 0.5; // Make it visible without direct light
  plane.MaterialIndex = addMaterial(world, plane_mat);
  auto idx_sch_plane = addObject(world, plane);
  addTransformToObject(world, idx_sch_plane, transformations::translation(0, -5, 0));

  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));

  // A ray that strikes the sphere at a grazing angle
  Ray ray(Point(0, 0.99, -2), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);

  // At a grazing angle, reflectance is high. The ray should reflect off the
  // sphere and hit the green plane below. The resulting color should have a
  // significant green component.
  ASSERT_GT(color.green(), 0.8);
}

// =================== Complex Scene Tests ===================

TEST(RenderingWorldWithMultipleObjects) {
  World world;
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));

  // Floor
  WorldObject floor{ShapeTypeTag{ShapeType::Plane}};
  auto floorMaterial = createDefaultMaterial();
  floor.MaterialIndex = addMaterial(world, floorMaterial);
  auto idx_floor2 = addObject(world, floor);
  addTransformToObject(world, idx_floor2, transformations::translation(0, -1, 0));

  // Sphere 1
  WorldObject sphere1{ShapeTypeTag{ShapeType::Sphere}};
  auto material1 = createDefaultMaterial();
  material1.surfaceColor = Color(1, 0, 0);
  sphere1.MaterialIndex = addMaterial(world, material1);
  auto idx_s1b = addObject(world, sphere1);

  // Sphere 2
  WorldObject sphere2{ShapeTypeTag{ShapeType::Sphere}};
  auto material2 = createDefaultMaterial();
  material2.surfaceColor = Color(0, 0, 1);
  sphere2.MaterialIndex = addMaterial(world, material2);
  auto idx_s2c = addObject(world, sphere2);
  addTransformToObject(world, idx_s2c, transformations::translation(2, 0, 0));
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should hit red sphere
  ASSERT_GT(color.red(), color.blue());
}

TEST(PatternOnSphere) {
  World world;
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
  auto material = createDefaultMaterial();
  
  Pattern pattern{PatternType::Checker, 
                  PatternData{Color(1, 1, 1), Color(0.25, 0.25, 0.25)}};
  material.patternIndex = addPattern(world, pattern);
  
  sphere.MaterialIndex = addMaterial(world, material);
  addObject(world, sphere);
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  ASSERT_NE(color, Color(0, 0, 0));

  auto testColor = Color(color.blue(), color.blue(), color.blue());
  ASSERT_EQ(color, testColor); // All channels should be equal as in checker pattern
  
}

TEST(MultipleIntersections) {
  World world;
  addLight(world, PointLight(Color(1,1,1), Point(-10,10,-10)));
  
  // Create multiple spheres along ray path
  for (int i = 0; i < 5; ++i) {
    WorldObject sphere{ShapeTypeTag{ShapeType::Sphere}};
    auto material = createDefaultMaterial();
    material.surfaceColor = Color(i * 0.2, 0, 0);
    sphere.MaterialIndex = addMaterial(world, material);
    auto idx_loop = addObject(world, sphere);
    addTransformToObject(world, idx_loop, transformations::translation(0, 0, i * 3));
  }
  
  Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
  auto color = colorAt(ray, world, 5);
  
  // Should hit nearest sphere (i=0, darkest red)
  ASSERT_TRUE(color.red() < 0.3);
}

// =================== Main ===================

int main() {
  std::cout << "\n═══════════════════════════════════════════════════════" << std::endl;
  std::cout << "  RAYTRACER BASIC FUNCTIONALITY TESTS" << std::endl;
  std::cout << "═══════════════════════════════════════════════════════\n" << std::endl;
  
  // Tests run automatically via static initialization
  
  std::cout << "\n═══════════════════════════════════════════════════════" << std::endl;
  std::cout << "  TEST SUMMARY" << std::endl;
  std::cout << "═══════════════════════════════════════════════════════" << std::endl;
  std::cout << "  Total tests:  " << totalTests << std::endl;
  std::cout << "  Passed:       " << passedTests << " ✓" << std::endl;
  std::cout << "  Failed:       " << failedTests << " ✗" << std::endl;
  std::cout << "═══════════════════════════════════════════════════════\n" << std::endl;
  
  return failedTests > 0 ? 1 : 0;
}
