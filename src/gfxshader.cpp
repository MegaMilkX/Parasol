#include "gfxshader.h"

GFXShader::GFXShader() : program(0), status_string("") {}

void GFXShader::operator=(GFXS::VertexAtom& atom)
{
    (vertex_stage["gl_Position"] = atom).HideOutputDeclaration(true);
}

void GFXShader::operator=(GFXS::PixelAtom& atom)
{
    fragment_stage["frag_out"] = atom;
}

bool GFXShader::Compile()
{
    vertex_stage.Link(fragment_stage);
    
    unsigned int v_stage = CompileStage(vertex_stage, GL_VERTEX_SHADER);
    unsigned int f_stage = CompileStage(fragment_stage, GL_FRAGMENT_SHADER);

    if (!v_stage)
        return false;
    if (!f_stage)
        return false;
    
    program = glCreateProgram();
    glAttachShader(program, v_stage);
    glAttachShader(program, f_stage);
    glLinkProgram(program);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetProgramiv(program, GL_LINK_STATUS, &Result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        status_string += &ProgramErrorMessage[0];
    }
    
    return true;
}

void GFXShader::Bind()
{
    glUseProgram(program);
    
    // TODO:
    // Send ALL uniform data to the shader

    for (unsigned int i = 0; i < uniforms.size(); ++i)
    {
        uniforms[i].Update();
    }
}

unsigned int GFXShader::CompileStage(GFXS::Stage& stage, unsigned int type)
{
    std::string source = stage.Evaluate();
    source = "#version 440\n" + source;
    
    unsigned int shader = glCreateShader(type);
    const char* c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, 0);
    glCompileShader(shader);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 1)
    {
        std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shader, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        status_string += &ShaderErrorMessage[0];
    }
    
    return shader;
}