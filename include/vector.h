#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include "math.h"

using std::sqrt;

class Vector3 {
public:
    double e[3];
public:
    Vector3() : e{0.0, 0.0, 0.0} {}
    Vector3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    // Vector3(const Vector4& v) {
    //     for (int i = 0; i < 3; i++) {
    //         e[i] = v[i] / v[3];
    //     }
    // }

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    Vector3 operator-() const {return Vector3(-e[0], -e[1], -e[2]);}
    double operator[](int i) const {return e[i];}
    double &operator[](int i) {return e[i];}

    Vector3 &operator+=(const Vector3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3 &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3 &operator/=(const double t) {
        return *this *= 1/t;
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    double length() const {
        return sqrt(length_squared());
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};

// Type aliases for Vector3
using point3 = Vector3;    // 3D point
// using color = Vector3;     // RGB color
// typedef Vector3 point3;
// typedef Vector3 color;

// Vector3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vector3 &v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3 &v) {
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, double t) {
    return t * v;
}

inline Vector3 operator/(Vector3 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vector3 &u, const Vector3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(Vector3 v) {
    return v / v.length();
}

inline static Vector3 random() {
    return Vector3(random_double(), random_double(), random_double());
}

inline static Vector3 random(double min, double max)  {
    return Vector3(random_double(min, max), random_double(min, max),
        random_double(min, max));
}

// Vector3 random_in_unit_sphere() {
//     while (true) {
//         auto p = random(-1, 1);
//         if (p.length_squared() >= 1) continue;
//         return p;
//     }
// }

// Vector3 random_unit_vector() {
//     return unit_vector(random_in_unit_sphere());
// }

// Vector3 random_in_hemisphere(const Vector3& normal) {
//     Vector3 in_unit_sphere = random_in_unit_sphere();
//     double cosine = dot(in_unit_sphere, normal);
//     if (cosine > 0.0)       // In the same hemisphere as the normal
//         return in_unit_sphere;
//     else if (cosine < 0.0)  // In the opposite hemisphere of the normal
//         return -in_unit_sphere;
//     else return random_in_hemisphere(normal);
// }

// Vector3 random_cosine_direction() {
//     double r1 = random_double();
//     double r2 = random_double();

//     auto phi = 2*pi*r1;
//     double x = cos(phi) * sqrt(r2);
//     double y = sin(phi) * sqrt(r2);
//     double z = sqrt(1-r2);

//     return Vector3(x, y, z);
// }

// Vector3 random_in_unit_disk() {
//     while (true) {
//         auto p = Vector3(random_double(-1, 1), random_double(-1, 1), 0);
//         if (p.length_squared() >= 1) continue;
//         return p;
//     }
// }

// Vector3 reflect(const Vector3& v, const Vector3& n) {
//     return v - 2 * dot(v, n) * n;
// }

// Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
//     auto cos_theta = fmin(dot(-uv, n), 1.0);
//     Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
//     Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
//     return r_out_perp + r_out_parallel;
// }

class Vector4 {
public:
    double e[4];
public:
    Vector4() : e{0.0, 0.0, 0.0, 0.0} {}
    Vector4(double e0, double e1, double e2, double e3) : e{e0, e1, e2, e3} {}
    Vector4(const Vector3& v) {
        e[0] = v[0];
        e[1] = v[1];
        e[2] = v[2];
        e[3] = 1.0;
    }

    double operator [] (int i) const {return e[i];}
    double &operator[] (int i) {return e[i];}

    friend std::ostream& operator << (std::ostream& out, const Vector4& v);
};

inline std::ostream& operator<<(std::ostream& out, const Vector4& v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2] << " " << v.e[3];
}

inline Vector4 homodiv(const Vector4& v) {
    return {v[0]/v[3], v[1]/v[3], v[2]/v[3], 1.0};
}

#endif