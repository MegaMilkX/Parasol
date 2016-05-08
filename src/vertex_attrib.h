#ifndef _VERTEX_ATTRIB_H_
#define _VERTEX_ATTRIB_H_

#include "typeid.h"
#include "math3f.h"

struct AttrPosition
{
    vec3f value;
    static short elemCount(){ return 3; }
    static int elemType(){ return TypeInfo<float>::GetId(); }
    static size_t size(){ return sizeof(AttrPosition); }
};

struct AttrNormal
{
    vec3f value;
    static short elemCount(){ return 3; }
    static int elemType(){ return TypeInfo<float>::GetId(); }
    static size_t size(){ return sizeof(AttrNormal); }
};

struct AttrUV
{
    vec2f value;
    static short elemCount(){ return 2; }
    static int elemType(){ return TypeInfo<float>::GetId(); }
    static size_t size(){ return sizeof(AttrUV); }
};

struct AttrRGBA
{
    vec4f value;
    static short elemCount(){ return 4; }
    static int elemType(){ return TypeInfo<float>::GetId(); }
    static size_t size(){ return sizeof(AttrRGBA); }
};

#endif