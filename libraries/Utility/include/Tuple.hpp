#ifndef TUPLE_H
#define TUPLE_H

#include <cmath>
#include <iostream>
#include <algorithm>
#include "FloatUtils.hpp"

namespace raytracer {
namespace utility {

struct alignas(16) Tuple {
    float x, y, z, w;

    explicit Tuple(const float x, const float y, const float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
    explicit Tuple() noexcept = default;

    inline bool isVector() const noexcept {
        return floatNearlyEqual(this->w, 0.0f);
    }
    inline bool isPoint() const noexcept {
        return floatNearlyEqual(this->w, 1.0f);
    }
    inline bool operator==(const Tuple& rhs) const noexcept {
        bool xEqual = floatNearlyEqual(this->x, rhs.x);
        bool yEqual = floatNearlyEqual(this->y, rhs.y);
        bool zEqual = floatNearlyEqual(this->z, rhs.z);
        bool wEqual = floatNearlyEqual(this->w, rhs.w);
        return xEqual && yEqual && zEqual && wEqual;
    }
    inline Tuple operator+(const Tuple& rhs) const noexcept {
        return Tuple(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }
    inline Tuple operator-(const Tuple& rhs) const noexcept {
        return Tuple(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }
    inline Tuple operator*(const float& rhs) const noexcept {
        return Tuple(x * rhs, y * rhs, z * rhs, w * rhs);
    }
    inline Tuple operator/(const float& rhs) const noexcept {
        return Tuple(x / rhs, y / rhs, z / rhs, w / rhs);
    }
    inline Tuple operator-() const noexcept {
        return *this * -1;
    }
    float magnitude() const noexcept {
        return std::sqrt(x * x + y * y + z * z);
    }
    inline Tuple normalize() const noexcept {
        const auto magnitude = this->magnitude();
        return Tuple(x / magnitude, y / magnitude, z / magnitude, w);
    }
    inline float dot(const Tuple& rhs) const noexcept {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }
    inline Tuple cross(const Tuple& rhs) const noexcept {
        return Tuple{
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x,
            0
        };
    }
    inline Tuple reflect(const Tuple& normal) const noexcept {
        return *this - (normal * 2 * this->dot(normal));
    }

};

// Definitions after the class
inline Tuple Point(const float x, const float y, const float z) noexcept {
    return Tuple(x, y, z, 1);
}
inline Tuple Vector(const float x, const float y, const float z) noexcept {
    return Tuple(x, y, z, 0);
}

inline Tuple operator*(const Tuple& lhs, const Tuple& rhs) noexcept {
    return Tuple{lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
}

inline std::ostream& operator<<(std::ostream& os, const Tuple& rhs) noexcept {
    os << rhs.x << ' ' << rhs.y << ' ' << rhs.z << ' ' << rhs.w << '\n';
    return os;
}

// It is assumed that the tuples are of the same type (point or vector)
inline Tuple componentWiseMin(const Tuple& lhs, const Tuple& rhs) noexcept {
    return Tuple{
        std::min(lhs.x, rhs.x),
        std::min(lhs.y, rhs.y),
        std::min(lhs.z, rhs.z),
        lhs.w
    };
}

// It is assumed that the tuples are of the same type (point or vector)
inline Tuple componentWiseMax(const Tuple& lhs, const Tuple& rhs) noexcept {
    return Tuple{
        std::max(lhs.x, rhs.x),
        std::max(lhs.y, rhs.y),
        std::max(lhs.z, rhs.z),
        lhs.w
    };
}

} // namespace utility
} // namespace raytracer

#endif // TUPLE_H