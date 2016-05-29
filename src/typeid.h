#ifndef _TYPEID_H_
#define _TYPEID_H_

#include <iostream>
#include <typeindex>

template<typename T>
struct TypeInfo
{
    static int GetId()
    {
        static int id = _NewId();
        //std::cout << "TypeInfo for " << typeid(T).name() << " = " << id << std::endl;
        return id;
    }
private:
};

inline int _NewId()
{
    static int id;
    return ++id;
}
/*
template<typename T>
int TypeInfo<T>::id = _NewId();
*/
#endif