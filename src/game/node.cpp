#include "node.h"

#include "scene.h"

Node::Node(Scene* scene) : scene(scene), parent(0)
{}
Node::Node(Node* parent) : scene(parent->GetScene()), parent(parent)
{}

void Node::Translate(float x, float y, float z, Space space) { Translate(vec3f(x, y, z), space); }
void Node::Translate(vec3f trans, Space space) 
{ 
    if (space == Space::LOCAL)
    {
        trans = toMat3(transform.Rotation()) * trans;
        transform.Translate(trans);
    }
    else if (space == Space::PARENT)
    {
        transform.Translate(trans);
    }
    else if (space == Space::WORLD)
    {
        trans = inverse(parent->GetTransform()) * trans;
        transform.Translate(trans);
    }
}
void Node::Rotate(float angle, float x, float y, float z, Space space) { Rotate(angle, vec3f(x, y, z), space); }
void Node::Rotate(float angle, vec3f axis, Space space) 
{
    if (space == Space::WORLD)
        if (parent == 0)
            space = Space ::PARENT;

    if (space == Space::LOCAL)
    {
        axis = toMat3(transform.Rotation()) * axis;
        transform.Rotate(angle, axis);
    }
    else if (space == Space::PARENT)
    {
        transform.Rotate(angle, axis);
    }
    else if (space == Space::WORLD)
    {
        axis = inverse(parent->GetTransform()) * axis;
        transform.Rotate(angle, axis);
    }
}

void Node::Scale(float x, float y, float z) { transform.Scale(x, y, z); }
void Node::Scale(vec3f scale) { transform.Scale(scale); }
void Node::Scale(float scale) { transform.Scale(scale); }

vec3f Node::Forward()
{
    return -(GetTransform()[2]);
}
vec3f Node::Back()
{
    return (GetTransform()[2]);
}
vec3f Node::Left()
{
    return -(GetTransform()[0]);
}
vec3f Node::Right()
{
    return (GetTransform()[0]);
}