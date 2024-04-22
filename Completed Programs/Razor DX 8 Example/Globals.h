//--------------------------------------------------------------------
//Name:		Globals.h
//Desc:		Contains global variables and structures	
//--------------------------------------------------------------------


#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "Main.h"

//------------------------------------------------------------------\\
// Defines

//Definition of HELPER_RELEASE function(Simplifies Shutdown Process)
#define HELPER_RELEASE(x) { if(x) { (x)->Release(); (x) = NULL; }}
//------------------------------------------------------------------//


//------------------------------------------------------------------\\
// Structures
struct _App 
{
	HINSTANCE hInst;
	HWND hWnd;
};

// A structure for our custom vertex type
struct VERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};
//------------------------------------------------------------------//


//------------------------------------------------------------------\\
// Class Definitions
class RAZORKEYS;
//------------------------------------------------------------------//

//------------------------------------------------------------------\\
// Class Globals
extern RAZORKEYS RazorKeys;
//------------------------------------------------------------------//


//------------------------------------------------------------------\\
// Global Direct3D Interface Pointer Vars
extern IDirect3D8				*Direct3D;
extern IDirect3DDevice8			*D3DDevice;
extern IDirect3DVertexBuffer8	*D3DVertexBuffer;
extern IDirect3DIndexBuffer8	*D3DIndexBuffer;
extern IDirect3DTexture8		*D3DTexture;

extern D3DDISPLAYMODE			*videoModes;
extern D3DDEVTYPE				deviceType;
//------------------------------------------------------------------//


//------------------------------------------------------------------\\
// Other Global Vars
extern HRESULT	hr;
extern _App		App;
extern bool		CheckSurfaces;
extern bool		Active;
extern bool		runnin;
extern bool		isWindowed;
extern int		ScreenWidth;
extern int		ScreenHeight;
extern int		nModes;
extern int		currentMode;
//------------------------------------------------------------------//

#endif
