#include "gfxfont.h"

#include <iostream>

#include "external/lodepng/lodepng.h"

#include "bin_font_nimbusmono_regular.h"

bool GFXFont::ReadTTF(File file)
{
    file.Seek(0, File::BEGIN);
    unsigned int bytesRead;
    unsigned char* data = file.Read(file.Size(), bytesRead);

    if (!data || !bytesRead)
        return false;

	unsigned char* font_data = new unsigned char[bytesRead];
	memcpy(font_data, data, bytesRead);

    if (FT_Init_FreeType(&ftlib))
        return false;
    if (FT_New_Memory_Face(ftlib, (FT_Byte*)font_data, bytesRead, 0, &face))
        return false;

    return true;
}

FT_Face GFXFont::_LoadDefaultFace()
{
	FT_Error error;

	FT_Library lib;
	error = FT_Init_FreeType(&lib);

	FT_Face face;
	if (FT_New_Memory_Face(lib, nimbusmono_regular_otf, sizeof(nimbusmono_regular_otf), 0, &face))
	{
		return 0;
	}
	return face;
}

GFXShader GFXFont::_GetDefaultShader()
{
	static bool first_time = true;
	static GFXShader shader;
	if (first_time)
	{
		shader = GFXShader::Create();
		GFXS::Position3D pos3d;
		pos3d.pos = GFXS::Position();
		shader.Transform(pos3d);
		GFXS::TextureColor2D texcol2d;
		texcol2d.texture_sampler = GFXS::Texture2D();
		texcol2d.uv = GFXS::UV();
		shader.Color(texcol2d);
		shader.Compile();

		shader.Color
		(
			GFXS::Vector4
			(
				GFXS::Color().X(), 
				GFXS::Color().Y(), 
				GFXS::Color().Z(), 
				GFXS::TextureColor2D
				(
					GFXS::Texture2D(), 
					GFXS::UV()
				).X()
			)
		);
		
		first_time = false;
	}
	return shader;
}

GFXFont GFXFont::Create()
{
	GFXFont f;
	f.shader = _GetDefaultShader();
    return f;
}

GFXFont::GFXFont()
{
	static FT_Face f = _LoadDefaultFace();
	face = f;
}

GFXFont::Glyph* GFXFont::GetGlyph(uint32_t character, int size)
{
	GFXFont::Glyph glyph = { 0 };
	//FT_Set_Pixel_Sizes(face, 0, size);
	FT_Set_Char_Size(face, size * 64, 0, 100, 0);

	glyph.index = character;
	glyph.size = size;
	std::map<uint32_t, GFXFont::Glyph>::iterator it = glyphs.find(character);
	if (it != glyphs.end())
		return &(it->second);

	FT_Error fterror;
	if ((fterror = FT_Load_Char(face, character, FT_LOAD_RENDER)))
	{
		std::cout << "Failed to FT_Load_Char: " << fterror << std::endl;
		return 0;
	}

    glyph.advance_x = face->glyph->advance.x / 64;
    glyph.advance_y = face->glyph->advance.y / 64;
    glyph.h_bearing_x = face->glyph->metrics.horiBearingX / 64;
    glyph.h_bearing_y = face->glyph->metrics.horiBearingY / 64;
    glyph.v_bearing_x = face->glyph->metrics.vertBearingX / 64;
    glyph.v_bearing_y = face->glyph->metrics.vertBearingY / 64;

    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
    {
        std::cout << "FT_GLYPH_FORMAT_BITMAP" << std::endl;
        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
            return 0;
    }

    glyph.w = face->glyph->bitmap.width;
    glyph.h = face->glyph->bitmap.rows;    
    
    if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
    {    
		unsigned int index = pack.Add(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);
		glyph.bitmap_index = index;
		/*
		BitmapPack::Bitmap atlas_bmp = pack.Pack(1);
		
		BitmapPack::Bitmap* bmp = pack.Get(index);
		
		glyph.uv_begin.x = bmp->uv_left;
		glyph.uv_begin.y = bmp->uv_top;
		glyph.uv_end.x = bmp->uv_right;
		glyph.uv_end.y = bmp->uv_bottom;

		GFXTexture2D& texture = atlases[size];
		texture.Bitmap(atlas_bmp.data, atlas_bmp.w, atlas_bmp.h, atlas_bmp.bpp);*/
	}
	
	glyphs.insert(std::make_pair(character, glyph));

    return &(glyphs[character]);
}

GFXMesh GFXFont::MakeString(const std::string& string, unsigned char size)
{
	GFXMesh mesh = GFXMesh::Create();

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	float advance_x = 0.0f;
	float advance_y = 0.0f;
	unsigned short index = 0;

	GFXFont::Glyph* glyph;
	BitmapPack::Bitmap* bmp;

	for (unsigned int i = 0; i < string.size(); ++i)
		GetGlyph(string[i], size);

	BitmapPack::Bitmap atlas_bmp = pack.Pack(1);

	GFXTexture2D& texture = atlases[size];
	texture.Bitmap(atlas_bmp.data, atlas_bmp.w, atlas_bmp.h, 1);

	for (unsigned int i = 0; i < string.size(); ++i)
	{
		glyph = GetGlyph(string[i], size);
		bmp = pack.Get(glyph->bitmap_index);

		if (!glyph)
			continue;
		if (!bmp)
			continue;

		GFXTexture2D tex = GetAtlas(size);
		vec2i dim = tex.GetDimensions();

		float uva_x = bmp->uv_left / (float)dim.x;
		float uva_y = bmp->uv_top / (float)dim.y;
		float uvb_x = bmp->uv_right / (float)dim.x;
		float uvb_y = bmp->uv_bottom / (float)dim.y;

		vertices.insert(vertices.end(),
		{
			{ vec3f(advance_x + 0.0f,			advance_y + 0.0f - (glyph->h - glyph->h_bearing_y), 0.0f), vec3f(0.5f, 0.1f, 0.1f), vec2f(uva_x, uva_y) },
			{ vec3f(advance_x + 1.0f * glyph->w, advance_y + 0.0f - (glyph->h - glyph->h_bearing_y), 0.0f), vec3f(0.5f, 0.3f, 0.1f), vec2f(uvb_x, uva_y) },
			{ vec3f(advance_x + 1.0f * glyph->w, advance_y + 1.0f * glyph->h - (glyph->h - glyph->h_bearing_y), 0.0f), vec3f(0.5f, 0.1f, 0.1f), vec2f(uvb_x, uvb_y) },
			{ vec3f(advance_x + 0.0f,			advance_y + 1.0f * glyph->h - (glyph->h - glyph->h_bearing_y), 0.0f), vec3f(0.5f, 0.3f, 0.1f), vec2f(uva_x, uvb_y) }
		});

		indices.push_back(index + 0);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index + 0);

		index += 4;
		advance_x += glyph->advance_x;
		//advance_y += glyph->h + glyph->advance_y;
	}

	mesh.SetVertices(vertices);
	mesh.SetIndices(indices);

	return mesh;
}

void GFXFont::Bind(unsigned char size)
{
	shader.Bind();
	atlases[size].Use(0);
}

void GFXFont::DebugDumpAtlasPNG()
{
	BitmapPack::Bitmap atlas_bmp = pack.Pack(1);

	std::vector<unsigned char> raw(atlas_bmp.data, atlas_bmp.data + atlas_bmp.w * atlas_bmp.h * atlas_bmp.bpp);
	std::vector<unsigned char> png;

	unsigned error = lodepng::encode(png, raw, atlas_bmp.w, atlas_bmp.h, LCT_GREY);
	lodepng::save_file(png, "test.png");
	std::cout << lodepng_error_text(error) << std::endl;
}