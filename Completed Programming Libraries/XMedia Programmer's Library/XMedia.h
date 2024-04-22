/************************************************************************
	XMedia.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#define DIRECTINPUT_VERSION 0x0700

#ifndef XMEDIA
#define XMEDIA

#define INITGUID
#include <objbase.h>
#include <ddraw.h>
#include <dsound.h>
#include <dinput.h>

#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>

#include <stdio.h>
#include <math.h>
#include <direct.h>
#include <wchar.h>
#include <mmsystem.h>

#include "XMediaErrors.h"

// for future versions of DirectX, easy to change
#define LPDIRECTDRAW	LPDIRECTDRAW7			// DDraw (main)
#define LPDDSURFACE 	LPDIRECTDRAWSURFACE7	// DDraw surface
#define DDSURFDESC		DDSURFACEDESC2			// surface description
#define DDPIXFORMAT		DDPIXELFORMAT			// pixel format
#define DDINTERFACE		IID_IDirectDraw7		// DDraw interface
#define DIINTERFACE		IID_IDirectInput7		// DInput interface
#define DIDEVICEINT		IID_IDirectInputDevice7 // DInput Device interface
#define LPDDPALETTE		LPDIRECTDRAWPALETTE		// DDraw palette
#define LPDSOUND		LPDIRECTSOUND			// DSound (main)
#define LPDINPUT		LPDIRECTINPUT7			// DInput (main)
#define LPDIDEVICE		LPDIRECTINPUTDEVICE7	// DInput device
#define DMPERF			IDirectMusicPerformance			// DMusic (main)
#define CLSIDDMPERF		CLSID_DirectMusicPerformance
#define IIDDMPERF		IID_IDirectMusicPerformance
#define DMLOADER		IDirectMusicLoader				// DMusic loader
#define CLSIDDMLOADER	CLSID_DirectMusicLoader
#define IIDDMLOADER		IID_IDirectMusicLoader
#define DMSEGMENT		IDirectMusicSegment
#define CLSIDDMSEGMENT	CLSID_DirectMusicSegment
#define IIDDMSEGMENT	IID_IDirectMusicSegment
#define DMSEGSTATE		IDirectMusicSegmentState
#define LPDSBUFFER		LPDIRECTSOUNDBUFFER

#define MAX_EXTFUNC			100
#define MAX_PALCOLORS		256
#define MAX_KEYS			256
#define MAX_MOUSEBUTTONS	8
#define MAX_JOYBUTTONS		128
#define MAX_KEYBOARDMOUSE	MAX_MOUSEBUTTONS + MAX_KEYS
#define MAX_BUTTONS			MAX_KEYS + MAX_MOUSEBUTTONS + MAX_JOYBUTTONS
#define MAX_MOUSEAXIS		3
#define MAX_JOYAXIS			36
#define BLACK				0

#define SCREEN_SYSMEM	DDSCAPS_SYSTEMMEMORY
#define SCREEN_VIDMEM	DDSCAPS_VIDEOMEMORY

// for converting RGBs into USHORT, UINT for diff depths
// (555 and 565 are both 16-bit color modes)
#define RGB555(r,g,b)	((b >> 3) + ((g >> 3) << 5) + ((r >> 3) << 10))
#define RGB565(r,g,b)	((b >> 3) + ((g >> 3) << 6) + ((r >> 3) << 11))
#define RGB32(r,g,b,a)	(b + (g << 8) + (r << 16) + (a << 24))

#define INIT_DDOBJ(obj)	{ZeroMemory(&obj,sizeof(obj)); \
						 obj.dwSize=sizeof(obj);}

#define FPS_TO_MS(fps)	(1000/fps)

typedef UINT	xmERROR;
typedef UINT	xmCOLOR;

DWORD			gbpp;	// global bytes per pixel FIXME: GET RID OF THIS!

struct xmPALCOLOR
{
	BYTE red;
	BYTE green;
	BYTE blue;
};

struct xmGENERICBITMAP
{
	DWORD width;		// width, height, bpp of bitmap
	DWORD height;
	DWORD bpp;

	LPBYTE	 buffer;				 // buffer for uncompressed data
	xmPALCOLOR palette[MAX_PALCOLORS]; // the bitmap palette
};

struct xmSCRINFO
{
	UINT	width;		// screen width, height, bpp (enhanced)
	UINT	height;
	UINT	bpp;
	UINT	pitch;		// bytes per scan line
	LPVOID	vidbuf;		// pointer to video buffer
};

struct xmINFO
{
	int	exclusions;

	int machineid;
};

#define XMEXCL_NODDRAW		1
#define XMEXCL_NODINPUT		2
#define XMEXCL_NODSOUND		4
#define XMEXCL_NODMUSIC		8
#define XMEXCL_NOKEY		16
#define XMEXCL_NOMOUSE		32
#define XMEXCL_NOJOY		64

#define XMLIM_DDRAWSOFT		8
#define XMLIM_DSOUNDSOFT	16
#define XMLIM_DMUSICSOFT	32


HRESULT xmError(LPCTSTR errormsg);

#include "CXMedia.h"
#include "CPolygon.h"
#include "CSurface.h"
#include "CSound.h"
#include "CMusic.h"
#include "CPan.h"
#include "CFont.h"

UNIVSTATE	univstate;	// for global input FIXME: GET RID OF THIS!

// compresses red, green, blue, alpha combos into a UINT based on the
// color depth of the screen NOTE: repl gbpp! by making a member func
inline xmCOLOR xmRGB(UINT red, UINT green = 0, UINT blue = 0, UINT alpha = 0)
{
	if(gbpp == 8)	return red;
	if(gbpp == 15)	return RGB555(red, green, blue);
	if(gbpp == 16)	return RGB565(red, green, blue);

	return RGB32(red, green, blue, alpha);
}

#include "CXMedia.cpp"		// XMedia class functions
#include "CPolygon.cpp"		// CPolygon class functions
#include "CSurface.cpp"		// CScreen class functions
#include "CSound.cpp"		// CSound class functions
#include "CMusic.cpp"		// CMusic class functions
#include "CPan.cpp"			// CPanBkgd class functions
#include "CFont.cpp"		// CFont class functions

#endif