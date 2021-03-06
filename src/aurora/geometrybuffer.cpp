#include "geometrybuffer.h"

#include "glextutil.h"

GeometryBuffer GeometryBuffer::Create(unsigned int target, unsigned int usage_hint)
{
    GeometryBuffer gb;
    gb.target = target;
    gb.usage = usage_hint;
    glGenBuffers(1, &gb.buffer);
    return gb;
}

void GeometryBuffer::Destroy()
{
	glDeleteBuffers(1, &buffer);
}

bool GeometryBuffer::Data(void* data, size_t sz)
{
    glBindBuffer(target, buffer);
    glBufferData(target, sz, data, usage);
    return true;
}

bool GeometryBuffer::Valid()
{
    return buffer != 0;
}

void GeometryBuffer::Bind()
{
    glBindBuffer(target, buffer);
}