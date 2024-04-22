/************************************************************************
	XDraw.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#include "XDraw.h"
#include "XDrawWin.h"
#include <stdio.h>

BOOL CXDraw::InitDMusic(void)
{
	if(FAILED(CoCreateInstance(CLSIDDMPERF, NULL, CLSCTX_INPROC,
							   IIDDMPERF, (void **)&lpdmp)))
		return FALSE;

	if(FAILED(lpdmp->Init(NULL, lpds, win_hwnd)))
		return FALSE;

	if(FAILED(lpdmp->AddPort(NULL)))
		return FALSE;

	return TRUE;
}


BOOL CXDraw::InitDSound(void)
{
	if(FAILED(DirectSoundCreate(NULL, &lpds, NULL)))
		return FALSE;

	if(FAILED(lpds->SetCooperativeLevel(win_hwnd, DSSCL_NORMAL)))
		return FALSE;
	
	return TRUE;
}


BOOL CXDraw::InitDInput(void)
{
	int i;

	// Initialize DirectInput
	if(FAILED(DirectInputCreateEx(win_hinstance, DIRECTINPUT_VERSION, 
								  DIINTERFACE, (void **)&lpdi, NULL)))
		return FALSE;


	// KEYBOARD	
	// Create keyboard device
	if(FAILED(lpdi->CreateDeviceEx(GUID_SysKeyboard, DIDEVICEINT, 
								   (void **)&lpdikey, NULL)))
		return FALSE;

	// Set keyboard coop level
	if(FAILED(lpdikey->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
										   DISCL_NONEXCLUSIVE)))
		return FALSE;

	// Set keyboard data format (uses standard structure)
	if(FAILED(lpdikey->SetDataFormat(&c_dfDIKeyboard)))
		return FALSE;

	// Aquire the keyboard
	if(FAILED(lpdikey->Acquire()))
		return FALSE;


	// MOUSE
	// Create mouse device
	if(FAILED(lpdi->CreateDeviceEx(GUID_SysMouse, DIDEVICEINT,
								   (void **)&lpdimouse, NULL)))
		return FALSE;

	// Set mouse coop level
	if(FAILED(lpdimouse->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
											 DISCL_NONEXCLUSIVE)))
		return FALSE;

	// Set mouse data format (use standard structure)
	if(FAILED(lpdimouse->SetDataFormat(&c_dfDIMouse2)))
		return FALSE;

	// Aquire the mouse
	if(FAILED(lpdimouse->Acquire()))
		return FALSE;


/*	// JOYSTICK
	// Create joystick device
	if(FAILED(lpdi->CreateDeviceEx(GUID_Joystick, DIDEVICEINT,
								   (void **)&lpdijoy, NULL)))
		return FALSE;

	// Set joystick coop level
	if(FAILED(lpdijoy->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
											 DISCL_NONEXCLUSIVE)))
		return FALSE;

	// Set joystick data format (use standard structure)
	if(FAILED(lpdijoy->SetDataFormat(&c_dfDIJoystick2)))
		return FALSE;

	// Aquire the joystick
	if(FAILED(lpdijoy->Acquire()))
		return FALSE;*/

	ZeroMemory(&rawstate, sizeof(RAWSTATE));
	ZeroMemory(&univstate, sizeof(UNIVSTATE));

	for(i=0;i<MAX_KEYS;i++)
		univstate.button[i] = &rawstate.keyboard[i];

	for(i=MAX_KEYS;i<MAX_KEYBOARDMOUSE;i++)
		univstate.button[i] = &rawstate.mouse.rgbButtons[i-MAX_KEYS];

	for(i=MAX_KEYBOARDMOUSE;i<MAX_BUTTONS;i++)
		univstate.button[i] = &rawstate.joystick.rgbButtons[i-MAX_KEYBOARDMOUSE];

	univstate.axis[AXIS_MOUSEX]		= &rawstate.mouse.lX;
	univstate.axis[AXIS_MOUSEY]		= &rawstate.mouse.lY;
	univstate.axis[AXIS_MOUSEZ]		= &rawstate.mouse.lZ;
	univstate.axis[AXIS_JOYX]		= &rawstate.joystick.lX;
	univstate.axis[AXIS_JOYY]		= &rawstate.joystick.lY;
	univstate.axis[AXIS_JOYZ]		= &rawstate.joystick.lZ;
	univstate.axis[AXIS_JOYROTX]	= &rawstate.joystick.lRx;
	univstate.axis[AXIS_JOYROTY]	= &rawstate.joystick.lRy;
	univstate.axis[AXIS_JOYROTZ]	= &rawstate.joystick.lRz;
	univstate.axis[AXIS_SLIDER0]	= &rawstate.joystick.rglSlider[0];
	univstate.axis[AXIS_SLIDER1]	= &rawstate.joystick.rglSlider[1];
	univstate.axis[AXIS_POV0]		= (long *)&rawstate.joystick.rgdwPOV[0];
	univstate.axis[AXIS_POV1]		= (long *)&rawstate.joystick.rgdwPOV[1];
	univstate.axis[AXIS_POV2]		= (long *)&rawstate.joystick.rgdwPOV[2];
	univstate.axis[AXIS_POV3]		= (long *)&rawstate.joystick.rgdwPOV[3];
	univstate.axis[AXIS_JOYXVEL]	= &rawstate.joystick.lVX;
	univstate.axis[AXIS_JOYYVEL]	= &rawstate.joystick.lVY;
	univstate.axis[AXIS_JOYZVEL]	= &rawstate.joystick.lVZ;
	univstate.axis[AXIS_JOYXANGVEL]	= &rawstate.joystick.lVRx;
	univstate.axis[AXIS_JOYYANGVEL]	= &rawstate.joystick.lVRy;
	univstate.axis[AXIS_JOYZANGVEL]	= &rawstate.joystick.lVRz;
	univstate.axis[AXIS_SLIDERVEL0]	= &rawstate.joystick.rglVSlider[0];
	univstate.axis[AXIS_SLIDERVEL1]	= &rawstate.joystick.rglVSlider[1];
	univstate.axis[AXIS_JOYXACC]	= &rawstate.joystick.lAX;
	univstate.axis[AXIS_JOYYACC]	= &rawstate.joystick.lAY;
	univstate.axis[AXIS_JOYZACC]	= &rawstate.joystick.lAZ;
	univstate.axis[AXIS_JOYXANGACC]	= &rawstate.joystick.lARx;
	univstate.axis[AXIS_JOYYANGACC]	= &rawstate.joystick.lARy;
	univstate.axis[AXIS_JOYZANGACC]	= &rawstate.joystick.lARz;
	univstate.axis[AXIS_SLIDERACC0]	= &rawstate.joystick.rglASlider[0];
	univstate.axis[AXIS_SLIDERACC1]	= &rawstate.joystick.rglASlider[1];
	univstate.axis[AXIS_JOYXFORCE]	= &rawstate.joystick.lFX;
	univstate.axis[AXIS_JOYYFORCE]	= &rawstate.joystick.lFY;
	univstate.axis[AXIS_JOYZFORCE]	= &rawstate.joystick.lFZ;
	univstate.axis[AXIS_JOYXTORQUE]	= &rawstate.joystick.lFRx;
	univstate.axis[AXIS_JOYYTORQUE]	= &rawstate.joystick.lFRy;
	univstate.axis[AXIS_JOYZTORQUE]	= &rawstate.joystick.lFRz;
	univstate.axis[AXIS_SLIDERFORCE0]	= &rawstate.joystick.rglFSlider[0];
	univstate.axis[AXIS_SLIDERFORCE1]	= &rawstate.joystick.rglFSlider[1];

	return TRUE;
}


void CXDraw::MapKey(int keynum, LPVOID loc)
{
	//univstate.keyboard[keynum] = (UCHAR *)loc;
}


BOOL CXDraw::GetInput(void)
{
	if(FAILED(lpdikey->GetDeviceState(MAX_KEYS, &rawstate.keyboard)))
		return FALSE;

	if(FAILED(lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE2), &rawstate.mouse)))
		return FALSE;

/*	if(FAILED(lpdijoy->Poll()))
		return FALSE;
	
	if(FAILED(lpdijoy->GetDeviceState(sizeof(DIJOYSTATE2), &rawstate.joystick)))
		return FALSE;*/

	if(rawstate.joystick.lX) univstate.xaxis = rawstate.joystick.lX;
	univstate.xaxis = rawstate.mouse.lX;

	if(rawstate.joystick.lY) univstate.yaxis = rawstate.joystick.lY;
	univstate.yaxis = rawstate.mouse.lY;

	if(rawstate.joystick.lZ) univstate.zaxis = rawstate.joystick.lZ;
	univstate.zaxis = rawstate.mouse.lZ;

	
	return TRUE;
}


// Constructor
CXDraw::CXDraw(void)
{
	lpdd		 = NULL;
	lpddsprimary = NULL;
	lpddpal		 = NULL;
	lpds		 = NULL;
	lpdi		 = NULL;
	lpdikey		 = NULL;
	lpdimouse	 = NULL;
	lpdijoy		 = NULL;
	lpdmp		 = NULL;

	musicloaders = 0;

	INIT_DDOBJ(ddbltcolorfx);
}


// Deconstructor
CXDraw::~CXDraw(void)
{
	if(lpdmp && !musicloaders)		// FIXME!!
	{
		lpdmp->CloseDown();
		lpdmp->Release();
	}
	
	if(lpdijoy)
		lpdijoy->Release();

	if(lpdimouse)
		lpdimouse->Release();

	if(lpdikey)	
		lpdikey->Release();

	if(lpddpal)
		lpddpal->Release();

	if(lpds)
		lpds->Release();

	if(lpdi)
		lpdi->Release();

	if(lpdd)
		lpdd->Release();

	CoUninitialize();			// FIX: OTHER DECONSTRUCTORS MIGHT NEED THIS!
}


// Advances the color indexes one color
BOOL CXDraw::ShiftPalette(int start, int end)
{	
	int numcolors = end - start + 1;
	PALETTEENTRY pal[MAX_PALCOLORS];
	
	// get the palette range requested
	if(FAILED(lpddpal->GetEntries(0, start, numcolors, pal)))
		return FALSE;
	
	// shift the colors up one entry
	if(FAILED(lpddpal->SetEntries(0, start + 1, numcolors - 1, pal)))
		return FALSE;

	// copy the last color to the first
	if(FAILED(lpddpal->SetEntries(0, start, 1, &pal[numcolors - 1])))
		return FALSE;

	return TRUE;
}


// Sets the hardware palette = to the XDraw palette
BOOL CXDraw::UsePalette(PALCOLOR *newpalette)
{
	PALETTEENTRY	pal[MAX_PALCOLORS];

	for(int i=0;i<256;i++)
	{
		pal[i].peBlue	= newpalette[i].blue;
		pal[i].peGreen	= newpalette[i].green;
		pal[i].peRed	= newpalette[i].red;
		pal[i].peFlags	= PC_NOCOLLAPSE;
	}
	
	if(lpddpal)
		lpddpal->Release();

	if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256,
								  pal, &lpddpal, NULL)))
		return FALSE;

	if(FAILED(lpddsprimary->SetPalette(lpddpal)))
		return FALSE;

	return TRUE;
}


// Flip the primary and back buffers
inline void CXDraw::Flip(void)
{
	while(FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
}


// Sets the video mode!
BOOL CXDraw::SetMode(DWORD width, DWORD height, DWORD bpp)
{
	if(FAILED(lpdd->SetDisplayMode(width, height, bpp, 0, 0)))
		return FALSE;

	return TRUE;
}


// Initializes DirectX libraries
BOOL CXDraw::Init(void)
{
	if(FAILED(CoInitialize(NULL)))
		return FALSE;
	
	if(InitDDraw() == FALSE)	// Initialize DirectDraw
		return FALSE;

	if(InitDInput() == FALSE)	// Initialize DirectInput
		return FALSE;

	if(InitDSound() == FALSE)	// Initialize DirectSound
		return FALSE;

	if(InitDMusic() == FALSE)	// Initialize DirectMusic
		return FALSE;

	return TRUE;
}


// Initializes DirectDraw
BOOL CXDraw::InitDDraw(void)
{
	if(FAILED(CoCreateInstance(CLSID_DirectDraw,
							   NULL,
							   CLSCTX_ALL,
							   DDINTERFACE,	
							   (void **)&lpdd)))
		return FALSE;
	
	if(FAILED(lpdd->Initialize(NULL)))
		return FALSE;

	if(win_windowed == TRUE)	// set Cooperative Level (windowed)
	{
		if(FAILED(lpdd->SetCooperativeLevel(win_hwnd, DDSCL_NORMAL | DDSCL_MULTITHREADED)))
			return FALSE;
	}
	else						// set Cooperative Level (full screen)
	{
		if(FAILED(lpdd->SetCooperativeLevel(win_hwnd,
											DDSCL_FULLSCREEN	|
											DDSCL_ALLOWMODEX	|
											DDSCL_EXCLUSIVE		|
											DDSCL_MULTITHREADED |
											DDSCL_ALLOWREBOOT)))
			return FALSE;
	}
	
	return TRUE;
}


// return the pointer to the DDraw Interface
inline LPDIRECTDRAW CXDraw::GetDirectDraw(void)
{
	return lpdd;
}


// generic Error function
BOOL xdError(LPCTSTR errormsg)
{
	ShowCursor(TRUE);
	MessageBox(win_hwnd, errormsg, "Error", MB_OK | MB_ICONSTOP);
	//SendMessage(win_hwnd, WM_CLOSE, 0, 0);
	exit(0);

	return TRUE;
}