#ifndef TUPLE_H
#define TUPLE_H

#include <ostream>
#include <array>

namespace raytracer {
namespace utility {

class Tuple {
public:
    explicit Tuple(const double x, const double y, const double z, double w) noexcept: data{x,y,z,w}{};
    Tuple() noexcept = default;

    bool isVector() const noexcept;
    bool isPoint() const noexcept;

    bool operator==(const Tuple& rhs) const noexcept;
    Tuple operator+(const Tuple& rhs) const noexcept;
    Tuple operator-(const Tuple& rhs) const noexcept;
    Tuple operator*(const double& rhs) const noexcept;
    Tuple operator/(const double& rhs) const noexcept;
    Tuple operator-() const noexcept;

    double magnitude() const noexcept;
    Tuple normalize() const noexcept;

    double dot(const Tuple& rhs) const noexcept;
    Tuple cross(const Tuple& rhs) const noexcept;
    Tuple reflect(const Tuple& reflect) const noexcept;

    const double& x() const noexcept;
    double& x() noexcept;
    const double& y() const noexcept;
    double& y() noexcept;
    const double& z() const noexcept;
    double& z() noexcept;
    const double& w() const noexcept;
    double& w() noexcept;
    
    std::array<double, 4> data;
};
Tuple Point(const double x, const double y, const double z) noexcept;
Tuple Vector(const double x, const double y, const double z) noexcept;

std::ostream& operator<<(std::ostream& os, const Tuple& rhs) noexcept;

} // namespace utility
} // namespace raytracer

#endif // TUPLE_H