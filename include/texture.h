#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include "vector.h"

class Texture {
public:
    int width;
    int height;
    std::vector<Vector4> buffer;
};

class Cubemap {
public:
    Texture faces[6];
};

#endif