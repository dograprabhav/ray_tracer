#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

bool hit_sphere(const point3 &center, double radius, const ray &r)
{
    // Equation of a sphere with center at (0 0 0):
    // => x² + y² + z² = r²
    // Equation of a sphere with center at (Cₓ Cᵧ C𝓏):
    // => (Cₓ - x)² + (Cᵧ - y)² + (C𝓏 - z)² = r²
    // A vector joining a point A(x₁ y₁ z₁) and B(x₂ y₂ z₂) is denoted by:
    // (𝐁 - 𝐀) = (x₂ - x₁, y₂ - y₁, z₂ - z₁)
    // Distance between a point A(x₁ y₁ z₁) and B(x₂ y₂ z₂) is denoted by:
    // d = √[(x₂ - x₁)² + (y₂ - y₁)² + (z₂ - z₁)²]
    // Similarly, a vector from a point P(x y z) to center C(𝐂ₓ 𝐂ᵧ 𝐂𝓏) is (𝐂 - 𝐏)
    // => (𝐂 - 𝐏) = V((𝐂ₓ - x), (𝐂ᵧ - y) (𝐂𝓏 - z))
    // For the point P to lie on the sphere it should be r (radius) distance away from center:
    // => (𝐂 - 𝐏) ⋅ (𝐂 - 𝐏) = (𝐂ₓ - x)² + (𝐂ᵧ - y)² + (𝐂𝓏 - z)²
    // and
    // => (𝐂ₓ - x)² + (𝐂ᵧ - y)² + (𝐂𝓏 - z)² = r² -> Distance from center
    // Therefore, for a point to lie
    // => (𝐂 - 𝐏) ⋅ (𝐂 - 𝐏) = r²

    // Equation of a general ray is RAY(t) = M * t + N [Equation of any straight line]
    // M represents the direction of ray
    // N represent the point of origin
    // A ray hits the sphere when it's 'r' distance away from the center
    // Hence, (𝐂 - RAY(t)) ⋅ (𝐂 - RAY(t)) = r²
    // => (𝐂 - (M * t + N)) ⋅ (𝐂 - (M * t + N)) = r²
    // => t² * M ⋅ M - 2 * t * M ⋅ (𝐂 - N) + (𝐂 - N) ⋅ (𝐂 - N) - r² = 0
    // Using the quadratic formula,
    // roots = -b ± √(b² - 4ac) / 2a
    // We get,
    // 𝑎 = M ⋅ M
    // 𝑏 = −2 M ⋅ (𝐂 − N)
    // c = (𝐂 − N) ⋅ (𝐂 − N) - r²
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) { // Point of intersection exists between the sphere and the ray
        return true;
    }
    return false;
}

color ray_color(const ray &r)
{
    // If ray hits sphere return red color
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);

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

int main()
{
    // Generating a sample image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    if (image_height < 1) {
        image_height = 1;
    }

    // Defining the camera center: For simpler calculations lets set the center at (0, 0, 0)
    // A point in 3D space from which all scene rays will originate (this is also commonly referred to as the eye point).
    // The vector from the camera center to the viewport center will be orthogonal (perpendicular) to the viewport.
    // We'll initially set the distance between the viewport and the camera center point to be one unit.
    // This distance is often referred to as the focal length.
    // Check "src/v2/how_it_works.png"
    auto focal_length = 1.0;
    auto camera_center = point3(0, 0, 0);
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
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left_corner_vector = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel_upper_left_center = viewport_upper_left_corner_vector + 0.5 * (pixel_delta_u + pixel_delta_v);

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
        std::clog << "\rLines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_center = pixel_upper_left_center + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            // Make a light ray using a point of origin and direction
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone                    \n";
}
