#ifndef OBJECT_H
#define OBJECT_H

#include "mesh.h"
#include "image.h"

class Object {
public:
    Mesh mesh;
    Image texture;
public:
    Object() = default;
    Object(const Mesh& _mesh, const Image& _texture) :
        mesh(_mesh), texture(_texture) {}
    Object(const char* meshPath, const char* texturePath) :
        mesh(meshPath), texture(texturePath) {}
};

#endif