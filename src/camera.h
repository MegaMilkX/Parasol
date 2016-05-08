#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "entity.h"
#include "node.h"

#include "gfxtarget.h"

class Camera : public Entity
{
public:
    //Creates a child node for parent argument with attached entity created
    static Camera* Create(Node* scene);
    
    void OnAttach(Node* parent);
    
    void Render(GFXTarget* target);
    
    friend Node;
private:
    Camera(){}
    
    Scene* scene;
    std::vector<Entity*>* renderables;
};

#endif