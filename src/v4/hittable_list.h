#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

// This line defines a C++ class named hittable_list that inherits from hittable.
// "hittable" is an abstract base class, it defines a common interface for any object that can be hit by a ray
class hittable_list : public hittable
{
public:
    /*
        "shared_ptr" is smart pointer from the C++ Standard Library (<memory>).
        shared_ptr<hittable> = hittable*
        "shared_ptr" provides Automatic Memory Management:
            -> No need to manually delete objects.
            -> When the last shared_ptr goes out of scope, the object is deleted.
    */
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    // hit(ray(t), interval(tmin, tmax), hit_record)
    // ray(t) -> ray as a function of time
    // interval(tmin, tmax) -> time interval tmin to tmax
    // hit_record -> Records any hits between 'ray(t)' and vector<shared_ptr<hittable>> objects
    // hit function iterates over all the objects and checks if any ray hits any of the objects
    // If a ray hits multiple objects it stores the object which is hit first
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        // If a ray hits multiple objects it stores the object which is hit first
        for (const auto &object : objects)
        {
            // Logic to smartly determine which object is hit first and store that in "hit_rec"
            if ((*object).hit(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif