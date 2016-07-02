#include "gstest.h"

void GSTest::OnInit()
{
    start_time = GetTickCount();

    GFXGlobal<int>::Get("Texture2D0") = 0;

    camera = scene.Add<Camera>();
    camera->Translate(0.0f, 0.0f, 2.7f);
    camera->Rotate(0.7f, vec3f(0.0f, 1.0f, 0.0f));
    camera->Perspective(1.5f, 1280, 720, 0.1f, 100.0f);

    camera2d = scene2d.Add<Camera>();
	camera2d->Translate(0.0f, 0.0f, 1.0f);
	camera2d->Ortho(0.0f, 1280, 0.0f, 720, 0.01f, 100.0f);

    cube = scene.Add<Renderable>();
    cube->Mesh(Resource<GFXMesh>::Get("cube", BLOCKING));
    cube->Material(Resource<GFXMaterial>::Get("material", BLOCKING));
    cube->Translate(-0.3f, 0.0f, 0.0f);

    cube2 = cube->GetNode()->AddNode()->Add<Renderable>();
    cube2->Translate(-1.4f, 0.0f, 0.0f);
    cube2->Material(Resource<GFXMaterial>::Get("material3", BLOCKING));

    BSphere* s = cube->GetNode()->Add<BSphere>();
    s->Radius(0.7f);

    BBox* box = cube2->GetNode()->Add<BBox>();
    box->Size(1.0f, 1.0f, 1.0f);

    LightOmni* omni = scene.Add<LightOmni>();

	Sprite* spr = scene2d.Add<Sprite>();
	spr->SetSprite("wallpaper", 0);
	
	sprite = scene2d.Add<Sprite>();
	sprite->SetSprite("pika", 0);
	sprite->Translate(950, 100, 0);

	TextRenderable* paragraph = scene2d.Add<TextRenderable>();	
	paragraph->Font("tahoma");
	paragraph->Text(R"(Three Rings for the Elven-kings under the sky,
Seven for the Dwarf-lords in halls of stone,
Nine for Mortal Men, doomed to die,
One for the Dark Lord on his dark throne
In the Land of Mordor where the Shadows lie.

One Ring to rule them all, One Ring to find them,
One Ring to bring them all and in the darkness bind them.
In the Land of Mordor where the Shadows lie.)");
	//paragraph->Text("Discard your hand. Draw seven cards, then discard three cards at random.");
	paragraph->Size(20);
	paragraph->Align(GFXString::VCENTER | GFXString::HCENTER);
	paragraph->Translate(640.0f, 360.0f, 0.0f);
	paragraph->Order(1);
}

void GSTest::OnSwitch()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    InputShowCursor(false);
}

void GSTest::OnUpdate()
{
    time = GetTickCount();
    cube->Rotate(0.01f, vec3f(0.0f, 1.0f, 0.0f));
    cube2->Rotate(0.01f, vec3f(1.0f, 0.0f, 0.0f));

    if (InputKB::Key(0x57))
    {
        camera->Translate(camera->Forward() * 0.05f, Space::WORLD);
    }
    if (InputKB::Key(0x41))
    {
        camera->Translate(camera->Left() * 0.05f, Space::WORLD);
    }
    if (InputKB::Key(0x53))
    {
        camera->Translate(camera->Back() * 0.05f, Space::WORLD);
    }
    if (InputKB::Key(0x44))
    {
        camera->Translate(camera->Right() * 0.05f, Space::WORLD);
    }

    if (GetActiveWindow() == window.GetHandle())
    {
        RECT rect;
        GetWindowRect(window.GetHandle(), &rect);
        int x = rect.left, y = rect.top;
        GetClientRect(window.GetHandle(), &rect);
        SetCursorPos(rect.right / 2 + rect.left + x, rect.bottom / 2 + rect.top + y);
    }

    /*
    if (ResourceQueueCount() == 0)
    {
		GameState::Pop();
    }*/
}

void GSTest::OnRender()
{	
	camera2d->Render();
    camera->Render();
	
	
	//
	/*
	GFXGlobal<mat4f>::Get("MatrixModel0") = mat4f(1.0f);

    Node* node = 0;
    vec3f point;
    node = scene.RayTest(camera->ScreenToWorld(vec2f(InputMouse::GetXNorm(), InputMouse::GetYNorm())), point);
    if (node)
    {
        GFXGlobal<mat4f>::Get("MatrixView0") = inverse(camera->GetNode()->GetTransform());
        std::vector<vec3f> points;
        points.push_back(point - vec3f(0.5f, 0.0f, 0.0f));
        points.push_back(point + vec3f(0.5f, 0.0f, 0.0f));
        points.push_back(point - vec3f(0.0f, 0.5f, 0.0f));
        points.push_back(point + vec3f(0.0f, 0.5f, 0.0f));
        points.push_back(point - vec3f(0.0f, 0.0f, 0.5f));
        points.push_back(point + vec3f(0.0f, 0.0f, 0.5f));
        line.Render(points);
    }
	*/
}

void GSTest::OnInput(InputEvent& e)
{
	if (e.Event() == InputEvent::MOUSEMOVE)
	{
		camera->Rotate(InputMouse::GetXRel() * -0.005f, vec3f(0.0f, 1.0f, 0.0f), Space::WORLD);
		camera->Rotate(InputMouse::GetYRel() * -0.005f, vec3f(1.0f, 0.0f, 0.0f), Space::LOCAL);
	}
	else if (e.Event() == InputEvent::DOWN)
	{
		if (e.KeyCode() == 0x01)
		{
			std::cout << "LOL" << std::endl;
		}
		else if (e.KeyCode() == 0x31)
			sprite->SetSprite("pika", 0);
		else if (e.KeyCode() == 0x32)
			sprite->SetSprite("pika", 1);
		else if (e.KeyCode() == 0x33)
			sprite->SetSprite("pika", 2);
		else if (e.KeyCode() == 0x34)
			sprite->SetSprite("pika", 3);
	}
	else if (e.Event() == InputEvent::UP)
	{
		if (e.KeyCode() == VK_SPACE)
		{
			vec3f pt;
			Node* node = scene.RayTest(ray(camera->Position(), camera->Forward()), pt);
			if (node)
			{
				Renderable* r = node->AddNode()->Add<Renderable>();
				r->Material(Resource<GFXMaterial>::Get("material2"));
				r->Position(pt, WORLD);
				r->Rotation(node->Rotation(), LOCAL);
				r->Scale(0.1f);
			}
		}
		
	}
}

void GSTest::OnCleanup()
{

}
