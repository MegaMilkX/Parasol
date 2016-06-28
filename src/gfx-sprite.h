#ifndef _GFX_SPRITE_H_
#define _GFX_SPRITE_H_

#include "macro/macro_readers_def.h"

#include "resource.h"

#include "math3f.h"

class GFXMaterial;
class GFXSprite;
class GFXSpriteMap
{
public:
    READERS
    (
        (ReadSPR) "spr"
    )
    bool ReadSPR(File file);

    static GFXSpriteMap Create();

	struct Vertex
	{
		VERTEX
		(
			(VertexAttrib::Position<0>) position,
			(VertexAttrib::RGBA<0>) color,
			(VertexAttrib::UV<0>) uv
		)
	};

	ResHdl<GFXMesh> GetMesh(unsigned int index) 
	{
		if(index >= sprite_meshes.size())
		{
			return sprite_meshes[sprite_meshes.size() - 1];
		}
		return sprite_meshes[index]; 
	}
	ResHdl<GFXMesh> GetMesh(std::string name) 
	{ 
		std::map<std::string, unsigned int>::iterator it = sprite_names.find(name);
		if (it == sprite_names.end())
		{
			return sprite_meshes[sprite_meshes.size() - 1];
		}
		return sprite_meshes[it->second];
	}

	void AddSpriteMesh(const GFXTexture2D& tex, const std::string& name, vec4i rect, vec2i origin);

private:
    ResHdl<GFXTexture2D> texture;
	std::map<std::string, unsigned int> sprite_names;
	std::vector<ResHdl<GFXMesh>> sprite_meshes;
};

class GFXSprite
{
public:
	GFXSprite(std::string sprite_map, std::string sprite);
	GFXSprite(std::string sprite_map, unsigned int id);
	GFXSprite(std::string sprite_map);
	GFXSprite();

	void Sprite(std::string name);
	void Sprite(unsigned int index);

	void Render();
private:
	ResHdl<GFXMaterial> material;
	ResHdl<GFXMesh> mesh;
    vec2f rect;
    vec2f origin;
};

#endif
