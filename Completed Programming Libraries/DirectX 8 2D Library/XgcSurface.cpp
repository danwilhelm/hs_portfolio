/************************************************************************
	XgcSurface.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

const float		ZPLANE2D = 1.0f;
const float		RHW		 = 1.0f;

const int		SCREEN_SYSMEM = 0, DDBLT_KEYSRC = 0;
const int		XMSUCCESS = 0;

#define XgcCOLOR(r,g,b)		D3DCOLOR_XRGB(r,g,b)
#define XgcCOLORA(r,g,b,a)	D3DCOLOR_ARGB(a,r,g,b)

D3DCOLOR XgcTransparency = XgcCOLORA(0,0,0,0);


const D3DCOLOR	WHITE	= XgcCOLOR(255,255,255);
const D3DCOLOR	BLACK	= XgcCOLOR(0,0,0);
const D3DCOLOR	RED		= XgcCOLOR(255,0,0);
const D3DCOLOR	GREEN	= XgcCOLOR(0,255,0);
const D3DCOLOR	BLUE	= XgcCOLOR(0,0,255);

struct XgcXYSCALE
{
	float x, y;
	float xpercent, ypercent;
};


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


#define BLIT_OPAQUE			0
#define BLIT_TRANSPARENT	DDBLT_KEYSRC

#include "XgcTexture.cpp"

class XgcSurface
{
	private:
		HRESULT SetupFunc(void);
		HRESULT InitSurfaceData(void);

	protected:
		LPD3DXSPRITE sprite;
		
		UINT		width;
		UINT		height;
		LPBYTE		vidbuf;
		int			pitch;
		UINT		bpp;
		int			bytespp;

	public:
		XgcSurface(void);
		Release(void);
		
		HRESULT SetTransparency(DWORD low, DWORD high);
	
		inline void Rect(float x0, float y0, float x1, float y1, D3DCOLOR color);
		inline void Clear(D3DCOLOR color = BLACK);	
		inline void PutPixel(float x, float y, D3DCOLOR color);
		void Line(float x0, float y0, float x1, float y1, D3DCOLOR color);
		HRESULT Text(int x, int y, char *text, COLORREF color);
};


inline void XgcSurface::Rect(float x0, float y0, float x1, float y1, D3DCOLOR color)
{
/*    CUSTOMVERTEX data[2] = {{x0, y0, ZPLANE2D, RHW, color}, {x1, y1, ZPLANE2D, RHW, color}};
	
	XgcDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);
	XgcDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &data, sizeof(CUSTOMVERTEX));*/
}


void XgcSurface::Line(float x0, float y0, float x1, float y1, D3DCOLOR color)
{
    CUSTOMVERTEX data[2] = {{x0, y0, ZPLANE2D, RHW, color}, {x1, y1, ZPLANE2D, RHW, color}};
	
	XgcDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);
	XgcDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &data, sizeof(CUSTOMVERTEX));
}



void XgcSurface::SetTransparency(D3DCOLOR color)
{
	XgcTransparency = color;

	return S_OK;
}


// clears the screen a desired color
inline void XgcSurface::Clear(D3DCOLOR color)
{
    XgcDevice->Clear( 0, NULL, D3DCLEAR_TARGET, color, ZPLANE2D, 0 );
}


// Puts a pixel on the screen!!!
inline void XgcSurface::PutPixel(float x, float y, D3DCOLOR color)
{			
    CUSTOMVERTEX data[1] = {x, y, ZPLANE2D, RHW, color};
	
	XgcDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);
	XgcDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &data, sizeof(CUSTOMVERTEX));
}


HRESULT XgcSurface::Text(int x, int y, char *text, COLORREF color)
{
	// D3DX (expand)
	return S_OK;
}


XgcSurface::XgcSurface(void)
{
	D3DXCreateSprite(XgcDevice, &sprite);
}


XgcSurface::Release(void)
{
/*	if(lpddclipper)
		lpddclipper->Release();
	
	if(lpdds)
		lpdds->Release();*/
}
