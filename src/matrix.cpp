#include <algorithm>
#include "matrix.h"

void Matrix4::initAssignValue(double value) {
    if (index >= 16) {
        std::cerr << "Too much elements for initializing." << std::endl;
        exit(1);
    }
    e[index/4][index%4] = value;
    index++;
}

Matrix4 Matrix4::identity() {
    Matrix4 I;
    for (int i = 0; i < 4; i++) {
        I.e[i][i] = 1.0;
    }
    return I;
}

Matrix4& operator , (Matrix4& m, double x) {
    m.initAssignValue(x);
    return m;
}

Matrix4& operator << (Matrix4& m, double x) {
    m.index = 0;
    m.initAssignValue(x);
    return m;
}

std::ostream& operator << (std::ostream& os, const Matrix4& m) {
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            os << m.e[i][j];
            if (k < 15) os << ",";
            if (j < 3) os << ' ';
            k++;
        }
        os << '\n';
    }
    return os;
}

Matrix4 operator * (const Matrix4& a, const Matrix4& b) {
    Matrix4 m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                m.e[i][j] += a.e[i][k] * b.e[k][j];
            }
        }
    }
    return m;
}

Vector4 operator * (const Matrix4& a, const Vector4& b) {
    Vector4 v;
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            v[i] += a.e[i][k] * b[k];
        }
    }
    return v;
}