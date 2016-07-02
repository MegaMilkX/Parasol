#ifndef _GUI_H_
#define _GUI_H_

#include "scene.h"
#include "camera.h"

DEF_ENTITY
(
    GUIElement,
    Entity,
    CONSTRUCTOR(),
    PUBLIC(),
    PROTECTED()
);

DEF_ENTITY
(
    GUIImage,
    GUIElement,
    CONSTRUCTOR(),
    PUBLIC(),
    PROTECTED()
);

class GUI
{
public:
    void Init()
    {
        cam = scene.Add<Camera>();
        cam->Ortho(0, 100, 0, 100, 0.1f, 100.0f);
        
        mesh = GFXMesh::Create(GFXMesh::DYNAMIC);
        std::vector<Vertex> verts = 
        {
            { vec3f(0.0f, 0.0f, -1.0f), vec3f(0.5f, 0.1f, 0.1f), vec2f(1.0f, 0.0f) },
            { vec3f(100.0f, 0.0f, -1.0f), vec3f(0.5f, 0.3f, 0.1f), vec2f(0.0f, 0.0f) },
            { vec3f(100.0f, 100.0f, -1.0f), vec3f(0.5f, 0.1f, 0.1f), vec2f(0.0f, 1.0f) },
            { vec3f(0.0f, 100.0f, -1.0f), vec3f(0.5f, 0.3f, 0.1f), vec2f(1.0f, 1.0f) },
        };
        std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 0 };
        mesh.SetVertices(verts);
        mesh.SetIndices(indices);

        material.Shader(Resource<GFXShader>::Get("shader"));
        material.Texture2D(Resource<GFXTexture2D>::Get("250px-162Furret"));
    }
    void Render()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        cam->Render();
        GFXGlobal<mat4f>::Get("MatrixModel0") = mat4f(1.0f);
        material.Bind();
        mesh.Render();
    }
    template<typename GUIELEM>
    GUIELEM* AddElement()
    {
        return scene.Add<GUIELEM>();
    }
private:
    Scene scene;
    Camera* cam;

    GFXMesh mesh;
    GFXMaterial material;
};

#endif