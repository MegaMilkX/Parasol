#ifndef _R3DATTRIB_H_
#define _R3DATTRIB_H_

#include <vector>

#define R3D_POSITION 100
#define R3D_NORMAL 200
#define R3D_TANGENT 300
#define R3D_BITANGENT 400
#define R3D_UV 500
#define R3D_COLOR 600
#define R3D_BONE 700
#define R3D_WEIGHT 800
#define R3D_INDEX 900

class IR3DAttrib
{
public:
    virtual ~IR3DAttrib(){}
};

template<int TYPE_INDEX, typename ELEM_TYPE, int ELEM_COUNT>
class R3DAttrib : public IR3DAttrib, public std::vector<ELEM_TYPE>
{
public:
    R3DAttrib& operator<<(const ELEM_TYPE value)
    {
        push_back(value);
        return (*this);
    }
    static int type_index(){ return TYPE_INDEX; }
private:
};

class R3DPosition : public R3DAttrib<R3D_POSITION, float, 3>{};
class R3DNormal : public R3DAttrib<R3D_NORMAL, float, 3>{};
class R3DTangent : public R3DAttrib<R3D_TANGENT, float, 3>{};
class R3DBitangent : public R3DAttrib<R3D_BITANGENT, float, 3>{};
class R3DTexCoord : public R3DAttrib<R3D_UV, float, 2>{};
class R3DColorRGBA : public R3DAttrib<R3D_COLOR, unsigned char, 4>{};
class R3DColorRGB : public R3DAttrib<R3D_COLOR, unsigned char, 3>{};
class R3DColorRG : public R3DAttrib<R3D_COLOR, unsigned char, 2>{};
class R3DColorR : public R3DAttrib<R3D_COLOR, unsigned char, 1>{};
class R3DBone4 : public R3DAttrib<R3D_BONE, unsigned short, 4>{};
class R3DBone3 : public R3DAttrib<R3D_BONE, unsigned short, 3>{};
class R3DBone2 : public R3DAttrib<R3D_BONE, unsigned short, 2>{};
class R3DBone1 : public R3DAttrib<R3D_BONE, unsigned short, 1>{};
class R3DBoneWeight4 : public R3DAttrib<R3D_WEIGHT, unsigned char, 4>{};
class R3DBoneWeight3 : public R3DAttrib<R3D_WEIGHT, unsigned char, 3>{};
class R3DBoneWeight2 : public R3DAttrib<R3D_WEIGHT, unsigned char, 2>{};
class R3DBoneWeight1 : public R3DAttrib<R3D_WEIGHT, unsigned char, 1>{};
class R3DIndex : public R3DAttrib<R3D_INDEX, unsigned int, 1>{};

#endif