#ifndef _TYPEID_H_
#define _TYPEID_H_

#include <iostream>

template<typename T>
struct TypeInfo
{
    static int GetId()
    {
        return id;
    }
private:
    static int id;
};

static int _NewId()
{
    static int id;
    return ++id;
}

template<typename T>
int TypeInfo<T>::id = _NewId();

#endif