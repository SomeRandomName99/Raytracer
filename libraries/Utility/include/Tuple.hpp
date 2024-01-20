#ifndef TUPLE_H
#define TUPLE_H

#include <ostream>
#include <array>

class Tuple {
public:
    explicit Tuple(double x, double y, double z, double w): data{x,y,z,w}{};
    Tuple() = default;

    bool isVector() const;
    bool isPoint() const;

    bool operator==(const Tuple& rhs)  const;
    Tuple operator+(const Tuple& rhs)  const;
    Tuple operator-(const Tuple& rhs)  const;
    Tuple operator*(const double& rhs) const;
    Tuple operator/(const double& rhs) const;
    Tuple operator-() const;

    double magnitude() const;
    Tuple normalize() const;

    double dot(const Tuple& rhs) const;
    Tuple cross(const Tuple& rhs) const;
    Tuple reflect(const Tuple& reflect) const;
    
    const double& x() const;
    double& x();
    const double& y() const;
    double& y();
    const double& z() const;
    double& z();
    const double& w() const;
    double& w();

    std::array<double, 4> data;
};
Tuple Point(double x,double y, double z);
Tuple Vector(double x,double y, double z);

std::ostream& operator<<(std::ostream& os, const Tuple& rhs);

#endif // TUPLE_H