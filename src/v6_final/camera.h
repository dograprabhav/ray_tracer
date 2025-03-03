#ifndef CAMERA_H
#define CAMERA_H

#include "commons.h"
#include "hittable.h"
#include "material.h"

struct camera_config {
    double aspect_ratio;
    int image_width;
    int samples_per_pixel;
    int max_depth;
    int vfov;
    point3 camera_lookfrom;
    point3 camera_lookat;
    vec3 vup;
    double defocus_angle;
    double focus_dist;
};

class camera
{
private:
    int image_width = 100;          // Rendered image width in pixel count
    double aspect_ratio = 1.0;      // Ratio of image width over height
    int image_height;               // Rendered image height
    point3 camera_center;           // Camera center
    point3 pixel_upper_left_center; // Location of pixel (0, 0) center
    vec3 pixel_delta_u;             // Offset to pixel to the right
    vec3 pixel_delta_v;             // Offset to pixel below
    int samples_per_pixel = 100;    // Count of random samples for each pixel
    double pixel_samples_scale;     // Color scale factor for a sum of pixel samples
    int max_depth = 10;             // Maximum number of ray bounces into scene

    double vfov = 90;                           // Vertical view angle (field of view)
    point3 camera_lookfrom = point3(0,0,0);     // Point camera is looking from
    point3 camera_lookat   = point3(0,0,-1);    // Point camera is looking at
    vec3   vup      = vec3(0,1,0);              // Camera-relative "up" direction
    vec3   defocus_disk_u;                      // Defocus disk horizontal radius
    vec3   defocus_disk_v;                      // Defocus disk vertical radius
    double defocus_angle = 0;                   // Variation angle of rays through each pixel
    double focus_dist = 10;                     // Distance from camera lookfrom point to plane of perfect focus



    void initialize() {
        image_height = int(image_width / aspect_ratio);
        if (image_height < 1)
        {
            image_height = 1;
        }

        // It computes the scaling factor for averaging the color values of multiple samples per pixel.
        // Since multiple rays are cast per pixel (samples_per_pixel), their accumulated color values need to be averaged.
        // Multiplying by pixel_samples_scale ensures that the final pixel color is normalized to stay within the correct range.
        pixel_samples_scale = 1.0 / samples_per_pixel;
        // Angle of the camera view in radians
        auto theta = degrees_to_radians(vfov);

        // Defining the camera center: For simpler calculations lets set the center at (0, 0, 0)
        // A point in 3D space from which all scene rays will originate (this is also commonly referred to as the eye point).
        // The vector from the camera center to the viewport center will be orthogonal (perpendicular) to the viewport.
        // We'll initially set the distance between the viewport and the camera center point to be one unit.
        // This distance is often referred to as the focal length.
        // Check "src/v2/how_it_works.png"
        camera_center = camera_lookfrom;
        // A viewport is an imaginary rectangle in the 3D world through which rays are cast to create an image.
        // It acts like a camera screen that captures the scene.
        // Each point on the viewport corresponds to a pixel in the final image.
        // Check "src/how_it_works.png"
        // Refer to diagram in 12.1 for this
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);
        // We don't just use aspect_ratio when computing viewport_width,
        // it's because the value set to aspect_ratio is the ideal ratio,
        // it may not be the actual ratio between image_width and image_height.
        // If image_height was allowed to be real valued—rather than just an integer—then it would be fine to use aspect_ratio.

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        auto w = unit_vector(camera_lookfrom - camera_lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);        

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        auto viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left_corner_vector = camera_center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel_upper_left_center = viewport_upper_left_corner_vector + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;        
    }

    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        if (depth <= 0)
            return color(0,0,0);
        hit_record rec;

        // If ray hits sphere
        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;
            // const ray& r_in         <- Incoming ray hitting the surface
            // const hit_record& rec   <- Information about the hit point (position, normal, etc.)
            // color& attenuation      <- How much light the material absorbs or reflects
            // ray& scattered          <- The scattered (reflected/refracted) ray        
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        // If the ray doesnt hit the sphere then do nothing and just put a edges to center liner blend
        // Linear blend:
        // blendedValue = (1 - x) * min_color_intensity + x * max_color_intensity
        // Here, x is a linear value from 0 to 1
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        auto colorWhite = color(1.0, 1.0, 1.0);
        auto colorLightBlue = color(0.5, 0.7, 1.0);
        return (1.0 - a) * colorWhite + a * colorLightBlue;
    }

    ray get_ray(int i, int j) const
    {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel_upper_left_center + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        // It determines the origin of the ray being traced from the camera.
        // - If defocus_angle is 0 or negative, the ray originates from the camera center.
        // - If defocus_angle is greater than 0, the ray originates from a randomly sampled point on the defocus disk.

        // This is needed to simulate the camera's depth of field, in an actual camera:
        // - Objects exactly at this focus distance appear sharp.
        // - Objects closer or farther than the focus distance become blurred due to the way light rays spread.

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    // Returns the vector to a random point in the [-0.5, -0.5] - [+0.5, +0.5] unit square.
    vec3 sample_square() const
    {
        // "random_double" returns a random number in range [0, 1]
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

public:
    camera(const camera_config& config) : 
    aspect_ratio(config.aspect_ratio), 
    image_width(config.image_width), 
    max_depth(config.max_depth),
    vfov(config.vfov),
    camera_lookfrom(config.camera_lookfrom),
    camera_lookat(config.camera_lookat),
    vup(config.vup),
    defocus_angle(config.defocus_angle),
    focus_dist(config.focus_dist)
    {}

    void render(const hittable &world)
    {
        initialize();

        // P3 image format
        // P3 is a plain text format for Portable Pixmap (PPM) image files.
        // It is one of the simplest image formats, where pixel data is represented in ASCII text.
        // In P3, each pixel is defined by three integers corresponding to the red, green, and blue color channels.
        // The first line of the output is "P3", identifying the file format.
        // The second line contains the width and height of the image.
        // The third line specifies the maximum color value (typically 255, representing the maximum intensity for each color channel).
        // Each subsequent line contains three integers (r, g, b) for each pixel's color in the image.
        std::cout << "P3\n";
        std::cout << image_width << ' ' << image_height << '\n';
        std::cout << "255\n";

        for (int j = 0; j < image_height; j++)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                color pixel_color(0, 0, 0);
                // Anti-Aliasing using supersampling technique
                // Rendered images often show jagged edges, known as aliasing, due to point sampling.
                // Real - world images appear smooth because they blend foreground and background colors.
                // To mimic this, we average multiple samples per pixel, simulating how our eyes perceive distant details.
                // A simple approach is to sample light within a pixel’s surrounding area to approximate a continuous image.                                                                                                                                                                                                                              
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone                  \n";
    }
};

#endif