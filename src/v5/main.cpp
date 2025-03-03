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

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    // 2 spheres to imitate actual bubble
    // The first sphere is the actual bubble with radius 0.5
    // The second sphere is the bubble's surface with radius 0.4
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));

    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera_config config = {
        16.0 / 9.0,         // Aspect ratio
        400,                // Image width
        100,                // Samples per pixel
        50,                 // Max depth
        70,                 // Vertical field of view
        point3(-2, 2, 1),   // Look from
        point3(0, 0, -1),   // Look at
        vec3(0, 1, 0),      // Vertical up vector from camera
        10.0,               // Defocus angle
        3.4                 // Focus distance
    };
    camera cam(config);
    cam.render(world);
}
