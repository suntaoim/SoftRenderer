#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <algorithm>
#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "camera.h"
#include "object.h"
#include "program.h"

class Primitive {
public:
    /* Origin data */
    Vector3 positions[3];
    Vector2 texcoords[3];
    Vector3 normals[3];
    /* For clipping */
    Vector4 clipCoords[3];
public:
    Primitive() = default;
};

typedef struct program program_t;
typedef Vector4 vertex_shader_t(void *attribs, void *varyings, void *uniforms);
typedef Vector4 fragment_shader_t(void *varyings, void *uniforms,
                                 int *discard, int backface);

bool isBackFace(Vector3 coords[]);
void drawObject(const Object& object, const Program& program,
    Framebuffer* framebuffer);
void drawTriangle(Framebuffer* framebuffer, Primitive& triangle,
    const Image& texture, const Program& program);
void rasterizeTriangle(Framebuffer* framebuffer, const Primitive& triangle,
    const Image& texture, const Program& program);

#endif