//--------------------------------------------------------------------
//Name:		Main.h
//Desc:		Includes all project headers and generic global functions	
//--------------------------------------------------------------------

#ifndef MAIN_H_
#define MAIN_H_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>	//Windows Header
#include <d3d8.h>		//D3D8 Header
#include <d3dx8.h>		//D3Dx Header
#include <stdio.h>
#include <dinput.h>

#include "Globals.h"
#include "InitRazor.h"
#include "RazorWin.h"
#include "Razor2D.h"
#include "RazorKeys.h"

#include "resource.h"

//------------------------------------------------------------------\\
// Direct3D Function Prototypes
HRESULT DrawScene();
//------------------------------------------------------------------//

BOOL MainLoop();

#endif