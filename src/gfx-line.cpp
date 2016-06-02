#include "gfx-line.h"

#include "vertex_attrib.h"
        
GFXShader CreateLineShader()
{
    GFXShader line_shader = GFXShader::Create();
    GFXS::Position3D pos3d;
    pos3d.pos = GFXS::Position();
    line_shader.Transform(pos3d);
    line_shader.Color(GFXS::RGBA());
    line_shader.Compile();
    return line_shader;
}

GFXLine::GFXLine()
{
    static GFXShader static_shader = CreateLineShader();
    shader = static_shader;

    vertex_buffer = GeometryBuffer::Create(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
}

void GFXLine::Render(std::vector<vec3f>& points)
{
    shader.Bind();

    vertex_buffer.Data(&points[0], sizeof(vec3f) * points.size());

    glVertexAttribPointer(VertexAttrib::Position<0>::index(),                       //*attrib index
                          VertexAttrib::Position<0>::elemCount(),                   //*attrib element count
                          type_to_gltype[VertexAttrib::Position<0>::elemType()],    //*element type
                          GL_FALSE,                             //*normalize?
                          0,                          //*stride
                          (void*)0);                       //*offset
    glEnableVertexAttribArray(VertexAttrib::Position<0>::index());
    
    glDrawArrays(GL_LINES, 0, points.size());
}