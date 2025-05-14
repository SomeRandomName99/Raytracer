#ifndef TUPLE_H
#define TUPLE_H

#include <ostream>
#include <array>

namespace raytracer {
namespace utility {

class Tuple {
public:
    explicit Tuple(const float x, const float y, const float z, float w) noexcept: data{x,y,z,w}{};
    explicit Tuple() noexcept = default;

    bool isVector() const noexcept;
    bool isPoint() const noexcept;

    bool operator==(const Tuple& rhs) const noexcept;
    Tuple operator+(const Tuple& rhs) const noexcept;
    Tuple operator+(const float& rhs) const noexcept;
    Tuple operator-(const Tuple& rhs) const noexcept;
    Tuple operator*(const float& rhs) const noexcept;
    Tuple operator/(const float& rhs) const noexcept;
    Tuple operator-() const noexcept;

    float magnitude() const noexcept;
    Tuple normalize() const noexcept;

    float dot(const Tuple& rhs) const noexcept;
    Tuple cross(const Tuple& rhs) const noexcept;
    Tuple reflect(const Tuple& reflect) const noexcept;

    inline const float& x() const noexcept{return const_cast<float&>(const_cast<const Tuple*>(this)->data[0]);}
    inline float& x() noexcept {return data[0];}
    inline const float& y() const noexcept{return const_cast<float&>(const_cast<const Tuple*>(this)->data[1]);}
    inline float& y() noexcept {return data[1];}
    inline const float& z() const noexcept{return const_cast<float&>(const_cast<const Tuple*>(this)->data[2]);}
    inline float& z() noexcept {return data[2];}
    inline const float& w() const noexcept{return const_cast<float&>(const_cast<const Tuple*>(this)->data[3]);}
    inline float& w() noexcept {return data[3];}
    
    std::array<float, 4> data;
};
Tuple Point(const float x, const float y, const float z) noexcept;
Tuple Vector(const float x, const float y, const float z) noexcept;

std::ostream& operator<<(std::ostream& os, const Tuple& rhs) noexcept;
Tuple operator*(const Tuple& lhs, const Tuple& rhs) noexcept;

// It is assumed that the tuples are of the same type(point or vector)
Tuple componentWiseMin(const Tuple& lhs, const Tuple& rhs) noexcept;

// It is assumed that the tuples are of the same type(point or vector)
Tuple componentWiseMax(const Tuple& lhs, const Tuple& rhs) noexcept;

} // namespace utility
} // namespace raytracer

#endif // TUPLE_H