#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <iostream>
#include "vector.h"
#include "shader.h"

struct Vertex {
    Vector3 position;
    Vector2 texcoord;
    Vector3 normal;
};

class Mesh {
public:
    int facesNum;
    std::vector<Vertex> vertices;
public:
    Mesh(const std::string& filepath);
    // const Vector3& position(unsigned int index);
    // const Vector2& texcoord(unsigned int index);
    // const Vector3& normal(unsigned int index);
    // const std::vector<unsigned int>& facePositions(unsigned int index);
    // const std::vector<unsigned int>& faceTexcoords(unsigned int index);
    // const std::vector<unsigned int>& faceNormals(unsigned int index);
};

#endif