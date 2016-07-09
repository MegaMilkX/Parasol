#ifndef _SCENE_H_
#define _SCENE_H_

#include <map>
#include <vector>

#include <libgame.h>

#include "node.h"
#include "scene-system.h"
#include "entity.h"

//============================
// Scene
//============================
class Scene
{
public:
    /*Has to initialize its root node*/
    Scene() : root_node(this) {  }
    //
    ~Scene();

    /* Scene systems */
    template<typename SYSTEM>
    SYSTEM*                 AddSystem();
    template<typename SYSTEM>
    SYSTEM*                 GetSystem();
    void                    AddSystemInstance(ISceneSystem* system);
    void                    AddSystemType(int index, ISceneSystem* system);

    /*Adds a Node to root with an attached entity of specified type*/
    template<typename ENTITY>
    ENTITY*                 Add();
    /*Get scene root Node*/
    Node*                   Root() { return &root_node; }
    // 
    void                    AddEntityInstance(Entity* entity);
    void                    AddEntityType(int index, Entity* entity);
    //
	template<typename T>
	std::vector<T*>&        GetEntities();
    std::vector<Entity*>&   GetEntitiesByType(int typeindex);
    //
    Node*                   RayTest(const ray& r, vec3f& point);
    std::vector<Node*>      RayTest(const ray& r);
private:
    /*Can't copy, won't copy*/
    Scene(const Scene& other) : root_node(this) {}
    Scene& operator=(const Scene& other) { return *this; }
    /*Systems*/
    std::vector<ISceneSystem*> systems;
    std::map<int, ISceneSystem*> system_by_type;
    /*Has a root node, is not itself one*/
    Node root_node;
    /*Scene owns all entities*/
    std::vector<Entity*> entities;
    /*Also stores them by type. Same entity can be stored as more than one type*/
    std::map<int, std::vector<Entity*>> entity_by_type;
};

template<typename SYSTEM>
SYSTEM* Scene::AddSystem()
{
    SYSTEM* system = new SYSTEM(this);
    return system;
}

template<typename SYSTEM>
SYSTEM* Scene::GetSystem()
{
    int index = TypeInfo<SYSTEM>::GetId();
    std::map<int, ISceneSystem*>::iterator it = system_by_type.find(index);
    if(it == system_by_type.end())
        return 0;
    return (SYSTEM*)(it->second);
}

template<typename ENTITY>
ENTITY* Scene::Add()
{
    ENTITY* entity = ENTITY::Create(this);
    return entity;
}

template<typename T>
std::vector<T*>& Scene::GetEntities()
{
	std::vector<Entity*>& e = GetEntitiesByType(TypeInfo<T>::GetId());
	void* ptr = &e;
	std::vector<T*>& r = *(std::vector<T*>*)ptr;

	return r;
}

#endif