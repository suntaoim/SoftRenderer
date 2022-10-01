#include <cmath>
#include <iostream>
#include "maths.h"
#include "vector.h"

using std::sqrt;

// Vector3 Utility Functions
// -------------------------

Vector3::Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z) {}

Vector3 random_in_unit_sphere() {
    while (true) {
        auto p = random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vector3 random_unit_vector() {
    return normalize(random_in_unit_sphere());
}

Vector3 random_in_hemisphere(const Vector3& normal) {
    Vector3 in_unit_sphere = random_in_unit_sphere();
    double cosine = dot(in_unit_sphere, normal);
    if (cosine > 0.0)       // In the same hemisphere as the normal
        return in_unit_sphere;
    else if (cosine < 0.0)  // In the opposite hemisphere of the normal
        return -in_unit_sphere;
    else return random_in_hemisphere(normal);
}

Vector3 random_cosine_direction() {
    double r1 = random_double();
    double r2 = random_double();

    auto phi = 2*pi*r1;
    double x = cos(phi) * sqrt(r2);
    double y = sin(phi) * sqrt(r2);
    double z = sqrt(1-r2);

    return Vector3(x, y, z);
}

Vector3 random_in_unit_disk() {
    while (true) {
        auto p = Vector3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - 2 * dot(v, n) * n;
}

Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

// Vector4 Utility Functions
// -------------------------
