#include "renderable.h"

Renderable* Renderable::Create(Node* scene)
{
    return Node::Create(scene)->AddEntity<Renderable>();
}

void Renderable::OnAttach(Node* parent)
{
    Entity::OnAttach(parent);
}

void Renderable::Render()
{
    
}