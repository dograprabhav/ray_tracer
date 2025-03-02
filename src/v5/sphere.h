#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
    sphere(const point3 &center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        /*
            Equation of a sphere with center at (0 0 0):
            => x² + y² + z² = r²
            Equation of a sphere with center at (Cₓ Cᵧ C𝓏):
            => (Cₓ - x)² + (Cᵧ - y)² + (C𝓏 - z)² = r²
            A vector joining a point A(x₁ y₁ z₁) and B(x₂ y₂ z₂) is denoted by:
            (𝐁 - 𝐀) = (x₂ - x₁, y₂ - y₁, z₂ - z₁)
            Distance between a point A(x₁ y₁ z₁) and B(x₂ y₂ z₂) is denoted by:
            d = √[(x₂ - x₁)² + (y₂ - y₁)² + (z₂ - z₁)²]
            Similarly, a vector from a point P(x y z) to center C(𝐂ₓ 𝐂ᵧ 𝐂𝓏) is (𝐂 - 𝐏)
            => (𝐂 - 𝐏) = V((𝐂ₓ - x), (𝐂ᵧ - y) (𝐂𝓏 - z))
            For the point P to lie on the sphere it should be r (radius) distance away from center:
            => (𝐂 - 𝐏) ⋅ (𝐂 - 𝐏) = (𝐂ₓ - x)² + (𝐂ᵧ - y)² + (𝐂𝓏 - z)²
            and
            => (𝐂ₓ - x)² + (𝐂ᵧ - y)² + (𝐂𝓏 - z)² = r² -> Distance from center
            Therefore, for a point to lie
            => (𝐂 - 𝐏) ⋅ (𝐂 - 𝐏) = r²

            Equation of a general ray is RAY(t) = M * t + N [Equation of any straight line]
            M represents the direction of ray
            N represent the point of origin
            A ray hits the sphere when it's 'r' distance away from the center
            Hence, (𝐂 - RAY(t)) ⋅ (𝐂 - RAY(t)) = r²
            => (𝐂 - (M * t + N)) ⋅ (𝐂 - (M * t + N)) = r²
            => t² * M ⋅ M - 2 * t * M ⋅ (𝐂 - N) + (𝐂 - N) ⋅ (𝐂 - N) - r² = 0
            Using the quadratic formula,
            roots = -b ± √(b² - 4ac) / 2a
            We get,
            𝑎 = M ⋅ M
            𝑏 = −2 M ⋅ (𝐂 − N)
            c = (𝐂 − N) ⋅ (𝐂 − N) - r²
            Simplifying,
            𝑎 = M ⋅ M = [LengthOfVector(V)]² --- (1)
            Let's say, h = M ⋅ (𝐂 − N)       --- (2)
            b = -2 ⋅ h                       --- (3)
            Using the quadratic formula:
            t = (-b ± sqrt(b⋅b - 4⋅a⋅c)) / (2⋅a)
            Substitute b = -2⋅h:
            t = (-(-2⋅h) ± sqrt((-2⋅h)⋅(-2⋅h) - 4⋅a⋅c)) / (2⋅a)
            Simplify:
            t = (2⋅h ± sqrt(4⋅h⋅h - 4⋅a⋅c)) / (2⋅a)
            Factor out 2 from the square root:
            t = (2⋅h ± 2⋅sqrt(h⋅h - a⋅c)) / (2⋅a)
            Cancel the 2's:
            t = (h ± sqrt(h⋅h - a⋅c)) / a    --- (4)
        */
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared(); // Using (1)
        auto h = dot(r.direction(), oc);         // Using (2)
        auto c = oc.length_squared() - radius * radius; // Using (3)

        auto discriminant = h * h - a * c; // Using (4)
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // Sets where along the ray the intersection happened in hit_record
        rec.t = root;
        // Sets where the actual 3D position of that intersection
        rec.p = r.at(rec.t);
        // Sets unit vector perpendicular to the spheres surface where intersection happened
        rec.normal = (rec.p - center) / radius; 

        return true;
    }

private:
    point3 center;
    double radius;
};

#endif
