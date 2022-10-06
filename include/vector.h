#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include "maths.h"

using std::sqrt;

using Vector3 = class Vector3;

class Vector2 {
public:
    double x;
    double y;
public:
    Vector2() : x(0.0), y(0.0) {}
    Vector2(double _x, double _y) : x(_x), y(_y) {}
    Vector2(const Vector3& v);

    Vector2 operator-() const {return Vector2(-x, -y);}
    double operator[](int i) const {
        switch (i) {
            case 0: return x;
            case 1: return y;
            default:
                std::cerr << "Index beyond the scope." << std::endl;
                exit(1);
        }
    }
    double &operator[](int i) {
        switch (i) {
            case 0: return x;
            case 1: return y;
            default:
                std::cerr << "Index beyond the scope." << std::endl;
                exit(1);
        }
    }

    Vector2 &operator+=(const Vector2 &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2 &operator*=(const double t) {
        x *= t;
        y *= t;
        return *this;
    }

    Vector2 &operator/=(const double t) {
        return *this *= 1/t;
    }

    double length_squared() const {
        return x*x + y*y;
    }

    double length() const {
        return sqrt(length_squared());
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s);
    }
};

using Point2 = Vector2;

inline std::ostream &operator<<(std::ostream &out, const Vector2 &v) {
    return out << v.x << " " << v.y;
}

inline Vector2 operator+(const Vector2 &u, const Vector2 &v) {
    return Vector2(u.x + v.x, u.y + v.y);
}

inline Vector2 operator-(const Vector2 &u, const Vector2 &v) {
    return Vector2(u.x - v.x, u.y - v.y);
}

inline Vector2 operator*(const Vector2 &u, const Vector2 &v) {
    return Vector2(u.x * v.x, u.y * v.y);
}

inline Vector2 operator*(double t, const Vector2 &v) {
    return Vector2(t * v.x, t * v.y);
}

inline Vector2 operator*(const Vector2 &v, double t) {
    return t * v;
}

inline Vector2 operator/(Vector2 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vector2 &u, const Vector2 &v) {
    return u.x * v.x + u.y * v.y;
}

inline double cross(const Vector2 &u, const Vector2 &v) {
    return u.x * v.y - u.y * v.x;
}

inline Vector2 unit_vector(Vector2 v) {
    return v / v.length();
}

using Vector4 = class Vector4;

class Vector3 {
public:
    double x;
    double y;
    double z;
public:
    Vector3() : x(0.0), y(0.0), z(0.0) {}
    Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector4& v);

    Vector3 operator-() const {return Vector3(-x, -y, -z);}
    double operator[](int i) const {
        switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                std::cerr << "Index beyond the scope." << std::endl;
                exit(1);
        }
    }
    double &operator[](int i) {
        switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                std::cerr << "Index beyond the scope." << std::endl;
                exit(1);
        }
    }

    Vector3 &operator+=(const Vector3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3 &operator-=(const Vector3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3 &operator*=(const double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vector3 &operator/=(const double t) {
        return *this *= 1/t;
    }

    double length_squared() const {
        return x*x + y*y + z*z;
    }

    double length() const {
        return sqrt(length_squared());
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }
};

using Point3 = Vector3;

inline std::ostream &operator<<(std::ostream &out, const Vector3 &v) {
    return out << v.x << " " << v.y << " " << v.z;
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vector3 operator*(double t, const Vector3 &v) {
    return Vector3(t * v.x, t * v.y, t * v.z);
}

inline Vector3 operator*(const Vector3 &v, double t) {
    return t * v;
}

inline Vector3 operator/(Vector3 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vector3 &u, const Vector3 &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline Vector3 normalize(Vector3 v) {
    return v / v.length();
}

inline static Vector3 random() {
    return Vector3(random_double(), random_double(), random_double());
}

inline static Vector3 random(double min, double max)  {
    return Vector3(random_double(min, max), random_double(min, max),
        random_double(min, max));
}

Vector3 random_in_unit_sphere();
Vector3 random_unit_vector();
Vector3 random_in_hemisphere(const Vector3& normal);
Vector3 random_cosine_direction();
Vector3 random_in_unit_disk();
Vector3 reflect(const Vector3& v, const Vector3& n);
Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat);

class Vector4 {
public:
    double x;
    double y;
    double z;
    double w;
public:
    Vector4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    Vector4(double _x, double _y, double _z, double _w) :
        x(_x), y(_y), z(_z), w(_w) {}
    Vector4(const Vector3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = 1.0;
    }

    double operator[](int i) const {
        switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default:
                std::cerr << "Index beyond the scope." << std::endl;
                exit(1);
        }
    }
    double &operator[](int i) {
        switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default:
                std::cerr << "Index beyond the scope." << std::endl;
                exit(1);
        }
    }
};

using Color = Vector4;

inline std::ostream& operator<<(std::ostream& out, const Vector4& v) {
    return out << v.x << " " << v.y << " " << v.z << " " << v.z;
}

inline Vector4 homodiv(const Vector4& v) {
    return {v.x/v.w, v.y/v.w, v.z/v.w, 1.0};
}

#endif