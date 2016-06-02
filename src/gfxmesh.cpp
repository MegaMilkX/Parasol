#include "gfxmesh.h"

#include <iostream>

//GL_HALF_FLOAT, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV

GFXMesh GFXMesh::Create(int usage)
{
    GFXMesh mesh;
    return mesh;
}
  
void GFXMesh::SetIndices(std::vector<unsigned short> indices)
{
    if(indices.size() == 0)
        return;
    
    index_buffer = GeometryBuffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    index_buffer.Data(indices.data(), sizeof(unsigned short) * indices.size());
    index_count = indices.size();
    index_type = GL_UNSIGNED_SHORT;
}