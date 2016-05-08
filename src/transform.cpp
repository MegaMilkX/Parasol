#include "transform.h"

Transform::Transform()
: transform(1.0f),
position(0.0f, 0.0f, 0.0f),
rotation(0.0f, 0.0f, 0.0f, 1.0f),
scale(1.0f, 1.0f, 1.0f)
{

}

void Transform::Translate(float x, float y, float z, Space space)
{
    Translate(vec3f(x, y, z), space);
}

void Transform::Translate(vec3f trans, Space space)
{
    if (space == LOCAL)
    {
        trans = toMat3(rotation) * trans;
        position = position + trans;
    }
    else if (space == PARENT)
    {
        position = position + trans;
    }
    else if (space == WORLD)
    {

    }
}

void Transform::Rotate(float angle, float x, float y, float z, Space space)
{
    Rotate(angle, vec3f(x, y, z), space);
}

void Transform::Rotate(float angle, vec3f axis, Space space)
{
    if (space == LOCAL)
    {
        quat q = angleAxis(angle, axis);
        rotation = q * rotation;
    }
    else if (space == PARENT)
    {

    }
    else if (space == WORLD)
    {
        quat q = angleAxis(angle, axis);
        rotation = rotation * q;
    }
    
    rotation = ::normalize(rotation);
}

void Transform::Scale(float x, float y, float z)
{
    scale = vec3f(x, y, z);
}

void Transform::Scale(vec3f scale)
{
    this->scale = scale;
}

void Transform::Scale(float scale)
{
    this->scale = vec3f(scale, scale, scale);
}

mat4f Transform::GetTransform()
{
    transform = mat4f(1.0f);
    return transform = translate(transform, position) * toMat4(rotation) * ::scale(transform, scale);
}