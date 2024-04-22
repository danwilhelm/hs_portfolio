/************************************************************************
	Xgc.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#include "Xgc.h"


inline void Xgc::SetTransparency(D3DCOLOR color)
{
	XgcTransparency = color;
}


// clears the screen a desired color
inline void Xgc::Clear(D3DCOLOR color)
{
    XgcDevice->Clear( 0, NULL, D3DCLEAR_TARGET, color, ZPLANE2D, 0 );
}


// Puts a pixel on the screen!!!
inline void Xgc::PutPixel(float x, float y, D3DCOLOR color)
{			
    XgcVERTEX data[1] = {x, y, ZPLANE2D, RHW, color};
	
	XgcDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);
	XgcDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &data, sizeof(XgcVERTEX));
}


inline void Xgc::Line(float x0, float y0, float x1, float y1, D3DCOLOR color)
{
    XgcVERTEX data[2] = {{x0, y0, ZPLANE2D, RHW, color}, {x1, y1, ZPLANE2D, RHW, color}};
	
	XgcDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);
	XgcDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &data, sizeof(XgcVERTEX));
}


HRESULT Xgc::SetAudioDirectory(char *path)
{
   // Convert to Unicode.
 
    WCHAR wstrSearchPath[MAX_PATH];
    MultiByteToWideChar( CP_ACP, 0, path, -1, 
                         wstrSearchPath, MAX_PATH );
 
    // Set the search directory.
 
    RETFAIL(dmloader->SetSearchDirectory( 
        GUID_DirectMusicAllTypes,   // Types of files sought.
        wstrSearchPath,             // Where to look.
        FALSE                       // Don't clear object data.
    ));

	return S_OK;
}


HRESULT Xgc::InitDAudio(void)
{
    // Initialize COM
    CoInitialize(NULL);
    
    // Create loader object
    CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
                      IID_IDirectMusicLoader8, (void**)&dmloader );

    // Create performance object
    CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
                      IID_IDirectMusicPerformance8, (void**)&dmperformance );

    // Initialize the performance with the standard audio path.
    // This initializes both DirectMusic and DirectSound and 
    // sets up the synthesizer. 
    RETFAIL(dmperformance->InitAudio( NULL, NULL, win_hwnd, 
					                   DMUS_APATH_SHARED_STEREOPLUSREVERB, 128,
							           DMUS_AUDIOF_ALL, NULL ));

	RETFAIL(dmperformance->CreateStandardAudioPath(DMUS_APATH_SHARED_STEREOPLUSREVERB, 
												   128, TRUE, &dmaudiopath));

	RETFAIL(dmperformance->GetDefaultAudioPath(&dmaudiopath));
	
	RETFAIL(dmaudiopath->GetObjectInPath(DMUS_PCHANNEL_ALL,
									DMUS_PATH_BUFFER_DMO, 1,
									GUID_All_Objects, 0, IID_IDirectSoundFXWavesReverb8,
									(LPVOID *) &dmsoundfx));

	return S_OK;
}


HRESULT Xgc::InitDInput(void)
{
	int i;

	RETFAIL(DirectInput8Create(win_hinstance, DIRECTINPUT_VERSION, 
							   IID_IDirectInput8, (void **)&dinput, NULL));

	// KEYBOARD	
	// Create keyboard device
	RETFAIL(dinput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL));

	// Set keyboard coop level
	RETFAIL(keyboard->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
										   DISCL_NONEXCLUSIVE));

	// Set keyboard data format (uses standard structure)
	RETFAIL(keyboard->SetDataFormat(&c_dfDIKeyboard));

	// Aquire the keyboard
	RETFAIL(keyboard->Acquire());


	// MOUSE
	// Create mouse device
	RETFAIL(dinput->CreateDevice(GUID_SysMouse, &mouse, NULL));

	// Set mouse coop level
	RETFAIL(mouse->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));

	// Set mouse data format (use standard structure)
	RETFAIL(mouse->SetDataFormat(&c_dfDIMouse2));

	// Aquire the mouse
	RETFAIL(mouse->Acquire());


/*	// JOYSTICK
	// Create joystick device
	RETFAIL(dinput->CreateDevice(GUID_Joystick, &joystick, NULL));

	// Set joystick coop level
	RETFAIL(joystick->SetCooperativeLevel(win_hwnd, DISCL_BACKGROUND |
										  DISCL_NONEXCLUSIVE));

	// Set joystick data format (use standard structure)
	RETFAIL(joystick->SetDataFormat(&c_dfDIJoystick2));

	// Aquire the joystick
	RETFAIL(joystick->Acquire());*/


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

	return S_OK;
}


HRESULT Xgc::GetInput(void)
{
	hr = keyboard->GetDeviceState(MAX_KEYS, &rawstate.keyboard);
	if(FAILED(hr)) 
    {
        hr = keyboard->Acquire();
        while(hr == DIERR_INPUTLOST) 
            hr = keyboard->Acquire();

        return S_OK; 
    }

	hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &rawstate.mouse);
	if(FAILED(hr))
	{
		hr = mouse->Acquire();
		while(hr == DIERR_INPUTLOST)
			hr = mouse->Acquire();

		return S_OK;
	}


/*	RETFAIL(joystick->Poll());
	RETFAIL(joystick->GetDeviceState(sizeof(DIJOYSTATE2), &rawstate.joystick));*/
	
	if(rawstate.joystick.lX) univstate.xaxis = rawstate.joystick.lX;
	univstate.xaxis = rawstate.mouse.lX;

	if(rawstate.joystick.lY) univstate.yaxis = rawstate.joystick.lY;
	univstate.yaxis = rawstate.mouse.lY;

	if(rawstate.joystick.lZ) univstate.zaxis = rawstate.joystick.lZ;
	univstate.zaxis = rawstate.mouse.lZ;

	
	return S_OK;
}


// Constructor
Xgc::Xgc(void)
{
	dinput			= NULL;
	keyboard		= NULL;
	mouse			= NULL;
	joystick		= NULL;

	dmloader		= NULL;
	dmperformance	= NULL;

	SetFPS(30);
}


void Xgc::ReleaseAudio(void)
{
    dmperformance->Stop(
        NULL,   // Stop all segments.
        NULL,   // Stop all segment states.
        0,      // Do it immediately.
        0       // Flags.
    );
 
    dmperformance->CloseDown();
 
	DX_RELEASE(dmaudiopath);
    DX_RELEASE(dmloader);
    DX_RELEASE(dmperformance);
 
    CoUninitialize();
}


void Xgc::ReleaseD3D(void)
{
	DX_RELEASE(sprite);
}


void Xgc::ReleaseDInput(void)
{
	DX_RELEASE(joystick);
	DX_RELEASE(mouse);
	DX_RELEASE(keyboard);
	DX_RELEASE(dinput);
}


// Deconstructor
Xgc::Release(void)
{
	ReleaseAudio();
	ReleaseDInput();
	ReleaseD3D();
}


/*// Sets the video mode!
HRESULT Xgc::SetMode(CSurface *surface, DWORD width, DWORD height, DWORD bpp, int backbuffers)
{
	if(FAILED(lpdd->SetDisplayMode(width, height, bpp, 0, 0)))
		return Xgc_INVALIDMODE;

	if(lpddsprimary) lpddsprimary->Release(), lpddsprimary = NULL;

	if(lpddssecondary) lpddssecondary->Release(), lpddssecondary = NULL;

	if(surface->CreatePrimary(this, backbuffers))
		return Xgc_NOPRIMARY;

	return XMSUCCESS;
}*/


// Initializes DirectX libraries
HRESULT Xgc::Create(int exclusions)
{

	RETFAIL(InitD3D());
	RETFAIL(InitDInput());
	RETFAIL(InitDAudio());

	//ShowCursor(FALSE);

	return S_OK;
}


HRESULT Xgc::InitD3D(void)
{
    // Create the D3D object.
    if(NULL == (XgcD3D = Direct3DCreate8(D3D_SDK_VERSION)))
        return E_FAIL;

    // Get the current desktop display mode, so we can set up a back
    // buffer of the same format
    D3DDISPLAYMODE d3ddm;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

	if(win_fullscreen)
	{
		d3dpp.BackBufferWidth = XgcDisplayMode.Width;
		d3dpp.BackBufferHeight = XgcDisplayMode.Height;
		d3dpp.BackBufferCount = 1;
		//d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
	}

	RETERR(XgcD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm));

	d3dpp.Windowed = !win_fullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	if(XgcDisplayMode.Format)
		d3dpp.BackBufferFormat = XgcDisplayMode.Format;
	else
		d3dpp.BackBufferFormat = d3ddm.Format;


    // Create the D3DDevice
    if( FAILED( XgcD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win_hwnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &XgcDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling
    XgcDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting
    XgcDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Turn off the zbuffer
    XgcDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

	// Create the sprite drawer object
	D3DXCreateSprite(XgcDevice, &sprite);

    return S_OK;
}
