#ifndef _GFXS_TYPE_H_
#define _GFXS_TYPE_H_

#include <string>
#include "math3f.h"

#define DEF_SHADER_TYPE(NAME, CTYPE, INTERNAL_NAME, INITIAL) \
    class NAME : public Type \
    { \
    public: \
        NAME* Clone() { return new NAME(*this); } \
        std::string InternalName() { return INTERNAL_NAME; } \
        std::string InitialValue() { return INITIAL; } \
    }

namespace GFXS
{
    class Type
    {
    public:
        virtual Type* Clone() = 0;
        virtual std::string InternalName() = 0;
        virtual std::string InitialValue() = 0;
    };

    DEF_SHADER_TYPE(Float, float, "float", "0.0");
    DEF_SHADER_TYPE(Vec2, vec2f, "vec2", "vec2(0.0)");
    DEF_SHADER_TYPE(Vec3, vec3f, "vec3", "vec3(0.0)");
    DEF_SHADER_TYPE(Vec4, vec4f, "vec4", "vec4(0.0)");
    DEF_SHADER_TYPE(Mat3, mat3f, "mat3", "mat3(1.0)");
    DEF_SHADER_TYPE(Mat4, mat4f, "mat4", "mat4(1.0)");
    DEF_SHADER_TYPE(Sampler2D, int, "sampler2D", "0");
}

#endif