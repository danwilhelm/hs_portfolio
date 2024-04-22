//--------------------------------------------------------------------
//Name:		Globals.cpp
//Desc:		Contains global variables and structures
//--------------------------------------------------------------------

#ifndef GLOBALS_CPP_
#define GLOBALS_CPP_

#include "Main.h"

//------------------------------------------------------------------\\
// Class Globals
RAZORKEYS RazorKeys;
//------------------------------------------------------------------//


//------------------------------------------------------------------\\
// Global Direct3D Interface Pointer Vars
IDirect3D8				*Direct3D			= NULL;
IDirect3DDevice8		*D3DDevice			= NULL;
IDirect3DVertexBuffer8	*D3DVertexBuffer	= NULL;
IDirect3DIndexBuffer8	*D3DIndexBuffer		= NULL;
IDirect3DTexture8		*D3DTexture			= NULL;

D3DDISPLAYMODE			*videoModes			= NULL;
D3DDEVTYPE				deviceType			= D3DDEVTYPE_HAL;
//------------------------------------------------------------------//


//------------------------------------------------------------------\\
// Other Global Vars
HRESULT hr;
_App	App;
bool	CheckSurfaces;
bool	Active = true;
bool	runnin = true;
bool	isWindowed = false;
int		ScreenWidth = 800, ScreenHeight = 600;
int		nModes = 0;
int		currentMode = 0;
//------------------------------------------------------------------//

#endif