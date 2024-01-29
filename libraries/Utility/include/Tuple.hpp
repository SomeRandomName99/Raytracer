#ifndef TUPLE_H
#define TUPLE_H

#include <ostream>
#include <array>

namespace raytracer {
namespace utility {

class Tuple {
public:
    explicit Tuple(float x, float y, float z, float w): data{x,y,z,w}{};
    Tuple() = default;

    bool isVector() const;
    bool isPoint() const;

    bool operator==(const Tuple& rhs)  const;
    Tuple operator+(const Tuple& rhs)  const;
    Tuple operator-(const Tuple& rhs)  const;
    Tuple operator*(const float& rhs) const;
    Tuple operator/(const float& rhs) const;
    Tuple operator-() const;

    float magnitude() const;
    Tuple normalize() const;

    float dot(const Tuple& rhs) const;
    Tuple cross(const Tuple& rhs) const;
    Tuple reflect(const Tuple& reflect) const;
    
    const float& x() const;
    float& x();
    const float& y() const;
    float& y();
    const float& z() const;
    float& z();
    const float& w() const;
    float& w();

    std::array<float, 4> data;
};
Tuple Point(float x,float y, float z);
Tuple Vector(float x,float y, float z);

std::ostream& operator<<(std::ostream& os, const Tuple& rhs);

} // namespace utility
} // namespace raytracer

#endif // TUPLE_H