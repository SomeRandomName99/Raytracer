#ifndef TUPLE_H
#define TUPLE_H

class Tuple {
public:
    Tuple(float x, float y, float z, float w);
    ~Tuple() = default;

    bool isVector() const;
    bool isPoint() const;

    bool operator==(const Tuple& rhs)  const;
    Tuple operator+(const Tuple& rhs)  const;
    Tuple operator-(const Tuple& rhs)  const;
    Tuple operator*(const double& rhs) const;
    Tuple operator/(const double& rhs) const;
    Tuple operator-() const;

    double magnitude() const;
    Tuple nomrmalize() const;

    double dot(const Tuple& rhs) const;
    Tuple cross(const Tuple& rhs) const;


    float x;
    float y;
    float z;
    float w;
};
Tuple Point(float x,float y, float z);
Tuple Vector(float x,float y, float z);

#endif // TUPLE_H