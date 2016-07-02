#include "gfx-sprite.h"

#include "gfxmaterial.h"

#include "util/string.h"

bool GFXSpriteMap::ReadSPR(File file)
{
	unsigned int sz = file.Size();
	std::string text = file.Read<std::string>(sz);

	std::vector<std::string> lines = split(text, '\n');

	ResHdl<GFXTexture2D> tex2d = Resource<GFXTexture2D>::Get(file.Name(), BLOCKING);
	vec2i tex_size = tex2d->GetDimensions();

	for (unsigned int i = 0; i < lines.size(); ++i)
	{		
		size_t first_op_bracket = lines[i].find_first_of('[');
		size_t first_cl_bracket = lines[i].find_first_of(']');
		size_t secnd_op_bracket = lines[i].find_last_of('[');
		size_t secnd_cl_bracket = lines[i].find_last_of(']');

		if (first_op_bracket == lines[i].npos ||
			first_cl_bracket == lines[i].npos ||
			secnd_op_bracket == lines[i].npos ||
			secnd_cl_bracket == lines[i].npos)
			continue;

		std::string spr_name = lines[i].substr(0, first_op_bracket);
		spr_name = remove_all_of(spr_name, ' ');

		std::string dim = lines[i].substr(first_op_bracket + 1, first_cl_bracket - first_op_bracket - 1);
		dim = remove_all_of(dim, ' ');

		std::string orig = lines[i].substr(secnd_op_bracket + 1, secnd_cl_bracket - secnd_op_bracket - 1);
		orig = remove_all_of(orig, ' ');

		std::vector<std::string> dim_vec = split(dim, ',');
		std::vector<std::string> orig_vec = split(orig, ',');

		if (dim_vec.size() != 4)
			continue;
		if (orig_vec.size() != 2)
			continue;

		int x0 = 0;
		int y0 = 0;
		int x1 = 0;
		int y1 = 0;
		int ox = 0;
		int oy = 0;

		try
		{
			x0 = std::stoi(dim_vec[0]);
			y0 = std::stoi(dim_vec[1]);
			x1 = std::stoi(dim_vec[2]);
			y1 = std::stoi(dim_vec[3]);
			ox = std::stoi(orig_vec[0]);
			oy = std::stoi(orig_vec[1]);
		}
		catch (...) { continue; }

		AddSpriteMesh(*tex2d, spr_name, vec4i(x0, y0, x1, y1), vec2i(ox, oy));
	}

	return true;
}

GFXSpriteMap GFXSpriteMap::Create()
{
    return GFXSpriteMap();
}

void GFXSpriteMap::AddSpriteMesh(const GFXTexture2D& tex, const std::string& name, vec4i rect, vec2i origin)
{
	vec2i tex_size = tex.GetDimensions();

	GFXMesh mesh = GFXMesh::Create(GFXMesh::STATIC);
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	vec4f r((float)rect.x, (float)rect.y, (float)rect.z, (float)rect.w);
	vec2f o((float)origin.x, (float)origin.y);

	vertices.insert(vertices.end(),
	{
		{ vec3f(-o.x,	-o.y, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f((r.x) / (float)tex_size.x, (r.y) / (float)tex_size.y) },
		{ vec3f(r.z - o.x, -o.y, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f((r.z + r.x) / (float)tex_size.x, (r.y) / (float)tex_size.y) },
		{ vec3f(r.z - o.x, r.w - o.y, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f((r.z + r.x) / (float)tex_size.x, (r.w + r.y) / (float)tex_size.y) },
		{ vec3f(-o.x,	r.w - o.y, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f((r.x) / (float)tex_size.x, (r.w + r.y) / (float)tex_size.y) }
	});

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	mesh.SetVertices(vertices);
	mesh.SetIndices(indices);

	sprite_names.insert(std::make_pair(name, sprite_meshes.size()));
	sprite_meshes.push_back(ResHdl<GFXMesh>::Create(mesh));
}

//===============================================

GFXSprite::GFXSprite(std::string sprite_map, std::string sprite)
{
	material = Resource<GFXMaterial>::Get(sprite_map, BLOCKING);
	ResHdl<GFXTexture2D> tex2d = material->Texture2D(0);
	ResHdl<GFXSpriteMap> spr_map = tex2d->SpriteMap();
	mesh = spr_map->GetMesh(sprite);
}

GFXSprite::GFXSprite(std::string sprite_map, unsigned int id)
{
	material = Resource<GFXMaterial>::Get(sprite_map, BLOCKING);
	ResHdl<GFXTexture2D> tex2d = material->Texture2D(0);
	ResHdl<GFXSpriteMap> spr_map = tex2d->SpriteMap();
	mesh = spr_map->GetMesh(id);
}

GFXSprite::GFXSprite(std::string sprite_map)
{
	material = Resource<GFXMaterial>::Get(sprite_map, BLOCKING);
	ResHdl<GFXTexture2D> tex2d = material->Texture2D(0);
	ResHdl<GFXSpriteMap> spr_map = tex2d->SpriteMap();
	mesh = spr_map->GetMesh(0);
}

GFXSprite::GFXSprite()
{

}

void GFXSprite::Sprite(std::string name)
{
	ResHdl<GFXTexture2D> tex2d = material->Texture2D(0);
	ResHdl<GFXSpriteMap> spr_map = tex2d->SpriteMap();
	mesh = spr_map->GetMesh(name);
}

void GFXSprite::Sprite(unsigned int index)
{
	ResHdl<GFXTexture2D> tex2d = material->Texture2D(0);
	ResHdl<GFXSpriteMap> spr_map = tex2d->SpriteMap();
	mesh = spr_map->GetMesh(index);
}

void GFXSprite::Render()
{
	material->Bind();
	mesh->Render();
}