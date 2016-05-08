#ifndef _GEOMETRYBUFFER_H_
#define _GEOMETRYBUFFER_H_

class GeometryBuffer
{
public:
    GeometryBuffer(){}
    static GeometryBuffer Create(unsigned int target, unsigned int usage_hint);
    bool Data(void* data, size_t sz);
private:
    
    unsigned int buffer;
    unsigned int target;
    unsigned int usage;
};

#endif