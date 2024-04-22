/************************************************************************
	CScreen.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#define BLIT_OPAQUE			0
#define BLIT_TRANSPARENT	DDBLT_KEYSRC

class CScreen : public CXDraw
{
	private:
		BOOL SetupFunc(void);
		BOOL InitScreenData(void);

	protected:
		LPDDSURFACE	lpdds;
		xdSCRINFO	scrinfo;

		UINT		width;
		UINT		height;
		LPBYTE		vidbuf;
		int			pitch;
		UINT		bpp;
		int			bytespp;

	public:
		CScreen(void);
		~CScreen(void);
		
		BOOL SetTransparency(DWORD low, DWORD high);
		BOOL CreatePrimary(CXDraw *xdraw, CScreen *dblbuf = NULL, CScreen *trpbuf = NULL);
		BOOL CreateScreen(CXDraw *xdraw, DWORD width, DWORD height, DWORD bpp, UINT flags = SCREEN_SYSMEM);
	
		BOOL Lock(void);
		BOOL Unlock(void);

		BOOL Load(CXDraw *xdraw, char *filename);
		BOOL LoadBMP(CXDraw *xdraw, char *filename, GENERICBITMAP *bitmap);
		BOOL BitmapToSurface(GENERICBITMAP *bitmap);
		BOOL FlipBitmap(GENERICBITMAP *bitmap);
	
		inline void Blit(CScreen *screen, int x, int y, int colorfx = BLIT_TRANSPARENT);
		inline void BlitScaled(CScreen *screen, int x, int y, int xwidth, int yheight, int colorfx = BLIT_TRANSPARENT);
		inline void BlitX(CScreen *screen, LPRECT dest, LPRECT src, int colorfx = BLIT_TRANSPARENT, int rot = 0);

		inline void Rect(int x0, int y0, int x1, int y1, xdCOLOR color);
		inline void Clear(xdCOLOR color = BLACK);	
		inline void PutPixel(int x, int y, xdCOLOR color);
		void Line(int x0, int y0, int x1, int y1, xdCOLOR color);
		void Poly(CPolygon *poly);
		BOOL Text(int x, int y, char *text, COLORREF color);

		inline UINT Width(void);
		inline UINT Height(void);
		inline xdSCRINFO GetScrInfo(void);
};