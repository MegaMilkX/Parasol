#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "math3f.h"

class Transform
{
public:
    Transform();

    void Translate(float x, float y, float z);
    void Translate(vec3f trans);
    void Rotate(float angle, float x, float y, float z);
    void Rotate(float angle, vec3f axis);
    void Scale(float x, float y, float z);
    void Scale(vec3f scale);
    void Scale(float scale);

    vec3f Position() { return position; }
    quat Rotation() { return rotation; }
    vec3f Scale() { return scale; }

    void Position(const vec3f& position) { this->position = position; }
    void Rotation(const quat& rotation) { this->rotation = rotation; }

    mat4f GetTransform();
private:
    vec3f position;
    quat  rotation;
    vec3f scale;
    mat4f transform;
};

#endif