#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
private:
    point3 orig;
    vec3 dir;

public:
    ray() {}
    // Any line in 3d plane can be represented by a point on the line and its direction
    ray(const point3 &origin, const vec3 &direction) : orig(origin), dir(direction) {}

    const point3 &origin() const { return orig; }
    const vec3 &direction() const { return dir; }

    // Calculate a point along a ray
    // Point(T) = A * T + B
    point3 at(double t) const
    {
        return orig + t * dir;
    }
};

#endif
