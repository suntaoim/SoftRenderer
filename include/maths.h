#ifndef MATH_H
#define MATH_H

#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returen a random real in [0,1)
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// Returen a random in [min, max)
inline double random_double(double min, double max) {
    return min + random_double() * (max - min);
}

// Return a random integer in [min,max]
inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline unsigned char float_to_uchar(double value) {
    return static_cast<unsigned char>(value * 255);
}

#endif