#ifndef _GFXSHADER_H_
#define _GFXSHADER_H_

#include "glextutil.h"

#include "gfxs_atom.h"

class GFXShader
{
public:
    GFXShader();
    void operator=(GFXS::VertexAtom& atom);
    void operator=(GFXS::PixelAtom& atom);

    bool Compile();
    void Bind();
    std::string StatusString(){ return status_string; }
private:
    unsigned int CompileStage(GFXS::Stage& stage, unsigned int type);

    GFXS::Stage vertex_stage;
    GFXS::Stage fragment_stage;

    unsigned int program;
    std::string status_string;

    std::vector<GFXS::Uniform> uniforms;
};

#endif