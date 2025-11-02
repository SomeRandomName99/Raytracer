#include "Pattern.hpp"
#include "Tuple.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

namespace raytracer::material {
  void setPatternTransform(Pattern& pattern, const utility::Matrix<4, 4>& transform) {
    pattern.transform = transform;
    pattern.inverseTransform = utility::inverse(transform);
  }

  utility::Color drawPatternAt(const Pattern& pattern, const utility::Tuple& point) {
    float noise = 0.0f;
    if(pattern.preturb){
      auto noise = stb_perlin_noise3(point.x, point.y, point.z, 0, 0, 0);
    }
    utility::Tuple objectPoint = pattern.inverseTransform * (point + utility::Vector(noise, noise, noise));
    switch (pattern.type) {
      case PatternType::Gradient: {
        utility::Color distance = pattern.data.b - pattern.data.a;
        auto fraction = objectPoint.x - std::floor(objectPoint.x);
        return pattern.data.a + distance * fraction;
      }
      case PatternType::Checker: {
        if ((static_cast<int>(std::floor(objectPoint.x) + std::floor(objectPoint.y) + std::floor(objectPoint.z)) % 2) == 0) {
          return pattern.data.a;
        } else {
          return pattern.data.b;
        }
      }
      case PatternType::Ring: {
        double dist = std::sqrt(objectPoint.x * objectPoint.x + objectPoint.z * objectPoint.z);
        if (static_cast<int>(std::floor(dist)) % 2 == 0) {
          return pattern.data.a;
        } else {
          return pattern.data.b;
        }
      }
      case PatternType::Stripe: {
        if (static_cast<int>(std::floor(objectPoint.x)) % 2 == 0) {
          return pattern.data.a;
        } else {
          return pattern.data.b;
        }
      }
      default:
        return utility::Color(0, 0, 0); // Fallback color
    }
  }
}