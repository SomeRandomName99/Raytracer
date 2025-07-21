#ifndef TUPLE_H
#define TUPLE_H

#include <ostream>
#include <array>
#include <cmath>
#include <algorithm>

#include "FloatUtils.hpp"

namespace raytracer {
namespace utility {

class Tuple {
public:
    explicit Tuple(const double x, const double y, const double z, double w) noexcept : data{x, y, z, w} {}
    explicit Tuple() noexcept = default;

    inline bool isVector() const noexcept {
        return floatNearlyEqual(this->w(), 0.0f);
    }
    inline bool isPoint() const noexcept {
        return floatNearlyEqual(this->w(), 1.0f);
    }
    inline bool operator==(const Tuple& rhs) const noexcept {
        bool xEqual = floatNearlyEqual(this->x(), rhs.x());
        bool yEqual = floatNearlyEqual(this->y(), rhs.y());
        bool zEqual = floatNearlyEqual(this->z(), rhs.z());
        bool wEqual = floatNearlyEqual(this->w(), rhs.w());
        return xEqual && yEqual && zEqual && wEqual;
    }
    inline Tuple operator+(const Tuple& rhs) const noexcept {
        return Tuple(x() + rhs.x(), y() + rhs.y(), z() + rhs.z(), w() + rhs.w());
    }
    inline Tuple operator-(const Tuple& rhs) const noexcept {
        return Tuple(x() - rhs.x(), y() - rhs.y(), z() - rhs.z(), w() - rhs.w());
    }
    inline Tuple operator*(const double& rhs) const noexcept {
        return Tuple(x() * rhs, y() * rhs, z() * rhs, w() * rhs);
    }
    inline Tuple operator/(const double& rhs) const noexcept {
        return Tuple(x() / rhs, y() / rhs, z() / rhs, w() / rhs);
    }
    inline Tuple operator-() const noexcept {
        return *this * -1;
    }
    double magnitude() const noexcept {
        return std::sqrt(x() * x() + y() * y() + z() * z() + w() * w());
    }
    inline Tuple normalize() const noexcept {
        return *this / this->magnitude();
    }
    inline double dot(const Tuple& rhs) const noexcept {
        return x() * rhs.x() + y() * rhs.y() + z() * rhs.z() + w() * rhs.w();
    }
    inline Tuple cross(const Tuple& rhs) const noexcept {
        return Tuple{
            y() * rhs.z() - z() * rhs.y(),
            z() * rhs.x() - x() * rhs.z(),
            x() * rhs.y() - y() * rhs.x(),
            0
        };
    }
    inline Tuple reflect(const Tuple& normal) const noexcept {
        return *this - (normal * 2 * this->dot(normal));
    }
    inline const double& x() const noexcept { return const_cast<double&>(const_cast<const Tuple*>(this)->data[0]); }
    inline double& x() noexcept { return data[0]; }
    inline const double& y() const noexcept { return const_cast<double&>(const_cast<const Tuple*>(this)->data[1]); }
    inline double& y() noexcept { return data[1]; }
    inline const double& z() const noexcept { return const_cast<double&>(const_cast<const Tuple*>(this)->data[2]); }
    inline double& z() noexcept { return data[2]; }
    inline const double& w() const noexcept { return const_cast<double&>(const_cast<const Tuple*>(this)->data[3]); }
    inline double& w() noexcept { return data[3]; }

    std::array<double, 4> data;
};

// Definitions after the class
inline Tuple Point(const double x, const double y, const double z) noexcept {
    return Tuple(x, y, z, 1);
}
inline Tuple Vector(const double x, const double y, const double z) noexcept {
    return Tuple(x, y, z, 0);
}

inline Tuple operator*(const Tuple& lhs, const Tuple& rhs) noexcept {
    return Tuple{lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z(), lhs.w() * rhs.w()};
}

inline std::ostream& operator<<(std::ostream& os, const Tuple& rhs) noexcept {
    os << rhs.x() << ' ' << rhs.y() << ' ' << rhs.z() << ' ' << rhs.w() << '\n';
    return os;
}

// It is assumed that the tuples are of the same type (point or vector)
inline Tuple componentWiseMin(const Tuple& lhs, const Tuple& rhs) noexcept {
    return Tuple{
        std::min(lhs.x(), rhs.x()),
        std::min(lhs.y(), rhs.y()),
        std::min(lhs.z(), rhs.z()),
        lhs.w()
    };
}

// It is assumed that the tuples are of the same type (point or vector)
inline Tuple componentWiseMax(const Tuple& lhs, const Tuple& rhs) noexcept {
    return Tuple{
        std::max(lhs.x(), rhs.x()),
        std::max(lhs.y(), rhs.y()),
        std::max(lhs.z(), rhs.z()),
        lhs.w()
    };
}

} // namespace utility
} // namespace raytracer

#endif // TUPLE_H