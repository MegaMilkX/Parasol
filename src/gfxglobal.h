#ifndef _GFXGLOBAL_H_
#define _GFXGLOBAL_H_

#include <string>
#include <vector>
#include <map>

#include "math3f.h"

template<typename TYPE>
class GFXGlobal
{
public:
    static GFXGlobal Get(std::string name, unsigned int index = 0);
    void operator=(const TYPE& value);
private:
    std::map<std::string, unsigned int> name_index;
    std::vector<TYPE> data;
};

#endif