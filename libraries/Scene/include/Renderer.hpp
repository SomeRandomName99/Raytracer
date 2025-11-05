#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Color.hpp"
#include "Ray.hpp"
#include "World.hpp"

namespace raytracer::scene{ 
using namespace utility;

Color colorAt(const Ray& ray, const World& world, size_t recursionLimit = 5) noexcept;

} // namespace raytracer::scene

#endif // RENDERER_HPP