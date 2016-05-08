#ifndef _ENTITY_H_
#define _ENTITY_H_

class Node;
class Entity
{
public:
    virtual ~Entity(){}
    virtual void OnAttach(Node* parent){ this->parent = parent; }
    friend Node;
protected:
    Entity(){}
    
    Node* parent;
};

#endif