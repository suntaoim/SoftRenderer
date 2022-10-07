#include <fstream>
#include <sstream>
#include <vector>
#include "mesh.h"
#include "graphics.h"

Mesh::Mesh(const std::string& filepath) {
    // Open file stream
    std::ifstream fileStream(filepath);
    if (!fileStream.is_open()) {
        std::cerr << "Fail to open " << filepath << std::endl;
        exit(-1);
    }

    // Temporary container
    std::vector<Vector3> positions;
    std::vector<Vector2> texcoords;
    std::vector<Vector3> normals;
    std::vector<int> facePositionIndices;
    std::vector<int> faceTexcoordIndices;
    std::vector<int> faceNormalIndices;
    std::string type;
    double x, y, z;
    int p[3], t[3], n[3];
    char slash;

    // Read by line
    std::string line;
    while (std::getline(fileStream, line)) {
        std::istringstream stringStream(line);

        stringStream >> type;
        if (type == "v") {
            stringStream >> x >> y >> z;
            positions.emplace_back(x, y, z);
        }
        else if (type == "vt") {
            stringStream >> x >> y >> z;
            texcoords.emplace_back(x, y);
        }
        else if (type == "vn") {
            stringStream >> x >> y >> z;
            normals.emplace_back(x, y, z);
        }
        else if (type == "f") {
            stringStream >> p[0] >> slash >> t[0] >> slash >> n[0];
            stringStream >> p[1] >> slash >> t[1] >> slash >> n[1];
            stringStream >> p[2] >> slash >> t[2] >> slash >> n[2];
            for (int i = 0; i < 3; i++) {
                facePositionIndices.emplace_back(p[i] - 1);
                faceTexcoordIndices.emplace_back(t[i]- 1);
                faceNormalIndices.emplace_back(n[i]- 1);
            }
        }
    }

    // Store verteices information by faces
    int indicesNum = static_cast<int>(facePositionIndices.size());
    vertices.resize(indicesNum);
    facesNum = indicesNum / 3;
    std::cout << "Face num: " << facesNum << std::endl;
    for (int i = 0; i < indicesNum; i++) {
        vertices[i].position = positions[facePositionIndices[i]];
        vertices[i].texcoord = texcoords[faceTexcoordIndices[i]];
        vertices[i].normal = normals[faceNormalIndices[i]];
    }
}