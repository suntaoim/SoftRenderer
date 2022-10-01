#ifndef SHADER_H
#define SHADER_H

#include "matrix.h"
#include "light.h"
#include "image.h"
#include "platform.h"

class Shader {
public:
    Matrix4 model;
    Matrix4 view;
    Matrix4 projection;
    Matrix4 mvp;
    Matrix4 viewport;
    Light light;
    Image* texture;
    Framebuffer* framebuffer;
public:
    Shader() = default;
    Shader(const Matrix4& _model, const Matrix4& _view,
           const Matrix4& _projection, const Matrix4& _viewport,
           const Light& _light, Image* _texture, Framebuffer* _framebuffer) :
           model(_model),
           view(_view),
           projection(_projection),
           viewport(_viewport),
           light(_light),
           texture(_texture),
           framebuffer(_framebuffer) {
        mvp = projection * view * model;
    }
};

#endif