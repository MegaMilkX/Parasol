#ifndef _GEOMETRYBUFFER_H_
#define _GEOMETRYBUFFER_H_

class GeometryBuffer
{
public:
    GeometryBuffer() : buffer(0), target(0), usage(0) {}
    static GeometryBuffer Create(unsigned int target, unsigned int usage_hint);
	void Destroy();
    bool Data(void* data, size_t sz);
    bool Valid();
    void Bind();
private:
    
    unsigned int buffer;
    unsigned int target;
    unsigned int usage;
};

#endif