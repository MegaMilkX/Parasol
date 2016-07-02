#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "scene.h"
#include "entity.h"

#include <libgame.h>

class Sprite : public IRenderable
{
	ENTITY_BODY
	(
		Sprite, IRenderable,
		CONSTRUCTOR
		(
			global_matrixmodel = GFXGlobal<mat4f>::Get("MatrixModel0");
			render_order = 0;
		)
	)

public:
	void SetSprite(std::string map, std::string sprite)
	{
		this->sprite = GFXSprite(map, sprite);
	}
	void SetSprite(std::string map, unsigned index)
	{
		sprite = GFXSprite(map, index);
	}
	void Order(int order) { render_order = order; }
	void Render()
	{
		global_matrixmodel = node->GetTransform();
		sprite.Render();
	}
	int RenderOrder() { return sprite.Material()->RenderOrder() + render_order; }
	bool AlphaBlended() { return sprite.Material()->AlphaBlend(); }
protected:
	GFXSprite sprite;
	GFXGlobal<mat4f> global_matrixmodel;
	int render_order;
};

#endif