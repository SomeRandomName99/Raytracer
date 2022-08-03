#ifndef TUPLE_H
#define TUPLE_H

class Tuple {
public:
    Tuple(float x, float y, float z, float w);
    ~Tuple() = default;

    float x;
    float y;
    float z;
    float w;
};

#endif // TUPLE_H