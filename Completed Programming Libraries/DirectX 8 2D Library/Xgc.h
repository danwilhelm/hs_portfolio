/************************************************************************
	CXMedia.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#include "XgcInput.h"

// from C++ STL
#include <list>			// able to use doubly linked lists
using namespace std;	// allows us to use STL! (note to self: get rid of these!)

//typedef list<CAnimation> ANIMLIST;


const float		ZPLANE2D = 1.0f;
const float		RHW		 = 1.0f;

struct XgcXYSCALE
{
	float x, y;
	float xpercent, ypercent;
};


// A structure for our custom vertex type
struct XgcVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};


#define XgcCOLOR(r,g,b)		D3DCOLOR_XRGB(r,g,b)
#define XgcCOLORA(r,g,b,a)	D3DCOLOR_ARGB(a,r,g,b)

D3DCOLOR XgcTransparency = XgcCOLORA(0,0,0,0);


const D3DCOLOR	WHITE	= XgcCOLOR(255,255,255);
const D3DCOLOR	BLACK	= XgcCOLOR(0,0,0);
const D3DCOLOR	RED		= XgcCOLOR(255,0,0);
const D3DCOLOR	GREEN	= XgcCOLOR(0,255,0);
const D3DCOLOR	BLUE	= XgcCOLOR(0,0,255);

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


class Xgc
{
	private:
		HRESULT InitD3D(void);
		HRESULT InitDInput(void);
		HRESULT InitDAudio(void);

		void ReleaseD3D(void);
		void ReleaseDInput(void);
		void ReleaseAudio(void);
		
	public:
		LPDIRECTINPUT8				dinput;
		LPDIRECTINPUTDEVICE8		keyboard;
		LPDIRECTINPUTDEVICE8		mouse;
		LPDIRECTINPUTDEVICE8		joystick;

		IDirectMusicLoader8			*dmloader;
		IDirectMusicPerformance8	*dmperformance;
		IDirectMusicAudioPath8		*dmaudiopath;
		IDirectSoundFXWavesReverb8  *dmsoundfx;

		LPD3DXSPRITE sprite;


		// GENERAL
		
		Xgc(void);
		Release(void);
		HRESULT Create(int exclusions = 0);

		
		// GRAPHICS
		
		inline void SetTransparency(D3DCOLOR color);
	
		inline void Clear(D3DCOLOR color = BLACK);	
		inline void PutPixel(float x, float y, D3DCOLOR color);
		inline void Line(float x0, float y0, float x1, float y1, D3DCOLOR color);

		
		// INPUT

		HRESULT GetInput(void);

		
		// AUDIO

		HRESULT SetAudioDirectory(char *path);
};



