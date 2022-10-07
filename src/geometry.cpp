#include "geometry.h"
#include "maths.h"

BoundingBox getBoundingBox(const Vector2 triangle[], int width, int height) {
    double xmin = min(min(triangle[0].x, triangle[1].x), triangle[2].x);
    double ymin = min(min(triangle[0].y, triangle[1].y), triangle[2].y);
    double xmax = max(max(triangle[0].x, triangle[1].x), triangle[2].x);
    double ymax = max(max(triangle[0].y, triangle[1].y), triangle[2].y);

    BoundingBox box;
    box.minimum.x = clamp(static_cast<int>(floor(xmin)), 0, width - 1);
    box.minimum.y = clamp(static_cast<int>(floor(ymin)), 0, height - 1);
    box.maximum.x = clamp(static_cast<int>(ceil(xmax)), 0, width - 1);
    box.maximum.y = clamp(static_cast<int>(ceil(ymax)), 0, height - 1);

    return box;
}

Vector3 getBarycentricCoord(Vector2 triangle[], const Vector2& p) {
    Vector3 coord = cross(Vector3(triangle[1].x - triangle[0].x,
                                  triangle[2].x - triangle[0].x,
                                  triangle[0].x - p.x),
                          Vector3(triangle[1].y - triangle[0].y,
                                  triangle[2].y - triangle[0].y,
                                  triangle[0].y - p.y));
    double u = coord.x / coord.z;
    double v = coord.y / coord.z;
    return Vector3(1.0 - u - v, u, v);
}

/**
 * @brief Get the View Matrix object
 * translate = 1.0, 0.0, 0.0, -position[0],
 *             0.0, 1.0, 0.0, -position[1],
 *             0.0, 0.0, 1.0, -position[2],
 *             0.0, 0.0, 0.0, 1.0
 * rotate = x[0], x[1], x[2], 0.0,
 *          y[0], y[1], y[2], 0.0,
 *          z[0], z[1], z[2], 0.0,
 *          0.0, 0.0, 0.0, 1.0
 * view = rotate * translate
 * @param position 
 * @param front 
 * @param worldup 
 * @return Matrix4 
 */
Matrix4 getViewMatrix(const Vector3& position, const Vector3& front,
const Vector3& worldup) {
    Vector3 axisZ = -normalize(front);
    Vector3 axisX = normalize(cross(worldup, axisZ));
    Vector3 axisY = normalize(cross(axisZ, axisX));

    Matrix4 view;
    view << axisX.x, axisX.y, axisX.z, -dot(axisX, position),
            axisY.x, axisY.y, axisY.z, -dot(axisY, position),
            axisZ.x, axisZ.y, axisZ.z, -dot(axisZ, position),
            0.0, 0.0, 0.0, 1.0;

    return view;
}

/**
 * @brief Get the Orthographic Matrix object
 * translate = 1.0, 0.0, 0.0, -(left + right)/2.0,
 *             0.0, 1.0, 0.0, -(bottom + top)/2.0,
 *             0.0, 0.0, 1.0, -(zNear + zFar)/2.0,
 *             0.0, 0.0, 0.0, 1.0
 * scale = 2.0/(right - left), 0.0, 0.0, 0.0,
 *         0.0, 2.0/(top - bottom), 0.0, 0.0,
 *         0.0, 0.0, 2.0/(zNear - zFar), 0.0,
 *         0.0, 0.0, 0.0, 1.0
 * ortho = scale * translate
 * @param right 
 * @param top 
 * @param zNear 
 * @param zFar 
 * @return Matrix4 
 */
Matrix4 getOrthographicMatrix(double right, double top, double zNear, double zFar) {
    double zRange = zNear - zFar;

    Matrix4 ortho;
    ortho << 1.0/right, 0.0, 0.0, 0.0,
             0.0, 1.0/top, 0.0, 0.0,
             0.0, 0.0, 2/zRange, -(zNear + zFar)/zRange,
             0.0, 0.0, 0.0, 1.0;
    return ortho;
}

/**
 * @brief Get the Perspective Matrix object
 * persp2ortho = zNear, 0.0, 0.0, 0.0,
 *               0.0, zNear, 0.0, 0.0,
 *               0.0, 0.0, zNear + zFar, -zNear * zFar,
 *               0.0, 0.0, 1.0, 0.0
 * persp = ortho * persp2ortho
 * @param fovy 
 * @param aspect 
 * @param zNear 
 * @param zFar 
 * @return Matrix4 
 */
Matrix4 getPerspectiveMatrix(double fovy, double aspect, double zNear, double zFar) {
    double top = tan(radians(fovy/2.0)) * -zNear;
    double right = aspect * top;
    double zRange = zNear - zFar;

    Matrix4 persp;
    persp << zNear/right, 0.0, 0.0, 0.0,
             0.0, zNear/top, 0.0, 0.0,
             0.0, 0.0, (zNear + zFar)/zRange, -(2.0 * zNear * zFar)/zRange,
             0.0, 0.0, 1.0, 0.0;
    return persp;
}

Vector4 viewportTransform(int screenWidth, int screenHeight,
                          const Vector4& ndcCoord) {
    float x = (ndcCoord.x + 1.0f) * 0.5f * static_cast<float>(screenWidth);
    float y = (ndcCoord.y + 1.0f) * 0.5f * static_cast<float>(screenHeight);
    float z = (ndcCoord.z + 1.0f) * 0.5f;
    return Vector3(x, y, z);
}