#ifndef _GFXMATERIAL_H_
#define _GFXMATERIAL_H_

#include "macro/macro_readers_def.h"

#include "gfxshader.h"
#include "gfxtexture2d.h"

#include "resource.h"

class GFXMaterial
{
public:
    READERS
    (
        (ReadMAT) "mat"
    );
    bool ReadMAT(File file);

    static GFXMaterial Create();

    struct IValue
    {
        std::string name;
        virtual void SetGlobal() = 0;
    };

    template<typename T>
    struct Value : public IValue
    {
        Value(const std::string& name) : global(GFXGlobal<T>::Get(name)) {}
        T value;
        GFXGlobal<T> global;
        void SetGlobal()
        {
            global = value;
        }
    };

    template<typename T>
    void Set(const std::string& name, T value);

    void Shader(ResHdl<GFXShader> shader) { this->shader = shader; }
    void Texture2D(ResHdl<GFXTexture2D> texture, unsigned short layer = 0)
    {
        if (textures.size() <= layer)
            textures.resize(layer + 1);
        textures[layer] = texture;
    }

    void Bind()
    {
        for (unsigned int i = 0; i < textures.size(); ++i)
            textures[i]->Use(i);
        for (unsigned int i = 0; i < values.size(); ++i)
            values[i]->SetGlobal();
        shader->Bind();
    }
private:
    ResHdl<GFXShader> shader;
    std::vector<ResHdl<GFXTexture2D>> textures;
    std::vector<IValue*> values;
};

template<typename T>
void GFXMaterial::Set(const std::string& name, T value)
{
    Value<T>* val = new Value<T>(name);
    val->value = value;
    val->name = name;
    val->global = GFXGlobal<T>::Get(name);
    values.push_back(val);
}

#endif