#include <iostream>

int main() {
    // P3 is a plain text format for Portable Pixmap (PPM) image files.
    // It is one of the simplest image formats, where pixel data is represented in ASCII text.
    // In P3, each pixel is defined by three integers corresponding to the red, green, and blue color channels.
    // The first line of the output is "P3", identifying the file format.
    // The second line contains the width and height of the image.
    // The third line specifies the maximum color value (typically 255, representing the maximum intensity for each color channel).
    // Each subsequent line contains three integers (r, g, b) for each pixel's color in the image.

    // Generating a sample image
    int image_width = 2560;
    int image_height = 256;
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::clog << "\rDone\n";
}
