#include "gfxshader.h"

#include "vertex_attrib.h"

#include <iostream>

#include "gfx.h"

std::map<int, int> glsl_version =
{ { 200, 110 },
{ 210, 120 },
{ 300, 130 },
{ 310, 140 },
{ 320, 150 },
{ 330, 330 },
{ 400, 400 },
{ 410, 410 },
{ 420, 420 },
{ 430, 430 },
{ 440, 440 },
{ 450, 450 } };

bool GFXShader::ReadXML(File file)
{
    tinyxml2::XMLDocument doc;
    file.Seek(0);
    size_t bytes = file.Size();
    std::string xml = file.Read<std::string>(bytes);
    if (doc.Parse(xml.c_str(), bytes) != tinyxml2::XML_NO_ERROR)
        return false;

    tinyxml2::XMLElement* elem = doc.RootElement();
    if (elem->Name() != std::string("Shader"))
        return false;

    elem = elem->FirstChildElement();
    while (elem)
    {
        if (elem->Name() == std::string("Transform"))
        {
            tinyxml2::XMLElement* e = elem->FirstChildElement();
            if (elem)
            {
                GFXS::Atom* atom = BuildOutputTree(e);
                Transform(*atom);
                delete atom;
            }
            else
                (vertex_stage["gl_Position"]).HideOutputDeclaration(true);
        }
        else if (elem->Name() == std::string("Color"))
        {
            tinyxml2::XMLElement* e = elem->FirstChildElement();
            if (elem)
            {
                GFXS::Atom* atom = BuildOutputTree(e);
                Color(*atom);
                delete atom;
            }
            else
                fragment_stage["frag_out"];
        }

        elem = elem->NextSiblingElement();
    }

    Compile();

    return true;
}

GFXShader GFXShader::Create()
{
    return GFXShader();
}

GFXShader::GFXShader() : program(0), status_string("") {}

void GFXShader::operator=(GFXS::VertexAtom& atom)
{
    (vertex_stage["gl_Position"] = atom).HideOutputDeclaration(true);
}

void GFXShader::operator=(GFXS::PixelAtom& atom)
{
    fragment_stage["frag_out"] = atom;
}

void GFXShader::Transform(GFXS::Atom& atom)
{
    (vertex_stage["gl_Position"] = atom).HideOutputDeclaration(true);
}

void GFXShader::Color(GFXS::Atom& atom)
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

    attrib_names = vertex_stage.GetAttribNames();
    for (unsigned int i = 0; i < attrib_names.size(); ++i)
    {
        // BindAttribLocation to GFXGlobal<TAGAttrib>::Get(attrib_names[i]).DataIndex()
        unsigned int idx = GFXGlobal<TAGAttrib>::Get(attrib_names[i]).DataIndex();
        glBindAttribLocation(program, idx, attrib_names[i].c_str());
    }

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

    for (unsigned int i = 0; i < uniform_names.size(); ++i)
    {
        uniform_locations.push_back(glGetUniformLocation(program, uniform_names[i].c_str()));
    }

    return true;
}

void GFXShader::Bind()
{
    glUseProgram(program);
    
    // Send ALL uniform data to the shader
    // TODO: check if uniform was updated since last time and only then send it
    for (unsigned int i = 0; i < uniforms.size(); ++i)
    {
        uniforms[i]->Uniform(uniform_locations[i]);
    }
}

bool GFXShader::Validate(std::string& error_str)
{
    int result;
    int info_log_len;
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 1)
    {
        std::vector<char> ShaderErrorMessage(info_log_len + 1);
        glGetProgramInfoLog(program, info_log_len, NULL, &ShaderErrorMessage[0]);
        error_str = &ShaderErrorMessage[0];
    }
    return result != 0;
}

unsigned int GFXShader::CompileStage(GFXS::Stage& stage, unsigned int type)
{
    std::string source = stage.Evaluate();
    // TODO: Add shader model deduction on runtime
    int ver = 450;
    if (glsl_version.find(GFXAPIVersion()) != glsl_version.end())
        ver = glsl_version[GFXAPIVersion()];
    source = "#version " + std::to_string(ver) + "\n" + source;
    
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

    std::vector<IGFXGlobal*> unfrms = stage.GetGlobals();
    std::vector<std::string> names = stage.GetGlobalNames();
    uniforms.insert(uniforms.end(), unfrms.begin(), unfrms.end());
    uniform_names.insert(uniform_names.end(), names.begin(), names.end());
    

    return shader;
}

GFXS::Atom* GFXShader::BuildOutputTree(const tinyxml2::XMLElement* elem)
{
    GFXS::Atom* atom = GFXS::Atom::GetPtr(elem->Name());
    if (!atom)
        return 0;

    atom = atom->Clone();

    elem = elem->FirstChildElement();
    int argument_index = 0;
    while (elem != 0)
    {
        GFXS::Atom* arg_atom = BuildOutputTree(elem);
        if (arg_atom)
        {
            const tinyxml2::XMLAttribute* attrib = elem->FirstAttribute();
            while (attrib)
            {
                arg_atom->SetParameter(attrib->Name(), attrib->Value());
                attrib = attrib->Next();
            }
            
            *(atom->ArgumentSlot(argument_index)) = *arg_atom;
            delete arg_atom;
        }
        argument_index++;
        elem = elem->NextSiblingElement();
    }

    return atom;
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