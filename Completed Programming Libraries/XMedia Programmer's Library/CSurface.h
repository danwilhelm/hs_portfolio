/************************************************************************
	CSurface.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


#define BLIT_OPAQUE			0
#define BLIT_TRANSPARENT	DDBLT_KEYSRC

class CSurface : public CXMedia
{
	private:
		HRESULT SetupFunc(void);
		HRESULT InitSurfaceData(void);

	protected:
		LPDIRECTDRAWCLIPPER lpddclipper;
		xmSCRINFO	scrinfo;

		UINT		width;
		UINT		height;
		LPBYTE		vidbuf;
		int			pitch;
		int			bytespp;
		CXMedia		*xmedia;

	public:
		LPDDSURFACE	lpdds;
		UINT		bpp;

		CSurface(void);
		Release(void);
		
		HRESULT SetTransparency(DWORD low, DWORD high);
		HRESULT CreatePrimary(CXMedia *xmedia, int backbuffers);
		HRESULT CreateSurface(CXMedia *xmedia, DWORD width, DWORD height, DWORD bpp, UINT flags = SCREEN_SYSMEM);
		HRESULT Clip(LPRECT rects, int numrects = 0);
	
		HRESULT Lock(void);
		HRESULT Unlock(void);

		HRESULT Load(CXMedia *xmedia, char *filename);
		HRESULT LoadBMP(CXMedia *xmedia, char *filename, xmGENERICBITMAP *bitmap);
		HRESULT BitmapToSurface(xmGENERICBITMAP *bitmap);
		HRESULT FlipBitmap(xmGENERICBITMAP *bitmap);
	
		inline void Blit(CSurface *surf, int x, int y, int colorfx = BLIT_TRANSPARENT);
		inline void BlitScaled(CSurface *surf, int x, int y, int xwidth, int yheight, int colorfx = BLIT_TRANSPARENT);
		inline void BlitX(CSurface *surf, LPRECT dest, LPRECT src, int colorfx = BLIT_TRANSPARENT, int rot = 0);

		void Poly(CPolygon *poly);
		inline void Rect(int x0, int y0, int x1, int y1, xmCOLOR color);
		inline void Clear(xmCOLOR color = BLACK);	
		inline void PutPixel(int x, int y, xmCOLOR color);
		void Line(int x0, int y0, int x1, int y1, xmCOLOR color);
		HRESULT Text(int x, int y, char *text, COLORREF color);

		inline UINT Width(void);
		inline UINT Height(void);
		inline xmSCRINFO GetScrInfo(void);
};
