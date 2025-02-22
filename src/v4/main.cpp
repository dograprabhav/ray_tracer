#include "commons.h"
#include "camera.h"
#include "interval.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

int main()
{   
    // World
    hittable_list world;
    // Adds 2 "hittable" abjects the "hittable_list"
    // Here "hittable" is an abstract base class
    // And "sphere" is a derived from "hittable"
    // The two spheres together create a simple scene :
    // A small sphere floating in space.A large sphere acting as the ground.
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam(16.0 / 9.0, 400);
    cam.render(world);
}
