#ifndef _GFXGLOBAL_H_
#define _GFXGLOBAL_H_

#include <string>
#include <vector>
#include <map>

#include "glextutil.h"

#include "math3f.h"

class IGFXGlobal
{
public:
    IGFXGlobal() : data_index(0) {}
    virtual ~IGFXGlobal() {};
    virtual void Uniform(unsigned int location) = 0;
    unsigned int DataIndex() { return data_index; }
protected:
    unsigned int data_index;
};

template<typename TYPE>
class GFXGlobal : public IGFXGlobal
{
public:
    GFXGlobal() {}
    GFXGlobal(unsigned int idx) 
    {
        data_index = idx;
    }
    static GFXGlobal Get(std::string name)
    {
        std::map<std::string, unsigned int>::iterator it;
        it = name_index.find(name);
        if (it != name_index.end())
            return GFXGlobal<TYPE>(it->second);
        else
        {
            unsigned int id = data.size();
            name_index.insert(std::make_pair(name, id));
            data.push_back(TYPE());
            GFXGlobal<TYPE> glob(id);
            return glob;
        }
    }

    void operator=(const TYPE& value)
    {
        GFXGlobal<TYPE>::data[data_index] = value;
    }

    operator TYPE()
    {
        return GFXGlobal<TYPE>::data[data_index];
    }

    void Uniform(unsigned int location) {}

private:
    static std::map<std::string, unsigned int> name_index;
    static std::vector<TYPE> data;
};

template<typename TYPE>
std::map<std::string, unsigned int> GFXGlobal<TYPE>::name_index;
template<typename TYPE>
std::vector<TYPE> GFXGlobal<TYPE>::data;

template<>
void GFXGlobal<float>::Uniform(unsigned int location) { glUniform1f(location, data[data_index]); }
template<>
void GFXGlobal<vec2f>::Uniform(unsigned int location) { glUniform2f(location, data[data_index].x, data[data_index].y); }
template<>
void GFXGlobal<vec3f>::Uniform(unsigned int location) { glUniform3f(location, data[data_index].x, data[data_index].y, data[data_index].z); }
template<>
void GFXGlobal<vec4f>::Uniform(unsigned int location) { glUniform4f(location, data[data_index].x, data[data_index].y, data[data_index].z, data[data_index].w); }
template<>
void GFXGlobal<int>::Uniform(unsigned int location) { glUniform1i(location, data[data_index]); }
template<>
void GFXGlobal<vec2i>::Uniform(unsigned int location) { glUniform2i(location, data[data_index].x, data[data_index].y); }
template<>
void GFXGlobal<vec3i>::Uniform(unsigned int location) { glUniform3i(location, data[data_index].x, data[data_index].y, data[data_index].z); }
template<>
void GFXGlobal<vec4i>::Uniform(unsigned int location) { glUniform4i(location, data[data_index].x, data[data_index].y, data[data_index].z, data[data_index].w); }
template<>
void GFXGlobal<unsigned int>::Uniform(unsigned int location) { glUniform1ui(location, data[data_index]); }
template<>
void GFXGlobal<vec2ui>::Uniform(unsigned int location) { glUniform2ui(location, data[data_index].x, data[data_index].y); }
template<>
void GFXGlobal<vec3ui>::Uniform(unsigned int location) { glUniform3ui(location, data[data_index].x, data[data_index].y, data[data_index].z); }
template<>
void GFXGlobal<vec4ui>::Uniform(unsigned int location) { glUniform4ui(location, data[data_index].x, data[data_index].y, data[data_index].z, data[data_index].w); }
template<>
void GFXGlobal<mat3f>::Uniform(unsigned int location) { glUniformMatrix3fv(location, 1, GL_FALSE, (float*)&(data[data_index])); }
template<>
void GFXGlobal<mat4f>::Uniform(unsigned int location) { glUniformMatrix4fv(location, 1, GL_FALSE, (float*)&(data[data_index])); }

#endif