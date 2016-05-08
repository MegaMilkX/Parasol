#ifndef _SCENE_H_
#define _SCENE_H_

#include <map>
#include <vector>

#include "node.h"

class Scene : public Node
{
public:
    static Scene Create();
    
    Scene* GetRoot(){ return this; }
    
    void AddEntity(int type, Entity* entity);
    std::vector<Entity*>* GetEntityContainer(int type);
private:
    Scene();
    
    std::map<int, std::vector<Entity*>> entity_containers;
};

#endif