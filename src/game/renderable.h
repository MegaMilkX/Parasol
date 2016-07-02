#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "scene.h"
#include "entity.h"

#include <libgame.h>

#include <iostream>

DEF_ENTITY
(
	IRenderable,
	Entity,
	CONSTRUCTOR(),
	PUBLIC
	(
		virtual void Render() {};
		virtual int RenderOrder() { return 0; }
		virtual bool AlphaBlended() { return false; }
	),
	PROTECTED()
);

DEF_ENTITY
(
    Renderable,
	IRenderable,
    CONSTRUCTOR
    (
        global_matrixmodel = GFXGlobal<mat4f>::Get("MatrixModel0");
    ),
    PUBLIC
    (
        void Mesh(ResHdl<GFXMesh> mesh) { this->mesh = mesh; }
        void Material(ResHdl<GFXMaterial> material) { this->material = material; }

        virtual void Render()
        {
            global_matrixmodel = node->GetTransform();
            material->Bind();
            mesh->Render();
        }

		int RenderOrder() { return material->RenderOrder(); }
		bool AlphaBlended() { return material->AlphaBlend(); }
    ),
    PROTECTED
    (
        ResHdl<GFXMesh> mesh;
        ResHdl<GFXMaterial> material;
        GFXGlobal<mat4f> global_matrixmodel;
    )
);

#endif