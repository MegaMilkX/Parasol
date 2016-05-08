#ifndef _GFX_H_
#define _GFX_H_

#include <windows.h>
#include "glextutil.h"

#include "gfxshader.h"
#include "gfxtexture2d.h"
#include "gfxmesh.h"
#include "gfxtarget.h"

typedef void (*GFXOnWindowResize)(int, int);

GFXTarget*  GFXInit(HWND hWnd);
void        GFXCleanup();
void        GFXSetWindowResizeCallback(GFXOnWindowResize callback);

void        GFXSwapBuffers();

int         GFXVersion();



#endif