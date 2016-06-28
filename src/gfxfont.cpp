#include "gfxfont.h"

#include <iostream>

#include "external/lodepng/lodepng.h"

#include "bin_font_nimbusmono_regular.h"

#include "gfx-string.h"

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
		
		shader.Color
		(
			GFXS::MultiplyVec4Vec4
			(
				GFXS::RGBA(),
				GFXS::TextureColor2DRedAlpha
				(
					GFXS::Texture2D(),
					GFXS::UV()
				)
			)
		);
		shader.Compile();
		/*
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
		*/

		//shader.Color(GFXS::Color() * GFXS::TextureColor2D(GFXS::Texture2D(), GFXS::UV()));
		
		
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

GFXFont::Glyph GFXFont::GetGlyph(uint32_t character, int size)
{
	GFXFont::Glyph glyph = { 0 };
	glyph.index = character;
	glyph.size = size;
	std::set<GFXFont::Glyph>::iterator it = glyphs.find(glyph);
	if (it != glyphs.end())
	{
		return *it;
	}
	//FT_Set_Char_Size(face, size * 64, 0, 72, 0);
	FT_Set_Pixel_Sizes(face, 0, size);
	FT_Error fterror;
	if ((fterror = FT_Load_Char(face, character, FT_LOAD_RENDER)))
	{
		std::cout << "Failed to FT_Load_Char: " << fterror << std::endl;
		return glyph;
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
            return glyph;
    }

    glyph.w = face->glyph->bitmap.width;
    glyph.h = face->glyph->bitmap.rows;    
    
    if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
    {    
		unsigned int index = packs[size].Add(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);
		glyph.bitmap_index = index;
		
		atlas_dirty[size] = true;
	}
	
	glyphs.insert(glyph);

    return glyph;
}

void GFXFont::MakeString(GFXString* gfxstring, const std::string& string, unsigned char size, int align)
{
	if (atlases.find(size) == atlases.end())
		atlases[size] = GFXTexture2D::Create();

	GFXMesh& mesh = gfxstring->mesh;
	mesh.Destroy();
	mesh = GFXMesh::Create();
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	float advance_x = 0.0f;
	float advance_y = 0.0f;
	unsigned short index = 0;

	GFXFont::Glyph glyph;
	BitmapPack::Bitmap* bmp;

	for (unsigned int i = 0; i < string.size(); ++i)
		GetGlyph(string[i], size);

	if (atlas_dirty[size])
	{
		BitmapPack::Bitmap atlas_bmp = packs[size].Pack(1);

		GFXTexture2D& texture = atlases[size];
		texture.Bitmap(atlas_bmp.data, atlas_bmp.w, atlas_bmp.h, 1);
		
		atlas_dirty[size] = false;
		
		std::set<GFXString*>::iterator it = strings.begin();
		for (it; it != strings.end(); ++it)
		{
			GFXString* ptr = (*it);
			if (ptr == gfxstring)
				continue;
			ptr->operator=(ptr->value);
		}
	}

	float max_x = 0;
	float max_y = 0;
	float alignment_margin_x = 0.0f;
	float alignment_margin_y = 0.0f;

	for (unsigned int i = 0; i < string.size(); ++i)
	{
		glyph = GetGlyph(string[i], size);
		bmp = packs[size].Get(glyph.bitmap_index);

		if (string[i] == '\n')
		{
			advance_y -= (face->ascender - face->descender) / 64;
			advance_x = 0.0f;
			continue;
		}

		if (!bmp)
			continue;

		GFXTexture2D tex = GetAtlas(size);
		vec2i dim = tex.GetDimensions();

		float uva_x = bmp->uv_left / (float)dim.x;
		float uva_y = bmp->uv_top / (float)dim.y;
		float uvb_x = bmp->uv_right / (float)dim.x;
		float uvb_y = bmp->uv_bottom / (float)dim.y;

		float x0 = advance_x;
		float x1 = advance_x + 1.0f * glyph.w;
		float y0 = advance_y - (glyph.h - glyph.h_bearing_y) - (face->height / 64);
		float y1 = advance_y + 1.0f * glyph.h - (glyph.h - glyph.h_bearing_y) - (face->height / 64);

		if (max_x < x1)
			max_x = x1;
		if (max_y > y1 - (glyph.h - glyph.h_bearing_y))
			max_y = y1 - (glyph.h - glyph.h_bearing_y);

		vertices.insert(vertices.end(),
		{
			{ vec3f(x0,	y0, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f(uva_x, uva_y) },
			{ vec3f(x1, y0, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f(uvb_x, uva_y) },
			{ vec3f(x1, y1, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f(uvb_x, uvb_y) },
			{ vec3f(x0,	y1, 0.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), vec2f(uva_x, uvb_y) }
		});

		indices.push_back(index + 0);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index + 0);

		index += 4;
		advance_x += (int)glyph.advance_x;
	}

	if ((align & (LEFT | RIGHT)) == (LEFT | RIGHT))
	{
		alignment_margin_x = max_x / 2;
	}
	else if ((align & LEFT) == LEFT)
	{
		alignment_margin_x = 0;
	}
	else if ((align & RIGHT) == RIGHT)
	{
		alignment_margin_x = max_x;
	}
	
	if ((align & (TOP | BOTTOM)) == (TOP | BOTTOM))
	{
		alignment_margin_y = max_y / 2;
	}
	else if ((align & TOP) == TOP)
	{
		alignment_margin_y = 0;
	}
	else if ((align & BOTTOM) == BOTTOM)
	{
		alignment_margin_y = max_y;
	}

	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position.value.x -= alignment_margin_x;
		vertices[i].position.value.y -= alignment_margin_y;
	}

	mesh.SetVertices(vertices);
	mesh.SetIndices(indices);
}

void GFXFont::Bind(unsigned char size)
{
	shader.Bind();
	atlases[size].Use(0);
}

void GFXFont::DebugDumpAtlasPNG(unsigned char size, const std::string& filename)
{
	BitmapPack::Bitmap atlas_bmp = packs[size].Pack(1);

	std::vector<unsigned char> raw(atlas_bmp.data, atlas_bmp.data + atlas_bmp.w * atlas_bmp.h * atlas_bmp.bpp);
	std::vector<unsigned char> png;

	unsigned error = lodepng::encode(png, raw, atlas_bmp.w, atlas_bmp.h, LCT_GREY);
	lodepng::save_file(png, filename);
	std::cout << lodepng_error_text(error) << std::endl;
}