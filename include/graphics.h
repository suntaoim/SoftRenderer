#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <algorithm>
#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "camera.h"
#include "light.h"

using std::min;
using std::max;

struct BoundingBox {
public:
    Point2 minimum;
    Point2 maximum;
public:
    BoundingBox() = default;
    BoundingBox(const Point2& a, const Point2& b) : minimum(a), maximum(b) {}
};

BoundingBox getBoundingBox(const Vector3 triangle[], int width, int height);

typedef struct program program_t;
typedef Vector4 vertex_shader_t(void *attribs, void *varyings, void *uniforms);
typedef Vector4 fragment_shader_t(void *varyings, void *uniforms,
                                 int *discard, int backface);

// /* program management */
// program_t *program_create(
//     vertex_shader_t *vertex_shader, fragment_shader_t *fragment_shader,
//     int sizeof_attribs, int sizeof_varyings, int sizeof_uniforms,
//     int double_sided, int enable_blend);
// void program_release(program_t *program);
// void *program_get_attribs(program_t *program, int nth_vertex);
// void *program_get_uniforms(program_t *program);

// /* graphics pipeline */
// void graphics_draw_triangle(Framebuffer *framebuffer, program_t *program);

Vector3 getBarycentricCoord(Vector3 triangle[], const Vector2& p);
Matrix4 getViewMatrix(const Camera& camera);
Matrix4 getOrthographicMatrix(double fovY, double aspectRatio, double near,
    double far);
Matrix4 getPerspectiveMatrix(double fovY, double aspectRatio, double near,
    double far);
Matrix4 getViewportMatrix(int w, int h);
void rasterizeTriangle(Vector3 positions[], Vector2 texcoords[], Image* texture,
const Light& light, Framebuffer* framebuffer);

#endif