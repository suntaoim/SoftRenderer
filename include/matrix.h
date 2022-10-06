#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

class Matrix4 {
public:
    double e[4][4] = {0.0};
private:
    int index;
    void initAssignValue(double value);
public:
    Matrix4() = default;
    Matrix4(const Matrix4& m);
    Matrix4(int i);
    static Matrix4 identity();
    static Matrix4 identity(double n);

    // Fill matrix by operator <<
    friend Matrix4& operator , (Matrix4& m, double x);
    friend Matrix4& operator << (Matrix4& m, double x);
};

std::ostream& operator << (std::ostream& os, const Matrix4& m);
Matrix4 operator * (const Matrix4& a, const Matrix4& b);
Vector4 operator * (const Matrix4& a, const Vector4& b);

#endif