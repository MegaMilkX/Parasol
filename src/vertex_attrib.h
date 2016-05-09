#ifndef _VERTEX_ATTRIB_H_
#define _VERTEX_ATTRIB_H_

#include "typeid.h"
#include "math3f.h"

#define DEF_ATTRIBUTE(NAME, ELEM_TYPE, ELEM_COUNT, STORAGE_TYPE, BASE_INDEX) \
    template<int INSTANCE_ID> \
    struct NAME \
    { \
        STORAGE_TYPE value; \
        static short elemCount(){ return ELEM_COUNT; } \
        static int elemType(){return TypeInfo<ELEM_TYPE>::GetId(); } \
        static size_t size(){ return sizeof(NAME); } \
        static unsigned int index(){ return BASE_INDEX + INSTANCE_ID; } \
        static std::string name(){ return #NAME + std::to_string(INSTANCE_ID); } \
    }

namespace VertexAttrib
{

    DEF_ATTRIBUTE(Position, float, 3, vec3f, 0);
    DEF_ATTRIBUTE(Normal, float, 3, vec3f, 1);
    DEF_ATTRIBUTE(UV, float, 2, vec2f, 2);
    DEF_ATTRIBUTE(RGBA, float, 4, vec4f, 13);

}

#endif