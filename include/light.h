#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {
public:
    Vector4 color;
public:
    Light() : color(1.0, 1.0, 1.0, 1.0) {}
    Light(const Vector4& _color) : color(_color) {}
};

class DirectionalLight : public Light {
public:
    Vector3 direction;
public:
    DirectionalLight() : direction(0.0, 0.0, -1.0) {}
    DirectionalLight(const Vector4& _color) :
        Light(_color), direction(0.0, 0.0, -1.0) {}
    DirectionalLight(const Vector3& _direction) :
        direction(_direction) {}
    DirectionalLight(const Vector4& _color, const Vector3& _direction) :
        Light(_color), direction(_direction) {}
};

class PointLight : public Light {

};

# endif