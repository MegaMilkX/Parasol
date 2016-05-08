#include "scene.h"

#include "node.h"

Node::~Node()
{
    std::set<Node*>::iterator children_it = children.begin();
    for(children_it; children_it != children.end(); ++children_it)
    {
        delete (*children_it);
    }
    children.clear();
    
    std::map<int, Entity*>::iterator comp_it = components.begin();
    for(comp_it; comp_it != components.end(); ++comp_it)
    {
        delete comp_it->second;
    }
    components.clear();
}

Node* Node::Create(Node* parent)
{
    Node* node = new Node();
    node->parent = parent;
    parent->children.insert(node);    
    return node;
}

void Node::AddEntityToRoot(int type, Entity* entity)
{
    GetRoot()->AddEntity(type, entity);
}