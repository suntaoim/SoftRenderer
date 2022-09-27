#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "matrix.h"

Matrix4 getViewMatrix(const Vector3& center, const Vector3& camera,
const Vector3& up) {
    Vector3 gaze, right;

    Vector3 z = unit_vector(camera - center);
    Vector3 x = unit_vector(cross(up, z));
    Vector3 y = unit_vector(cross(z, x));

    Matrix4 translate, rotate;
    translate << 1.0, 0.0, 0.0, -camera[0],
                 0.0, 1.0, 0.0, -camera[1],
                 0.0, 0.0, 1.0, -camera[2],
                 0.0, 0.0, 0.0, 1.0;
    rotate << x[0], x[1], x[2], 0.0,
              y[0], y[1], y[2], 0.0,
              z[0], z[1], z[2], 0.0,
              0.0, 0.0, 0.0, 1.0;

    return rotate * translate;
}

Matrix4 getOrthogonalProjectionMatrix(double left, double right, double bottom,
double top, double near, double far) {
    Matrix4 translate, scale;
    translate << 1.0, 0.0, 0.0, -(left + right)/2.0,
                 0.0, 1.0, 0.0, -(bottom + far)/2.0,
                 0.0, 0.0, 1.0, -(near + far)/2.0,
                 0.0, 0.0, 0.0, 1.0;
    scale << 2.0/(right - left), 0.0, 0.0, 0.0,
             0.0, 2.0/(top - bottom), 0.0, 0.0,
             0.0, 0.0, 2.0/(near - far), 0.0,
             0.0, 0.0, 0.0, 1.0;
    return scale * translate;
}

Matrix4 getPerspectiveProjectionMatrix(double left, double right, double bottom,
double top, double near, double far) {
    Matrix4 perspective2orthogonal;
    perspective2orthogonal << near, 0.0, 0.0, 0.0,
                              0.0, near, 0.0, 0.0,
                              0.0, 0.0, near + far, -near * far,
                              0.0, 0.0, 1.0, 0.0;
    return getOrthogonalProjectionMatrix(left, right, bottom, top, near, far) *
        perspective2orthogonal;
}

Matrix4 getViewportMatrix(double x, double y, int w, int h) {
    Matrix4 viewport;
    viewport << (double)w/2.0, 0.0, 0.0, (double)w/2.0,
                0.0, (double)h/2.0, 0.0, (double)h/2.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0;
    return viewport;
}

#endif