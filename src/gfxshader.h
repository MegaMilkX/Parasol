#ifndef _GFXSHADER_H_
#define _GFXSHADER_H_

#include "external/tinyxml2.h"

#include "macro\macro_readers_def.h"

#include "gfxs_atom.h"

class GFXShader
{
public:
    READERS
    (
        (ReadXML) "xml"
    );
    bool ReadXML(File file);

    static GFXShader Create();

    GFXShader();
    void operator=(GFXS::VertexAtom& atom);
    void operator=(GFXS::PixelAtom& atom);

    void Transform(GFXS::Atom& atom);
    void Color(GFXS::Atom& atom);

    bool Compile();
    void Bind();
    bool Validate(std::string& error_str);
    std::string StatusString(){ return status_string; }

    void Uniform(std::string& name, float value);
    void Uniform(std::string& name, vec2f& value);
    void Uniform(std::string& name, vec3f& value);
    void Uniform(std::string& name, vec4f& value);
    void Uniform(std::string& name, int value);
    void Uniform(std::string& name, vec2i& value);
    void Uniform(std::string& name, vec3i& value);
    void Uniform(std::string& name, vec4i& value);
    void Uniform(std::string& name, unsigned int value);
    void Uniform(std::string& name, vec2ui& value);
    void Uniform(std::string& name, vec3ui& value);
    void Uniform(std::string& name, vec4ui& value);
    void Uniform(std::string& name, mat3f& value);
    void Uniform(std::string& name, mat4f& value);
private:
    unsigned int CompileStage(GFXS::Stage& stage, unsigned int type);

    GFXS::Atom* BuildOutputTree(const tinyxml2::XMLElement* elem);

    GFXS::Stage vertex_stage;
    GFXS::Stage fragment_stage;

    unsigned int program;
    std::string status_string;

    std::vector<std::string> attrib_names;
    std::vector<std::string> uniform_names;
    std::vector<IGFXGlobal*> uniforms;
    std::vector<unsigned int> uniform_locations;
};

#endif