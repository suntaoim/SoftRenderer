#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"
#include "light.h"

class Program {
public:
    VertexShader* vertexShader;
    FragmentShader* fragmentShader;
    DirectionalLight* light;
public:
    Program() = default;
    Program(VertexShader* _vertexShader,
            Matrix4 _viewport,
            FragmentShader* _fragmentShader,
            DirectionalLight* _light) :
            vertexShader(_vertexShader),
            fragmentShader(_fragmentShader),
            light(_light) {}
};

#endif