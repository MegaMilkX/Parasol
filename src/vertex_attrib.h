#ifndef _VERTEX_ATTRIB_H_
#define _VERTEX_ATTRIB_H_

#include "typeid.h"
#include "math3f.h"

#include "gfxglobal.h"

class TAGAttrib {};

#define DEF_ATTRIBUTE(NAME, ELEM_TYPE, ELEM_COUNT, STORAGE_TYPE) \
    template<int INSTANCE_ID> \
    struct NAME \
    { \
        STORAGE_TYPE value; \
        static short elemCount(){ return ELEM_COUNT; } \
        static int elemType(){return TypeInfo<ELEM_TYPE>::GetId(); } \
        static size_t size(){ return sizeof(NAME); } \
        static unsigned int index(){ return GFXGlobal<TAGAttrib>::Get(name()).DataIndex(); } \
        static std::string name(){ return #NAME + std::to_string(INSTANCE_ID); } \
    }

namespace VertexAttrib
{

    DEF_ATTRIBUTE(Position, float, 3, vec3f);
    DEF_ATTRIBUTE(Normal, float, 3, vec3f);
    DEF_ATTRIBUTE(UV, float, 2, vec2f);
    DEF_ATTRIBUTE(RGBA, float, 4, vec4f);

}

#endif