/************************************************************************
	CXMedia.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


#include "XMedia.h"
#include "XMediaWin.h"
#include <stdio.h>

HRESULT CXMedia::InitDMusic(void)
{
	if(FAILED(CoCreateInstance(CLSIDDMPERF, NULL, CLSCTX_INPROC,
							   IIDDMPERF, (void **)&lpdmp)))
		return CXMEDIA_NODMUSIC;

	if(FAILED(lpdmp->Init(NULL, lpds, win_hwnd)))
		return CXMEDIA_NODMUSIC;

	if(FAILED(lpdmp->AddPort(NULL)))
		return CXMEDIA_NODMUSIC;

	return XMSUCCESS;
}


HRESULT CXMedia::InitDSound(void)
{
	if(FAILED(DirectSoundCreate(NULL, &lpds, NULL)))
		return CXMEDIA_NODSOUND;

	if(FAILED(lpds->SetCooperativeLevel(win_hwnd, DSSCL_NORMAL)))
		return CXMEDIA_NODSOUND;

	if(FAILED(CoCreateInstance(CLSIDDMLOADER, NULL, CLSCTX_INPROC,
							   IIDDMLOADER, (void **)&lpdml)))
		return CXMEDIA_NODSOUND;
	
	return XMSUCCESS;
}


HRESULT CXMedia::InitDInput(int exclusions)
{
	int i;

	// Initialize DirectInput
	if(FAILED(DirectInputCreateEx(win_hinstance, DIRECTINPUT_VERSION, 
								  DIINTERFACE, (void **)&lpdi, NULL)))
		return CXMEDIA_NODINPUT;

	
	if(!(exclusions & XMEXCL_NOKEY))
	{
		// KEYBOARD	
		// Create keyboard device
		if(FAILED(lpdi->CreateDeviceEx(GUID_SysKeyboard, DIDEVICEINT, 
									   (void **)&lpdikey, NULL)))
			return CXMEDIA_KEYIN;

		// Set keyboard coop level
		if(FAILED(lpdikey->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
											   DISCL_NONEXCLUSIVE)))
			return CXMEDIA_KEYIN;

		// Set keyboard data format (uses standard structure)
		if(FAILED(lpdikey->SetDataFormat(&c_dfDIKeyboard)))
			return CXMEDIA_KEYIN;

		// Aquire the keyboard
		if(FAILED(lpdikey->Acquire()))
			return CXMEDIA_KEYIN;
	}


	if(!(exclusions & XMEXCL_NOMOUSE))
	{
		// MOUSE
		// Create mouse device
		if(FAILED(lpdi->CreateDeviceEx(GUID_SysMouse, DIDEVICEINT,
									   (void **)&lpdimouse, NULL)))
			return CXMEDIA_MOUSEIN;

		// Set mouse coop level
		if(FAILED(lpdimouse->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
												 DISCL_NONEXCLUSIVE)))
			return CXMEDIA_MOUSEIN;

		// Set mouse data format (use standard structure)
		if(FAILED(lpdimouse->SetDataFormat(&c_dfDIMouse2)))
			return CXMEDIA_MOUSEIN;

		// Aquire the mouse
		if(FAILED(lpdimouse->Acquire()))
			return CXMEDIA_MOUSEIN;
	}


/*	if(!(exclusions & XMEXCL_NOJOY))
	{
		// JOYSTICK
		// Create joystick device
		if(FAILED(lpdi->CreateDeviceEx(GUID_Joystick, DIDEVICEINT,
									   (void **)&lpdijoy, NULL)))
			return CXMEDIA_JOYIN;

		// Set joystick coop level
		if(FAILED(lpdijoy->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
												 DISCL_NONEXCLUSIVE)))
			return CXMEDIA_JOYIN;

		// Set joystick data format (use standard structure)
		if(FAILED(lpdijoy->SetDataFormat(&c_dfDIJoystick2)))
			return CXMEDIA_JOYIN;

		// Aquire the joystick
		if(FAILED(lpdijoy->Acquire()))
			return CXMEDIA_JOYIN;
	}*/


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

	return XMSUCCESS;
}


// FIXME: Obsolete!
void CXMedia::MapKey(int keynum, LPVOID loc)
{
	//univstate.keyboard[keynum] = (UCHAR *)loc;
}


HRESULT CXMedia::GetInput(void)
{
	if(FAILED(lpdikey->GetDeviceState(MAX_KEYS, &rawstate.keyboard)))
		return CXMEDIA_KEYIN;

	if(FAILED(lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE2), &rawstate.mouse)))
		return CXMEDIA_MOUSEIN;

/*	if(FAILED(lpdijoy->Poll()))
		return CXMEDIA_JOYIN;
	
	if(FAILED(lpdijoy->GetDeviceState(sizeof(DIJOYSTATE2), &rawstate.joystick)))
		return CXMEDIA_JOYIN;*/

	if(rawstate.joystick.lX) univstate.xaxis = rawstate.joystick.lX;
	univstate.xaxis = rawstate.mouse.lX;

	if(rawstate.joystick.lY) univstate.yaxis = rawstate.joystick.lY;
	univstate.yaxis = rawstate.mouse.lY;

	if(rawstate.joystick.lZ) univstate.zaxis = rawstate.joystick.lZ;
	univstate.zaxis = rawstate.mouse.lZ;

	
	return XMSUCCESS;
}


// Constructor
CXMedia::CXMedia(void)
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
	lpdml		 = NULL;

	musicloaders = 0;

	INIT_DDOBJ(ddbltcolorfx);
}


// Deconstructor
CXMedia::Release(void)
{
	if(lpdml)
		lpdml->Release();

	if(lpdmp)
	{
		lpdmp->Stop(NULL, NULL, 0, 0);
		lpdmp->CloseDown();
		lpdmp = NULL;
	}

	if(lpdijoy)
	{
		lpdijoy->Release();
		lpdijoy = NULL;
	}

	if(lpdimouse)
	{
		lpdimouse->Release();
		lpdimouse = NULL;
	}

	if(lpdikey)	
	{
		lpdikey->Release();
		lpdikey = NULL;
	}

	if(lpddpal)
	{
		lpddpal->Release();
		lpddpal = NULL;
	}

	if(lpds)
	{
		lpds->Release();
		lpds = NULL;
	}

	if(lpdi)
	{
		lpdi->Release();
		lpdi = NULL;
	}

	if(lpddssecondary)
	{
		lpddssecondary->Release();
		lpddssecondary = NULL;
	}

	if(lpddsprimary)
	{
		lpddsprimary->Release();
		lpddsprimary = NULL;
	}

	if(lpdd)
	{
		lpdd->Release();
		lpdd = NULL;
	}

	CoUninitialize();			// FIX: OTHER DECONSTRUCTORS MIGHT NEED THIS!
}


// Advances the color indexes one color
HRESULT CXMedia::ShiftPalette(int start, int end)
{	
	int numcolors = end - start + 1;
	PALETTEENTRY pal[MAX_PALCOLORS];
	
	// get the palette range requested
	if(FAILED(lpddpal->GetEntries(0, start, numcolors, pal)))
		return CXMEDIA_CHANGEPAL;
	
	// shift the colors up one entry
	if(FAILED(lpddpal->SetEntries(0, start + 1, numcolors - 1, pal)))
		return CXMEDIA_CHANGEPAL;

	// copy the last color to the first
	if(FAILED(lpddpal->SetEntries(0, start, 1, &pal[numcolors - 1])))
		return CXMEDIA_CHANGEPAL;

	return XMSUCCESS;
}


// Sets the hardware palette = to the XDraw palette
HRESULT CXMedia::UsePalette(xmPALCOLOR *newpalette)
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
		return CXMEDIA_SETPAL;

	if(FAILED(lpddsprimary->SetPalette(lpddpal)))
		return CXMEDIA_SETPAL;

	return XMSUCCESS;
}


// Flip the primary and back buffers
inline void CXMedia::Flip(void)
{
	if(lpddsprimary->IsLost() == DDERR_SURFACELOST)
		lpddsprimary->Restore();

	while(FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
}


// Sets the video mode!
HRESULT CXMedia::SetMode(CSurface *surface, DWORD width, DWORD height, DWORD bpp, int backbuffers)
{
	if(FAILED(lpdd->SetDisplayMode(width, height, bpp, 0, 0)))
		return CXMEDIA_INVALIDMODE;

	if(lpddsprimary) lpddsprimary->Release(), lpddsprimary = NULL;

	if(lpddssecondary) lpddssecondary->Release(), lpddssecondary = NULL;

	if(surface->CreatePrimary(this, backbuffers))
		return CXMEDIA_NOPRIMARY;

	return XMSUCCESS;
}


// Initializes DirectX libraries
HRESULT CXMedia::Create(int exclusions)
{
	int inputexclusions = 0, err;
	
	if(FAILED(CoInitialize(NULL)))
		return COM_UNKNOWN;
	
	if(!(exclusions & XMEXCL_NODDRAW))
		if(InitDDraw())					// Initialize DirectDraw
			return CXMEDIA_NODDRAW;

	if(!(exclusions & XMEXCL_NODINPUT))
	{
		do
		{
			err = InitDInput();				// Initialize DirectInput

			if(err == CXMEDIA_KEYIN)
				inputexclusions |= XMEXCL_NOKEY;

			if(err == CXMEDIA_MOUSEIN)
				inputexclusions |= XMEXCL_NOMOUSE;

			if(err == CXMEDIA_JOYIN)
				inputexclusions |= XMEXCL_NOJOY;

		} while(err != CXMEDIA_NODINPUT && err != XMSUCCESS);

		if(err == CXMEDIA_NODINPUT)
			return CXMEDIA_NODINPUT;			
	}

	if(!(exclusions & XMEXCL_NODSOUND)) 
		if(InitDSound())				// Initialize DirectSound
			return CXMEDIA_NODSOUND;

	if(!(exclusions & XMEXCL_NODMUSIC)) 
		if(InitDMusic())				// Initialize DirectMusic
			return CXMEDIA_NODMUSIC;

	ShowCursor(FALSE);

	return XMSUCCESS;
}


// Initializes DirectDraw
HRESULT CXMedia::InitDDraw(void)
{
	if(FAILED(CoCreateInstance(CLSID_DirectDraw,
							   NULL,
							   CLSCTX_ALL,
							   DDINTERFACE,	
							   (void **)&lpdd)))
		return COM_UNKNOWN;
	
	if(FAILED(lpdd->Initialize(NULL)))
		return COM_UNKNOWN;

	if(win_windowed == TRUE)	// set Cooperative Level (windowed)
	{
		if(FAILED(lpdd->SetCooperativeLevel(win_hwnd, DDSCL_NORMAL | DDSCL_MULTITHREADED)))
			return CXMEDIA_NODDRAW;
	}
	else						// set Cooperative Level (full screen)
	{
		if(FAILED(lpdd->SetCooperativeLevel(win_hwnd,
											DDSCL_FULLSCREEN	|
											DDSCL_ALLOWMODEX	|
											DDSCL_EXCLUSIVE		|
											DDSCL_MULTITHREADED |
											DDSCL_ALLOWREBOOT)))
			return CXMEDIA_NODDRAW;
	}


	return XMSUCCESS;
}


// return the pointer to the DDraw Interface
inline LPDIRECTDRAW CXMedia::GetDirectDraw(void)
{
	return lpdd;
}


// generic Error function
HRESULT xmError(LPCTSTR errormsg)
{
	ShowCursor(TRUE);
	MessageBox(win_hwnd, errormsg, "Error", MB_OK | MB_ICONSTOP);
	
	Shutdown();
	PostQuitMessage(0);

	return TRUE;
}