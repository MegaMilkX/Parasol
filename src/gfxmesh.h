#ifndef _GFXMESH_H_
#define _GFXMESH_H_

#include <vector>
#include <map>
#include "glextutil.h"

#include "geometrybuffer.h"

#include "macro\macro_vertex_def.h"

#include "macro\macro_readers_def.h"

extern std::map<int, int> type_to_gltype;

struct Vertex
{
    VERTEX
    (
        (VertexAttrib::Position<0>) position,
        (VertexAttrib::Normal<0>) normal,
        (VertexAttrib::UV<0>) uv
    )
};

class GFXMesh
{
public:
    READERS
    (
        (ReadR3D) "r3d"
    )
    
    bool ReadR3D(File file)
    {
        std::vector<Vertex> vertices =
        {
            {vec3f(-0.5f, -0.5f, 0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(1.0f, 0.0f)},
            {vec3f(0.5f, -0.5f, 0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(0.0f, 0.0f)},
            {vec3f(0.5f, 0.5f, 0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(0.0f, 1.0f)},
            {vec3f(-0.5f, 0.5f, 0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(1.0f, 1.0f)},
            {vec3f(-0.5f, -0.5f, -0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(0.0f, 0.0f)},
            {vec3f(0.5f, -0.5f, -0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(1.0f, 0.0f)},
            {vec3f(0.5f, 0.5f, -0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(1.0f, 1.0f)},
            {vec3f(-0.5f, 0.5f, -0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(0.0f, 1.0f)}
        };
        std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 0, 3, 2, 6, 6, 7, 3, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 0, 1, 5, 5, 4, 0, 1, 5, 6, 6, 2, 1 };
        
        SetVertices(vertices);
        SetIndices(indices);
        
        return true; 
    }
    
    enum
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
    };
    static GFXMesh Create(int usage = STATIC);
    GFXMesh() : vao(0) {}
    template<typename T>
    void SetVertices(std::vector<T> vertices);
    void SetIndices(std::vector<unsigned short> indices);
    void Bind()
    {
        vertex_buffer.Bind();
        index_buffer.Bind();
        size_t offset = 0;
        for (unsigned int i = 0; i < attribs.size(); ++i)
        {
            AttrInfo attrInfo = attribs[i];
            glVertexAttribPointer(attrInfo.index,                       //*attrib index
                                  attrInfo.elemCount,                   //*attrib element count
                                  type_to_gltype[attrInfo.elemType],    //*element type
                                  GL_FALSE,                             //*normalize?
                                  vertex_size,                          //*stride
                                  (void*)offset);                       //*offset
            offset += attrInfo.size;
            glEnableVertexAttribArray(attrInfo.index);
        }
    }
    void Render()
    {
        Bind();
        glDrawElements(GL_TRIANGLES, index_count, index_type, 0);
    }
    
    bool IsValid() { return vao; }
private:
    GLuint vao;
    GeometryBuffer vertex_buffer;
    GeometryBuffer index_buffer;

    std::vector<AttrInfo> attribs;
    size_t vertex_size;

    int index_count;
    unsigned int index_type;
};

template<typename T>
void GFXMesh::SetVertices(std::vector<T> vertices)
{
    if(vertices.size() == 0)
        return;
    
    vertex_size = sizeof(T);
    vertex_buffer = GeometryBuffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertex_buffer.Data(vertices.data(), sizeof(T) * vertices.size());
    size_t offset = 0;
    for(int i = T::attribCount() - 1; i >= 0; --i)
    {
        attribs.push_back(T::getAttrInfo(i));
    }
}

#endif