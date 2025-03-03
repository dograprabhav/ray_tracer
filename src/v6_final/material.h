#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
  public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
public:
    lambertian(const color& albedo) : albedo(albedo) {}
  
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        // attenuation: How much light the material absorbs or reflects
        attenuation = albedo;
        return true;
      }
  
    private:
      color albedo;
};

class metal : public material {
public:
    // `albedo` defines the material's base color.
    // `fuzz` controls how blurry the reflections are; it's clamped to 1 to avoid extreme fuzziness.
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        // Compute the reflection vector based on the incident ray and the surface normal
        vec3 reflected = reflect(r_in.direction(), rec.normal);

        // Apply fuzziness by adding a small random perturbation to the reflection direction
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        
        // Create the scattered ray starting from the hit point, moving in the reflected direction
        scattered = ray(rec.p, reflected);

        // The attenuation (color absorption) is determined by the material's albedo
        attenuation = albedo;
        
        // The function returns true only if the scattered ray is still in the valid hemisphere  
        // (i.e., it has a positive dot product with the normal)
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
    public:
      dielectric(double refraction_index) : refraction_index(refraction_index) {}
  
      bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
      const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction());

            // When light hits a surface, some of it reflects and some refracts (passes through). The amount of light that reflects depends on:
            // - Incident angle (𝜃): Light hitting at a steeper angle reflects more.
            // - Material properties: Different materials have different refractive indices (η), which influence how much light reflects.

            // Finds critical angle to ensure if reflection happens or refraction
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            // Use Schlick's approximation for reflectance.
            // reflectance(cos_theta, ri) > random_double()
            // This takes into account the material properties and the incident angle to determine whether the ray reflects or refracts.
            if (cannot_refract || reflectance(cos_theta, ri) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, ri);

            scattered = ray(rec.p, direction);

            return true;
      }
  
    private:
      // Refractive index in vacuum or air, or the ratio of the material's refractive index over
      // the refractive index of the enclosing media
      double refraction_index;
      
      static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }

  };
  

#endif
