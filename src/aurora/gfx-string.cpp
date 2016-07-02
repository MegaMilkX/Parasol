#include "gfx-string.h"

GFXString::GFXString()
	: value(""), size(12), align(LEFT), mesh(GFXMesh::Create(GFXMesh::DYNAMIC))
{
	mesh = GFXMesh::Create();
	font_face->_RegString(this);
}

GFXString::GFXString(const std::string& str)
	: value(str), size(12), align(LEFT), mesh(GFXMesh::Create(GFXMesh::DYNAMIC))
{
	mesh = GFXMesh::Create();
	font_face->_RegString(this);
	value = str;
	MakeMesh();
}

GFXString::~GFXString()
{
	font_face->_UnregString(this);
	mesh.Destroy();
}

GFXString& GFXString::operator=(const std::string& str)
{
	if(!mesh.Valid())
		mesh = GFXMesh::Create();
	value = str;
	MakeMesh();
	return *this;
}

void GFXString::Size(unsigned char size)
{
	this->size = size;
	MakeMesh();
}

void GFXString::Font(const std::string& font_name)
{
	font_face->_UnregString(this);
	font_face = Resource<GFXFont>::Get(font_name, BLOCKING);
	font_face->_RegString(this);
	MakeMesh();
}

void GFXString::Align(int align)
{
	this->align = align;
	MakeMesh();
}

void GFXString::Render()
{
	font_face->Bind(this->size);
	mesh.Render();
}

void GFXString::MakeMesh()
{
	font_face->MakeString(this, value, size, align);
}