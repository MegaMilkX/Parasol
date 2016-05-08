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

void GFXShader::Uniform(std::string& name, float value) { glUniform1f(glGetUniformLocation(program, name.c_str()), value); }
void GFXShader::Uniform(std::string& name, vec2f& value) { glUniform2f(glGetUniformLocation(program, name.c_str()), value.x, value.y); }
void GFXShader::Uniform(std::string& name, vec3f& value) { glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z); }
void GFXShader::Uniform(std::string& name, vec4f& value) { glUniform4f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w); }
void GFXShader::Uniform(std::string& name, int value) { glUniform1i(glGetUniformLocation(program, name.c_str()), value); }
void GFXShader::Uniform(std::string& name, vec2i& value) { glUniform2i(glGetUniformLocation(program, name.c_str()), value.x, value.y); }
void GFXShader::Uniform(std::string& name, vec3i& value) { glUniform3i(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z); }
void GFXShader::Uniform(std::string& name, vec4i& value) { glUniform4i(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w); }
void GFXShader::Uniform(std::string& name, unsigned int value) { glUniform1ui(glGetUniformLocation(program, name.c_str()), value); }
void GFXShader::Uniform(std::string& name, vec2ui& value) { glUniform2ui(glGetUniformLocation(program, name.c_str()), value.x, value.y); }
void GFXShader::Uniform(std::string& name, vec3ui& value) { glUniform3ui(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z); }
void GFXShader::Uniform(std::string& name, vec4ui& value) { glUniform4ui(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w); }
void GFXShader::Uniform(std::string& name, mat3f& value) { glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, (float*)&value); }
void GFXShader::Uniform(std::string& name, mat4f& value) { glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, (float*)&value); }