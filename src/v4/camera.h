#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

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

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        if (image_height < 1)
        {
            image_height = 1;
        }

        // Defining the camera center: For simpler calculations lets set the center at (0, 0, 0)
        // A point in 3D space from which all scene rays will originate (this is also commonly referred to as the eye point).
        // The vector from the camera center to the viewport center will be orthogonal (perpendicular) to the viewport.
        // We'll initially set the distance between the viewport and the camera center point to be one unit.
        // This distance is often referred to as the focal length.
        // Check "src/v2/how_it_works.png"
        auto focal_length = 1.0;
        camera_center = point3(0, 0, 0);
        // A viewport is an imaginary rectangle in the 3D world through which rays are cast to create an image.
        // It acts like a camera screen that captures the scene.
        // Each point on the viewport corresponds to a pixel in the final image.
        // Check "src/how_it_works.png"
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width) / image_height);
        // We don't just use aspect_ratio when computing viewport_width,
        // it's because the value set to aspect_ratio is the ideal ratio,
        // it may not be the actual ratio between image_width and image_height.
        // If image_height was allowed to be real valued—rather than just an integer—then it would be fine to use aspect_ratio.

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left_corner_vector = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel_upper_left_center = viewport_upper_left_corner_vector + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray &r, const hittable &world) const
    {
        hit_record rec;

        // If ray hits sphere
        if (world.hit(r, interval(0, infinity), rec))
        {
            // It calculates and returns the shaded color based on the surface normal
            auto whiteColor = color(1, 1, 1);
            // "rec.normal" has values between [−1, 1]
            // We add (1, 1, 1) = whiteColor to make it positive
            // Now "rec.normal" + whiteColor has values between [0, 2]
            // Multiplying it by 0.5 scales it back into [0, 1] acceptable range
            return 0.5 * (rec.normal + whiteColor);
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

public:
    camera(const double aspect_ratio, const int width) : aspect_ratio(aspect_ratio), image_width(width) {}

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
                auto pixel_center = pixel_upper_left_center + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - camera_center;
                ray r(camera_center, ray_direction);

                color pixel_color = ray_color(r, world);
                write_color(std::cout, pixel_color);
            }
        }

        std::clog << "\rDone                  \n";
    }
};

#endif