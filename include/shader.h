#ifndef SHADER_H
#define SHADER_H

#include "matrix.h"
#include "image.h"
#include "platform.h"

class VertexShader {
public:
    Matrix4 model;
    Matrix4 view;
    Matrix4 projection;
    Matrix4 mvp;
public:
    VertexShader() = default;
    VertexShader(const Matrix4& _model,
                 const Matrix4& _view,
                 const Matrix4& _projection) :
                 model(_model),
                 view(_view),
                 projection(_projection) {
        mvp = projection * view * model;
    }
};

class FragmentShader {

};

#endif