#ifndef _NODE_H_
#define _NODE_H_

#include <set>
#include <map>

#include "typeid.h"

#include "entity.h"

#include "transform.h"

class Scene;
class Node
{
public:
    virtual ~Node();
    template<typename T>
    T* AddEntity();
    template<typename T>
    T* GetEntity();
    
    virtual Scene* GetRoot(){ return parent->GetRoot(); }

    static Node* Create(Node* parent);
private:
    void AddEntityToRoot(int type, Entity* entity);

    Node* parent;
    std::set<Node*> children;
    std::map<int, Entity*> components;
};

template<typename T>
T* Node::AddEntity()
{
    if(components.find(TypeInfo<T>::GetId()) != components.end())
        return 0;
        
    T* t = new T();
    components.insert(std::make_pair(TypeInfo<T>::GetId(), t));
    t->OnAttach(this);
    AddEntityToRoot(TypeInfo<T>::GetId(), t);
    return t;
}
template<typename T>
T* Node::GetEntity()
{
    std::map<int, Entity*>::iterator it = components.find(TypeInfo<T>::GetId());
    if(it != components.end())
        return it->second;
    else
        return 0;
}

#endif