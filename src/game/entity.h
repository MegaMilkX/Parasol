#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <libgame.h>

#include "node.h"

class Node;
class Scene;
class Entity
{
public:
    virtual ~Entity() {}
    Node* GetNode() { return node; }
    void Translate(float x, float y, float z, Space space = LOCAL);
    void Translate(vec3f trans, Space space = LOCAL);
    void Rotate(float angle, float x, float y, float z, Space space = LOCAL);
    void Rotate(float angle, vec3f axis, Space space = LOCAL);
    void Scale(float x, float y, float z);
    void Scale(vec3f scale);
    void Scale(float scale);

    vec3f Position() { return node->Position(); }
    quat Rotation() { return node->Rotation(); }
    vec3f Scale() { return node->Scale(); }

    void Position(vec3f& position, Space space = LOCAL) { node->Position(position, space); }
    void Rotation(quat& rotation, Space space = LOCAL) { node->Rotation(rotation, space); }

    vec3f Forward();
    vec3f Back();
    vec3f Left();
    vec3f Right();
protected:
    Entity(Node* node);
    Node* node;
};

#define CONSTRUCTOR(SOURCE) SOURCE
#define PUBLIC(SOURCE) public: SOURCE
#define PROTECTED(SOURCE) protected: SOURCE

#define DEF_ENTITY(NAME, BASE, CONSTRUCT, PUB, PROT) \
    class NAME : public BASE \
    { \
    public: \
        static NAME* Create(Scene* scene) \
        { \
            NAME* entity = new NAME(scene->Root()->AddNode()); \
            return entity; \
        } \
        static NAME* Create(Node* node) \
        { \
            NAME* entity = new NAME(node); \
            return entity; \
        } \
    protected: \
        NAME(Node* node) : BASE(node) \
        { \
            Scene* scene = node->GetScene(); \
            scene->AddEntityType(TypeInfo<NAME>::GetId(), this); \
            node->Add<NAME>(this); \
            CONSTRUCT \
        } \
        PUB \
        PROT \
    }

#define ENTITY_BODY(NAME, BASE, CONSTRUCT) \
	public: \
	static NAME* Create(Scene* scene) \
    { \
        NAME* entity = new NAME(scene->Root()->AddNode()); \
        return entity; \
    } \
    static NAME* Create(Node* node) \
    { \
        NAME* entity = new NAME(node); \
        return entity; \
    } \
	protected: \
	NAME(Node* node) : BASE(node) \
    { \
        Scene* scene = node->GetScene(); \
        scene->AddEntityType(TypeInfo<NAME>::GetId(), this); \
        node->Add<NAME>(this); \
        CONSTRUCT \
    }



#endif
