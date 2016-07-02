#ifndef _GFXTARGET_H_
#define _GFXTARGET_H_

#include "glextutil.h"

#include <set>

class GFXTarget
{
public:
    ~GFXTarget();
    
    void Clear();
    
    static GFXTarget* Create(GFXTarget* parent);
    //Creates default GFXTarget. Don't use
    static GFXTarget* Create();
    static void Delete(GFXTarget* object);
private:
    GFXTarget();
    
    GFXTarget* parent;
    int target;
};

#endif