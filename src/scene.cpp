#include "scene.h"

Scene Scene::Create()
{
    return Scene();
}

void Scene::AddEntity(int type, Entity* entity)
{
    entity_containers[type].push_back(entity);
}

std::vector<Entity*>* Scene::GetEntityContainer(int type)
{
    return &(entity_containers[type]);
}

Scene::Scene(){}