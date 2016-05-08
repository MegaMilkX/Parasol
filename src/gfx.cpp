#include "gfx.h"

#include <iostream>

WNDPROC GFXOldWndProc;
HDC deviceContext;
HGLRC context;
int contextVersion = 0;
GFXTarget* rootRenderTarget;

GFXOnWindowResize onResizeCallback = 0;

LRESULT CALLBACK GFXWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_SIZE:
        glViewport(0,0, LOWORD(lParam), HIWORD(lParam));
        //TODO: What if I want to use member functions as callbacks?
        if(onResizeCallback)
            onResizeCallback(LOWORD(lParam), HIWORD(lParam));
    default:
        return CallWindowProc(GFXOldWndProc, hWnd, msg, wParam, lParam);
    }
    //return DefWindowProc(hWnd, msg, wParam, lParam);
    //return CallWindowProc(GFXOldWndProc, hWnd, msg, wParam, lParam);
    return 0;
}

GFXTarget* GFXInit(HWND hWnd)
{
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    deviceContext = GetDC(hWnd);
    
    int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
    
    if(!pixelFormat)
        return 0;
        
    if(!SetPixelFormat(deviceContext, pixelFormat, &pfd))
        return 0;
    
    HGLRC renderingContext = wglCreateContext(deviceContext);
    wglMakeCurrent(deviceContext, renderingContext);
    
    WGLEXTLoadFunctions();
    if(!wglCreateContextAttribsARB)
    {
        context = renderingContext;
    }
    else
    {
        int attr[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 5,
            WGL_CONTEXT_FLAGS_ARB, 0,
            0
        };
        
        HGLRC renderingContext3plus = wglCreateContextAttribsARB(deviceContext, 0, attr);
        
        if(!renderingContext3plus)
        {
            context = renderingContext;
        }
        else
        {
            context = renderingContext3plus;
            wglMakeCurrent(NULL,NULL);
            wglDeleteContext(renderingContext);
            wglMakeCurrent(deviceContext, context);
        }
    }
    
    GLEXTLoadFunctions();
    
    int version[2];
    glGetIntegerv(GL_MAJOR_VERSION, &version[0]);
    glGetIntegerv(GL_MINOR_VERSION, &version[1]);
    contextVersion = version[0] * 100 + version[1] * 10;
    rootRenderTarget = GFXTarget::Create(); //Default OpenGL framebuffer is 0 and we can't change it
                                                //We don't actually need HWND for this, whatever
    
    std::cout << "OpenGL v" << GFXVersion() << " ready.\n";
    
    std::cout << "GLSL v" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    
    //=======================================================
    glClearColor (0.2f, 0.2f, 0.2f, 0.0f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    GFXOldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)GFXWndProc);
    if(!GFXOldWndProc)
        return false;
    
    return rootRenderTarget;
}

void GFXCleanup()
{
    //TODO: Get rid of rootRenderTarget
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(context);
}

void GFXSetWindowResizeCallback(GFXOnWindowResize callback)
{
    onResizeCallback = callback;
}

void GFXSwapBuffers()
{
    SwapBuffers(deviceContext);
};

int GFXVersion()
{
    return contextVersion;
}
