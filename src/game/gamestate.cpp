#include "GameState.h"

std::stack<GameState*> GameState::state_stack;
Window GameState::window;
std::map<int, GameState*> GameState::state_cache;

DebugOverlay GameState::debug_overlay;

struct Vert
{
    VERTEX
    (
        (VertexAttrib::Position<0>) position,
        (VertexAttrib::Normal<0>) normal,
        (VertexAttrib::UV<0>) uv
    )
};

bool GameState::Init()
{
    window = Window::Create(L"game", 1280, 720);
    
    GFXTarget* gfxTarget = GFXInit(window.GetHandle());
    if(!gfxTarget)
    {
        std::cout << "GFX initialization fucked up\n";
        return false;
    }

    InputInit(window.GetHandle(), GameState::PostInput);
    
    Resource<GFXMesh>::AddSearchPath("data\\meshes");
    Resource<GFXShader>::AddSearchPath("data\\shaders");
    Resource<GFXMaterial>::AddSearchPath("data\\materials");
    Resource<GFXTexture2D>::AddSearchPath("data\\textures");
    Resource<GFXFont>::AddSearchPath("data\\fonts");

    //===================
    // Fallback resources
    //===================
    GFXMesh mesh = GFXMesh::Create();
    std::vector<Vert> vertices =
    {
        { vec3f(-0.5f, -0.5f, 0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(1.0f, 0.0f) },
        { vec3f(0.5f, -0.5f, 0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(0.0f, 0.0f) },
        { vec3f(0.5f, 0.5f, 0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(0.0f, 1.0f) },
        { vec3f(-0.5f, 0.5f, 0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(1.0f, 1.0f) },
        { vec3f(-0.5f, -0.5f, -0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(0.0f, 0.0f) },
        { vec3f(0.5f, -0.5f, -0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(1.0f, 0.0f) },
        { vec3f(0.5f, 0.5f, -0.5f), vec3f(0.5f, 0.3f, 0.1f), vec2f(1.0f, 1.0f) },
        { vec3f(-0.5f, 0.5f, -0.5f), vec3f(0.5f, 0.1f, 0.1f), vec2f(0.0f, 1.0f) }
    };
    std::vector<unsigned short> indices = { 0, 1, 2, 2, 3, 0, 3, 2, 6, 6, 7, 3, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 0, 5, 1, 5, 0, 4, 1, 5, 6, 6, 2, 1 };
    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);
    Resource<GFXMesh>::SetFallbackData(mesh);

    GFXShader shader = GFXShader::Create();
    GFXS::Position3D pos3d;
    pos3d.pos = GFXS::Position();
    shader.Transform(pos3d);
    GFXS::TextureColor2D texcol2d;
    texcol2d.texture_sampler = GFXS::Texture2D();
    texcol2d.uv = GFXS::UV();
    shader.Color(texcol2d);
    shader.Compile();
    Resource<GFXShader>::SetFallbackData(shader);

    GFXMaterial material = GFXMaterial::Create();
    material.Texture2D(ResHdl<GFXTexture2D>(), 0);
    material.Shader(ResHdl<GFXShader>());
    Resource<GFXMaterial>::SetFallbackData(material);

    GFXTexture2D texture2d = GFXTexture2D::Create();

    unsigned char* data = new unsigned char[4 * 4 * 3];
    int divider = 2;
    int div_mod = 0;
    for (unsigned int i = 0; i < 4 * 4; ++i)
    {
        
        divider = 2;
        if (i % divider == 0)
        {
            if (i / 4 % 2 == 0)
            {data[i * 3 + 0] = 0; data[i * 3 + 1] = 0; data[i * 3 + 2] = 0;}
            else { data[i * 3 + 0] = 255; data[i * 3 + 1] = 0; data[i * 3 + 2] = 255; }
        }
        else
        {
            if (i / 4 % 2 == 0)
            {data[i * 3 + 0] = 255; data[i * 3 + 1] = 0; data[i * 3 + 2] = 255;}
            else { data[i * 3 + 0] = 0; data[i * 3 + 1] = 0; data[i * 3 + 2] = 0; }
        }
    }
    texture2d.Bitmap(data, 4, 4, 3);
    delete[] data;
    Resource<GFXTexture2D>::SetFallbackData(texture2d);

	GFXFont font = GFXFont::Create();
	Resource<GFXFont>::SetFallbackData(font);
    //===================
    
	debug_overlay.Init();

    return true;
}

void GameState::Cleanup()
{
    ResourceAsync::Cleanup();
    InputCleanup();
    GFXCleanup();
}

void GameState::Pop()
{
	GameState* state = state_stack.top();
    state_stack.pop();
    if(state_stack.size()>0)
        state_stack.top()->OnSwitch();
}

bool GameState::Update()
{
	debug_overlay.PerfCountBegin();

    state_stack.top()->OnUpdate();
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    state_stack.top()->OnRender();
    
	debug_overlay.Render();
	
    GFXSwapBuffers();

	debug_overlay.PerfCountEnd();

    return window.Update();
}

void GameState::PostInput(InputEvent& e)
{
	state_stack.top()->OnInput(e);
}