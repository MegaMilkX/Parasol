#include "external/json.hpp"

#include "gfxmaterial.h"

GFXMaterial GFXMaterial::Create()
{
    return GFXMaterial();
}

bool GFXMaterial::ReadMAT(File file)
{
    file.Seek(0);
    size_t bytes = file.Size();
    std::string json_str = file.Read<std::string>(bytes);

    nlohmann::json json;
    json = json.parse(json_str);
    
    if (!json.is_object())
        return false;
    
    nlohmann::json::iterator it = json.begin();
    for (it; it != json.end(); ++it)
    {
        if (it.key() == "shader")
        {
            if (it->is_string())
            {
                Shader(Resource<GFXShader>::Get(it->get<std::string>()));
            }
        }
        else if (it.key() == "textures")
        {
            if (it->is_array())
            {
                for (unsigned int i = 0; i < it->size(); ++i)
                {
                    Texture2D(Resource<GFXTexture2D>::Get(it->at(i).get<std::string>()), i);
                }
            }
        }
    }
    return true;
}