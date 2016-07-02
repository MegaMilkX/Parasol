#include "gfxtarget.h"

extern HDC deviceContext;

std::set<GFXTarget*> targets;

//Public

GFXTarget::~GFXTarget()
{
}

void GFXTarget::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Static

GFXTarget* GFXTarget::Create(GFXTarget* parent)
{
    GFXTarget* tgt = new GFXTarget();
    tgt->parent = parent;
    targets.insert(tgt);
    return tgt;
}

GFXTarget* GFXTarget::Create()
{
    GFXTarget* tgt = new GFXTarget();
    targets.insert(tgt);
    return tgt;
}

void GFXTarget::Delete(GFXTarget* object)
{
    targets.erase(object);
    delete object;
}
    
//Private
GFXTarget::GFXTarget() : target(0)
{
}