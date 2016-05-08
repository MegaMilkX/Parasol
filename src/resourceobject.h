#ifndef _RESOURCEOBJECT_H_
#define _RESOURCEOBJECT_H_

#include "macro\macro_readers_def.h"

class ResourceObject
{
public:
    virtual ResourceObject(){}
    virtual int ReaderCount() = 0;
    virtual std::string GetReaderExtension(int id) = 0;
private:
};

#endif