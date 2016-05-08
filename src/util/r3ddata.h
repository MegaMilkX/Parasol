#ifndef _R3DDATA_H_
#define _R3DDATA_H_

#include <map>
#include <vector>

#include "..\filesystem\file.h"

#include "r3dattrib.h"

class R3DData
{
public:
    typedef int type_index;
    //bool Read(File file);
    //bool Write(File file);

    template<typename T>
    void PushAttribute(T& data);
    template<typename T>
    T* GetAttribute(unsigned int instance = 0);
    template<typename T>
    unsigned int AttribCount();
private:
    std::map<type_index, std::vector<IR3DAttrib*>> attributes;
};

template<typename T>
void R3DData::PushAttribute(T& data)
{
    T* new_attr = new T();
    memcpy(new_attr, &data, sizeof(T));

    if (attributes.find(T::type_index()) == attributes.end())
    {
        std::vector<IR3DAttrib*> atrs;
        atrs.push_back(new_attr);
        attributes.insert(std::make_pair(T::type_index(), atrs));
    }
    else
    {
        std::map<type_index, std::vector<IR3DAttrib*>>::iterator it;
        it = attributes.find(T::type_index());
        it->second.push_back(new_attr);
    }
}

template<typename T>
T* R3DData::GetAttribute(unsigned int instance)
{
    std::vector<IR3DAttrib*> vec = attributes[T::type_index()];

    if (instance < vec.size())
    {
        return (T*)vec[instance];
    }
    else
    {
        return 0;
    }
}

template<typename T>
unsigned int R3DData::AttribCount()
{
    if (attributes.find(T::type_index()) == attributes.end())
    {
        return 0;
    }
    else
    {
        std::map<type_index, std::vector<IR3DAttrib*>>::iterator it;
        it = attributes.find(T::type_index());
        return it->second.size();
    }
}

#endif
