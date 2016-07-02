#ifndef _GFX_STRING_H_
#define _GFX_STRING_H_

#include <string>
#include "resource.h"
#include "gfxfont.h"
#include "gfxmesh.h"

class GFXString
{
public:
	enum ALIGN
	{
		LEFT = 1,
		RIGHT = 2,
		TOP = 4,
		BOTTOM = 8,
		HCENTER = LEFT | RIGHT,
		VCENTER = TOP | BOTTOM
	};

	GFXString();
	GFXString(const std::string& str);
	~GFXString();
	GFXString& operator=(const std::string& str);

	void Size(unsigned char size);
	void Font(const std::string& font_name);
	void Align(int align);

	ResHdl<GFXMaterial> Material() { return material; }

	void Render();

	friend GFXFont;
private:
	void MakeMesh();

	std::string value;
	unsigned char size;
	int align;
	ResHdl<GFXFont> font_face;
	ResHdl<GFXMaterial> material;
	GFXMesh mesh;
};

#endif