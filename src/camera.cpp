#include "scene.h"

#include "renderable.h"

#include "camera.h"

Camera* Camera::Create(Node* scene)
{
    return Node::Create(scene)->AddEntity<Camera>();
}

void Camera::OnAttach(Node* parent)
{
    scene = parent->GetRoot();
    renderables = scene->GetEntityContainer(TypeInfo<Renderable>::GetId());
    
    Entity::OnAttach(parent);
}

void Camera::Render(GFXTarget* target)
{
    // 1. Get all the renderables belonging to our scene
    // they are in std::vector<Renderable*>* renderables;
    
    // 2. Sort them by visibility and other stuff
    
    // 3. Draw
    for(unsigned int i = 0; i < renderables->size(); ++i)
    {
        ((Renderable*)(*renderables)[i])->Render();
    }
}