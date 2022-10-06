#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "matrix.h"
#include "geometry.h"
#include "platform.h"
#include "shader.h"

class Light {
public:
    Vector4 color;
    // For shadow mapping
public:
    Light() : color(1.0, 1.0, 1.0, 1.0) {}
    Light(const Vector4& _color) : color(_color) {}
};

class DirectionalLight : public Light {
public:
    Vector3 direction;
    bool enableShdow;
    VertexShader* vertexShader;
    Framebuffer* shadowMap;
public:
    // Todo: change direction with input
    DirectionalLight(bool _enableShdow = false,
                     const Vector4& _color = Vector4(1.0, 1.0, 1.0, 1.0),
                     const Vector3& _direction = normalize(Vector3(0.0, -1.0, -1.0))) :
                     enableShdow(_enableShdow),
                     Light(_color),
                     direction(_direction) {
        if (enableShdow) {
            Matrix4 model = Matrix4::identity();
            Matrix4 view = getViewMatrix(-direction, direction,
                Vector3(0.0, 1.0, 0.0));
            Matrix4 projection = getOrthographicMatrix(1, 1, 0, -2);
            // Todo: send in WINODW_HEIGHT and WINDOW_WIDTH
            vertexShader = new VertexShader(model, view, projection);
            shadowMap = new Framebuffer(800, 800);
        }
    }
};

class PointLight : public Light {

};

# endif