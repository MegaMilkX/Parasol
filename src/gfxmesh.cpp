#include "gfxmesh.h"

//GL_HALF_FLOAT, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV

std::map<int, int> type_to_gltype = 
{ { TypeInfo<char>::GetId(), GL_BYTE },
  { TypeInfo<unsigned char>::GetId(), GL_UNSIGNED_BYTE },
  { TypeInfo<short>::GetId(), GL_SHORT },
  { TypeInfo<unsigned short>::GetId(), GL_UNSIGNED_SHORT }, 
  { TypeInfo<int>::GetId(), GL_INT }, 
  { TypeInfo<unsigned int>::GetId(), GL_UNSIGNED_INT },
  { TypeInfo<float>::GetId(), GL_FLOAT },
  { TypeInfo<double>::GetId(), GL_DOUBLE } };

GFXMesh GFXMesh::Create(int usage)
{
    GFXMesh mesh;
    glGenVertexArrays(1, &mesh.vao);
    
    return mesh;
}
  
void GFXMesh::SetIndices(std::vector<unsigned short> indices)
{
    if(indices.size() == 0)
        return;
    
    glBindVertexArray(vao);
    index_buffer = GeometryBuffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    index_buffer.Data(indices.data(), sizeof(unsigned short) * indices.size());
    index_count = indices.size();
    index_type = GL_UNSIGNED_SHORT;
}