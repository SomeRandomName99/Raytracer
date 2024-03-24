#ifndef TUPLE_H
#define TUPLE_H

#include <ostream>
#include <array>

namespace raytracer {
namespace utility {

class Tuple {
public:
    explicit Tuple(const double x, const double y, const double z, double w) noexcept: data{x,y,z,w}{};
    explicit Tuple() noexcept = default;

    bool isVector() const noexcept;
    bool isPoint() const noexcept;

    bool operator==(const Tuple& rhs) const noexcept;
    Tuple operator+(const Tuple& rhs) const noexcept;
    Tuple operator+(const double& rhs) const noexcept;
    Tuple operator-(const Tuple& rhs) const noexcept;
    Tuple operator*(const double& rhs) const noexcept;
    Tuple operator/(const double& rhs) const noexcept;
    Tuple operator-() const noexcept;

    double magnitude() const noexcept;
    Tuple normalize() const noexcept;

    double dot(const Tuple& rhs) const noexcept;
    Tuple cross(const Tuple& rhs) const noexcept;
    Tuple reflect(const Tuple& reflect) const noexcept;

    inline const double& x() const noexcept{return const_cast<double&>(const_cast<const Tuple*>(this)->data[0]);}
    inline double& x() noexcept {return data[0];}
    inline const double& y() const noexcept{return const_cast<double&>(const_cast<const Tuple*>(this)->data[1]);}
    inline double& y() noexcept {return data[1];}
    inline const double& z() const noexcept{return const_cast<double&>(const_cast<const Tuple*>(this)->data[2]);}
    inline double& z() noexcept {return data[2];}
    inline const double& w() const noexcept{return const_cast<double&>(const_cast<const Tuple*>(this)->data[3]);}
    inline double& w() noexcept {return data[3];}
    
    std::array<double, 4> data;
};
Tuple Point(const double x, const double y, const double z) noexcept;
Tuple Vector(const double x, const double y, const double z) noexcept;

std::ostream& operator<<(std::ostream& os, const Tuple& rhs) noexcept;
Tuple operator*(const Tuple& lhs, const Tuple& rhs) noexcept;

} // namespace utility
} // namespace raytracer

#endif // TUPLE_H