#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "matrix.h"

struct BoundingBox {
public:
    Point2 minimum;
    Point2 maximum;
public:
    BoundingBox() = default;
    BoundingBox(const Point2& a, const Point2& b) : minimum(a), maximum(b) {}
};

BoundingBox getBoundingBox(const Vector2 triangle[], int width, int height);
Vector3 getBarycentricCoord(Vector2 triangle[], const Vector2& p);
Matrix4 getViewMatrix(const Vector3& position, const Vector3& front,
    const Vector3& worldup);
Matrix4 getOrthographicMatrix(double right, double top, double zNear,
    double zFar);
Matrix4 getPerspectiveMatrix(double fovY, double aspectRatio, double near,
    double far);
Vector4 viewportTransform(int screenWidth, int screenHeight,
    const Vector4& ndcCoord);

#endif