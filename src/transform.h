#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "math3f.h"

enum Space
{
    LOCAL,
    PARENT,
    WORLD
};

class Transform
{
public:
    Transform();

    void Translate(float x, float y, float z, Space space = LOCAL);
    void Translate(vec3f trans, Space space = LOCAL);
    void Rotate(float angle, float x, float y, float z, Space space = LOCAL);
    void Rotate(float angle, vec3f axis, Space space = LOCAL);
    void Scale(float x, float y, float z);
    void Scale(vec3f scale);
    void Scale(float scale);

    mat4f GetTransform();
private:
    vec3f position;
    quat  rotation;
    vec3f scale;
    mat4f transform;
};

#endif