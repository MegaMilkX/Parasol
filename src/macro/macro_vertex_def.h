//========================================
// Abandon hope all ye who enter here
//========================================

#ifndef _MACRO_VERTEX_DEF_H_
#define _MACRO_VERTEX_DEF_H_

#include "vertex_attrib.h"

#include "macro_for_each_arg.h"
#include "macro_typed_expr.h"
#include "macro_narg.h"

struct AttrInfo
{
    short elemCount;
    int elemType;
    size_t size;
};

#define VERTEX(...) \
    static int attribCount() { return PP_NARGS(__VA_ARGS__); } \
    template<typename T> \
    void Set(T& value){} \
    static AttrInfo getAttrInfo(int i) \
    { \
        AttrInfo attrInfo; \
        switch (i) \
        { \
            FOR_EACH_ARG(CASE, __VA_ARGS__) \
        } \
        return attrInfo; \
    } \
    FOR_EACH_ARG(DEF, __VA_ARGS__)

#define CASE(i, arg) case i: attrInfo = STRIP(arg)##Info(); break;
#define DEF(i, arg) PAIR(arg); \
    template<> \
    void Set(TYPEOF(arg)& value){ this->STRIP(arg) = value; } \
    static AttrInfo STRIP(arg)##Info() \
    { \
        AttrInfo info; \
        info.elemCount = TYPEOF(arg)::elemCount(); \
        info.elemType = TYPEOF(arg)::elemType(); \
        info.size = TYPEOF(arg)::size(); \
        return info; \
    }

#endif