#ifndef HITTABLE_H
#define HITTABLE_H

#include "commons.h"
#include "interval.h"

class material;

class hit_record
{
public:
    point3 p;                   // Stores the point of intersection where the ray hits an object
    vec3 normal;                // Stores the direction of the surface normal at the hit point
    double t;                   // Stores the distance along the ray where the intersection occurs
    bool front_face;            // Indicates whether the ray hit the front face of the object
    shared_ptr<material> mat;   // Stores the material of the object

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter outward_normal is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual ~hittable() = default;

    // Pure virtual function, all derived classes must implement this
    // It checks whether a ray intersects the object and updates "hit_record &rec" with the hit details
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

#endif