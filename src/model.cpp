#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"
#include "graphics.h"

Model::Model(const std::string& filepath) {
    // Open file stream
    std::ifstream fileStream(filepath);
    if (!fileStream.is_open()) {
        std::cerr << "Fail to open " << filepath << std::endl;
        exit(-1);
    }

    // Read by line
    std::string line;
    while (std::getline(fileStream, line)) {
        std::istringstream stringStream(line);
        std::string type;
        double x, y, z;
        unsigned int p0, p1, p2;
        unsigned int t0, t1, t2;
        unsigned int n0, n1, n2;
        char slash;

        stringStream >> type;
        if (type == "v") {
            stringStream >> x >> y >> z;
            positions.push_back({x, y, z});
        }
        else if (type == "vt") {
            stringStream >> x >> y >> z;
            texcoords.push_back({x, y});
        }
        else if (type == "vn") {
            stringStream >> x >> y >> z;
            normals.push_back({x, y, z});
        }
        else if (type == "f") {
            stringStream >> p0 >> slash >> t0 >> slash >> n0;
            stringStream >> p1 >> slash >> t1 >> slash >> n1;
            stringStream >> p2 >> slash >> t2 >> slash >> n2;
            facePositionIndexes.push_back({p0, p1, p2});
            faceTexcoordIndexes.push_back({t0, t1, t2});
            faceNormalIndexes.push_back({n0, n1, n2});
        }
    }

    faceNum = facePositionIndexes.size();
    std::cout << "Face num: " << faceNum << std::endl;
}

void Model::draw(const Shader& shader) {
    for (unsigned int i = 0; i < faceNum; i++) {
        std::vector<unsigned int> positionIndexes = facePositionIndexes[i];
        std::vector<unsigned int> texcoordIndexes = faceTexcoordIndexes[i];

        Vector3 screenCoords[3];
        Vector2 texCoords[3];
        // double intensity[3];
        for (unsigned int j = 0; j < 3; j++) {
            Vector3 v = positions[positionIndexes[j] - 1];
            // Vertex Shder
            screenCoords[j] = 
                shader.viewport * homodiv(shader.mvp * Vector4(v));
            texCoords[j] = texcoords[texcoordIndexes[j] - 1];
        }
        // Fragment Shader
        rasterizeTriangle(screenCoords, texCoords, shader.texture, shader.light,
            shader.framebuffer);
    }
}

// const Vector3& Model::position(unsigned int index) {
//     return positions[index - 1];
// }

// const Vector2& Model::texcoord(unsigned int index) {
//     return texcoords[index - 1];
// }

// const Vector3& Model::normal(unsigned int index) {
//     return normals[index - 1];
// }

// const std::vector<unsigned int>& Model::facePositions(unsigned int index) {
//     return positionIndexes[index];
// }

// const std::vector<unsigned int>& Model::faceTexcoords(unsigned int index) {
//     return texcoordIndexes[index];
// }

// const std::vector<unsigned int>& Model::faceNormals(unsigned int index) {
//     return normalIndexes[index];
// }