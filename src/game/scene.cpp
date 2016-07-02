#include "scene.h"

#include "bounding-shape.h"

Scene::~Scene()
{
    for (unsigned int i = 0; i < entities.size(); ++i)
        delete entities[i];
}

void Scene::AddEntityInstance(Entity* entity)
{
    entities.push_back(entity);
}

void Scene::AddEntityType(int index, Entity* entity)
{
    entity_by_type[index].push_back(entity);
}

std::vector<Entity*>& Scene::GetEntitiesByType(int typeindex)
{
    return entity_by_type[typeindex];
}

Node* Scene::RayTest(const ray& r, vec3f& point)
{
    Node* result_node = 0;
    float dist = 1000000000000.0f;
    vec3f pt;
    std::vector<Entity*> shapes = Root()->GetScene()->GetEntitiesByType(TypeInfo<BoundingShape>::GetId());
    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        BoundingShape* shape = (BoundingShape*)(shapes[i]);
        if (shape->RayTest(r, point))
        {
            float d = (point - r.origin).length();
            if (d < dist)
            {
                pt = point;
                dist = d;
                result_node = shape->GetNode();
            }
        }
    }
    point = pt;
    return result_node;
}

std::vector<Node*> Scene::RayTest(const ray& r)
{
    std::vector<Node*> nodes;
    vec3f point;
    std::vector<Entity*> shapes = Root()->GetScene()->GetEntitiesByType(TypeInfo<BoundingShape>::GetId());
    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        BoundingShape* shape = (BoundingShape*)(shapes[i]);
        if (shape->RayTest(r, point))
            nodes.push_back(shape->GetNode());
    }

    return nodes;
}