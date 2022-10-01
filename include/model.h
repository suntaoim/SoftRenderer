#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <iostream>
#include "vector.h"
#include "shader.h"

class Model {
private:
    std::vector<Vector3> positions;
    std::vector<Vector2> texcoords;
    std::vector<Vector3> normals;
    std::vector<std::vector<unsigned int>> facePositionIndexes;
    std::vector<std::vector<unsigned int>> faceTexcoordIndexes;
    std::vector<std::vector<unsigned int>> faceNormalIndexes;
public:
    unsigned int faceNum;
public:
    Model(const std::string& filepath);
    void draw(const Shader& shader);
    // const Vector3& position(unsigned int index);
    // const Vector2& texcoord(unsigned int index);
    // const Vector3& normal(unsigned int index);
    // const std::vector<unsigned int>& facePositions(unsigned int index);
    // const std::vector<unsigned int>& faceTexcoords(unsigned int index);
    // const std::vector<unsigned int>& faceNormals(unsigned int index);
};

// class Model {
// private:
//     std::vector<Vector3> verts_;
//     std::vector<std::vector<int> > faces_;
// public:
//     Model(const char *filename);
//     ~Model();
//     int nverts();
//     int nfaces();
//     Vector3 vert(int i);
//     std::vector<int> face(int idx);
// };

// Model::Model(const char *filename) : verts_(), faces_() {
//     std::ifstream in;
//     in.open(filename, std::ifstream::in);
//     if (in.fail()) return;
//     std::string line;
//     while (!in.eof()) {
//         std::getline(in, line);
//         std::istringstream iss(line.c_str());
//         char trash;
//         if (!line.compare(0, 2, "v ")) {
//             iss >> trash;
//             Vector3 v;
//             for (int i = 0; i < 3; i++) {
//                 iss >> v[i];
//             }
//             verts_.push_back(v);
//         } else if (!line.compare(0, 2, "f ")) {
//             std::vector<int> f;
//             int itrash, idx;
//             iss >> trash;
//             while (iss >> idx >> trash >> itrash >> trash >> itrash) {
//                 idx--; // in wavefront obj all indices start at 1, not zero
//                 f.push_back(idx);
//             }
//             faces_.push_back(f);
//         }
//     }
//     std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
// }

// Model::~Model() {}

// int Model::nverts() {
//     return (int)verts_.size();
// }

// int Model::nfaces() {
//     return (int)faces_.size();
// }

// std::vector<int> Model::face(int idx) {
//     return faces_[idx];
// }

// Vector3 Model::vert(int i) {
//     return verts_[i];
// }

#endif