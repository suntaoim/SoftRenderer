#include <assert.h>
#include "graphics.h"
#include "maths.h"
#include "geometry.h"

Framebuffer::Framebuffer(int _width, int _height) {
    assert(_width > 0 && _height > 0);

    int colorBufferSize = _width * _height * 4;
    int depthBufferSize = _width * _height * sizeof(float);
    Vector4 defaultColor = {0, 0, 0, 1};
    float defaultDepth = -10000;

    width = _width;
    height = _height;
    colorBuffer = (unsigned char*)malloc(colorBufferSize);
    depthBuffer = (float*)malloc(depthBufferSize);

    clearColor(defaultColor);
    clearDepth(defaultDepth);
}

Framebuffer::~Framebuffer() {
    free(colorBuffer);
    free(depthBuffer);
}

void Framebuffer::clearColor(const Vector4& color) {
    int num_pixels = width * height;
    int i;
    for (i = 0; i < num_pixels; i++) {
        colorBuffer[i * 4 + 0] = float_to_uchar(color.x);
        colorBuffer[i * 4 + 1] = float_to_uchar(color.y);
        colorBuffer[i * 4 + 2] = float_to_uchar(color.z);
        colorBuffer[i * 4 + 3] = float_to_uchar(color.w);
    }
}

void Framebuffer::clearDepth(float depth) {
    int num_pixels = width * height;
    int i;
    for (i = 0; i < num_pixels; i++) {
        depthBuffer[i] = depth;
    }
}

bool isBackFace(Vector3 coords[]) {
    Vector3 a = coords[0];
    Vector3 b = coords[1];
    Vector3 c = coords[2];
    float z = a.x * b.y - a.y * b.x +
              b.x * c.y - b.y * c.x +
              c.x * a.y - c.y * a.x;
    return z <= 0;
}

void drawObject(const Object& object, const Program& program,
                Framebuffer* framebuffer) {
    for (int i = 0; i < object.mesh.facesNum; i++) {
        Primitive triangle;
        for (int j = 0; j < 3; j++) {
            triangle.positions[j] = object.mesh.vertices[i * 3 + j].position;
            triangle.texcoords[j] = object.mesh.vertices[i * 3 + j].texcoord;
            triangle.normals[j] = object.mesh.vertices[i * 3 + j].normal;
        }
        drawTriangle(framebuffer, triangle, object.texture, program);
    }
}

void drawTriangle(Framebuffer* framebuffer, Primitive& triangle,
                  const Image& texture, const Program& program) {
    Vector4 clipCoords[3];
    for (int i = 0; i < 3; i++) {
        triangle.clipCoords[i] = program.vertexShader->mvp *
            Vector4(triangle.positions[i]);
    }
    rasterizeTriangle(framebuffer, triangle, texture, program);
}

void rasterizeTriangle(Framebuffer* framebuffer, const Primitive& triangle,
                       const Image& texture, const Program& program) {
    Vector3 ndcCoords[3];
    Vector2 texcoords[3];
    for (int i = 0; i < 3; i++) {
        // Calculate NDC coordinates
        ndcCoords[i] = homodiv(triangle.clipCoords[i]);
        texcoords[i] = triangle.texcoords[i];
    }

    /* Backface cullinf */
    if (isBackFace(ndcCoords)) return;

    /* Reciprocals of w */
    float recipW[3];
    for (int i = 0; i < 3; i++) {
        recipW[i] = 1.0 / triangle.clipCoords[i].w;
    }

    /* Viewport mapping */
    Vector2 screenCoords[3];
    float screenDepths[3];
    for (int i = 0; i < 3; i++) {
        Vector3 screenCoord = viewportTransform(framebuffer->width,
            framebuffer->height, ndcCoords[i]);
        screenCoords[i] = screenCoord;
        screenDepths[i] = screenCoord.z;
    }

    /* Rasterize triangle */
    BoundingBox box = getBoundingBox(screenCoords, framebuffer->width,
        framebuffer->height);
    for (int x = box.minimum.x; x <= box.maximum.x; x++) {
        for (int y = box.minimum.y; y <= box.maximum.y; y++) {
            /* Calculate correct barycentric coordinates in perspective space. */
            Vector3 weightScreen = getBarycentricCoord(screenCoords, Vector2(x, y));
            if (weightScreen.x < 0.0 || weightScreen.y < 0.0 || weightScreen.z < 0.0) {
                continue;
            }
            int index = y * framebuffer->width + x;

            /* Depth test */
            float z = weightScreen.x * screenDepths[0] +
                       weightScreen.y * screenDepths[1] +
                       weightScreen.z * screenDepths[2];
            if (z < framebuffer->depthBuffer[index]) {
                continue;
            }

            /* Correct barycentric coordinates */
            Vector3 weightWorld;
            for (int i = 0; i < 3; i++) {
                weightWorld[i] = weightScreen[i] * recipW[i];
            }
            float normalizer = 1.0/(weightWorld[0] + weightWorld[1] + weightWorld[2]);
            for (int i = 0; i < 3; i++) {
                weightWorld[i] *= normalizer;
            }

            /* Interpolate with barycentric coordinates */
            double u = weightWorld.x * texcoords[0].x +
                       weightWorld.y * texcoords[1].x +
                       weightWorld.z * texcoords[2].x;
            double v = weightWorld.x * texcoords[0].y +
                       weightWorld.y * texcoords[1].y +
                       weightWorld.z * texcoords[2].y;

            // Depth test
            if (framebuffer->depthBuffer[index] < z) {
                framebuffer->depthBuffer[index] = z;

                // Here just is the shading of directional light
                // Todo: Abstracts the process of calculating the color of a
                // fragment as an interface
                Vector3 color = texture.sample(u, v) * program.light->color;

                framebuffer->colorBuffer[index * 4 + 0] = float_to_uchar(color.x);
                framebuffer->colorBuffer[index * 4 + 1] = float_to_uchar(color.y);
                framebuffer->colorBuffer[index * 4 + 2] = float_to_uchar(color.z);
            }
        }
    }
}