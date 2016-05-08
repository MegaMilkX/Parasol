#ifndef _DATAHANDLE_H_
#define _DATAHANDLE_H_

#include <vector>
#include <string>

template<typename DATA>
class ResHdl
{
public:
    ResHdl() : handle(0){}
    DATA* operator->();
    void Release();

    static ResHdl<DATA> Create();
    static ResHdl<DATA> Create(DATA value);
private:
    ResHdl(unsigned int index) : index(index), magic(++ResHdl<DATA>::magic_next){}
    union
    {
        struct
        {
            unsigned int index : 16;
            unsigned int magic : 16;
        };
        unsigned int handle;
    };

    //
    static std::vector<DATA> data_storage;
    static std::vector<unsigned int> magic_storage;
    static std::vector<unsigned int> free_storage;
    static unsigned int magic_next;
};

template<typename DATA>
std::vector<DATA> ResHdl<DATA>::data_storage;
template<typename DATA>
std::vector<unsigned int> ResHdl<DATA>::magic_storage;
template<typename DATA>
std::vector<unsigned int> ResHdl<DATA>::free_storage;
template<typename DATA>
unsigned int ResHdl<DATA>::magic_next = 0;

template<typename DATA>
DATA* ResHdl<DATA>::operator->()
{
    if (magic_storage[index] != magic)
        return 0;
    return &(data_storage[index]);
}

template<typename DATA>
void ResHdl<DATA>::Release()
{
    if (magic == 0)
        return;
    if (ResHdl<DATA>::magic_storage[index] != magic)
        return;
    ResHdl<DATA>::magic_storage[index] = 0;
    ResHdl<DATA>::free_storage.push_back(index);
    ResHdl<DATA>::data_storage[index] = DATA();
}

template<typename DATA>
ResHdl<DATA> ResHdl<DATA>::Create()
{
    ResHdl<DATA> resource;
    unsigned int index;
    if (free_storage.empty())
    {
        index = data_storage.size();
        data_storage.push_back(DATA());
        resource = ResHdl<DATA>(index);
        magic_storage.push_back(resource.magic);
    }
    else
    {
        index = free_storage.back();
        free_storage.pop_back();
        resource = ResHdl<DATA>(index);
        magic_storage[index] = resource.magic;
    }

    return resource;
}

template<typename DATA>
ResHdl<DATA> ResHdl<DATA>::Create(DATA value)
{
    ResHdl<DATA> resource;
    unsigned int index;
    if (free_storage.empty())
    {
        index = data_storage.size();
        data_storage.push_back(value);
        resource = ResHdl<DATA>(index);
        magic_storage.push_back(resource.magic);
    }
    else
    {
        index = free_storage.back();
        free_storage.pop_back();
        resource = ResHdl<DATA>(index);
        data_storage[index] = value;
        magic_storage[index] = resource.magic;
    }

    return resource;
}

#endif