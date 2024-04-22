/************************************************************************
	CScreen.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#include "CBitmap.h"
#include <fstream.h>


inline void CScreen::Rect(int x0, int y0, int x1, int y1, xdCOLOR color)
{
	this->Line(x0, y0, x1, y0, color);
	this->Line(x1, y0, x1, y1, color);
	this->Line(x1, y1, x0, y1, color);
	this->Line(x0, y1, x0, y0, color);	
}


void CScreen::Poly(CPolygon *poly)
{
	int i;

	poly->Translate();

	for(i=0;i<poly->maxvertex;i++)
	{
		this->Line((int)(poly->vertex[i].x   + poly->x),
				   (int)(poly->vertex[i].y   + poly->y),
				   (int)(poly->vertex[i+1].x + poly->x),
				   (int)(poly->vertex[i+1].y + poly->y),
				   poly->color);
	}

	this->Line((int)(poly->vertex[0].x + poly->x),
			   (int)(poly->vertex[0].y + poly->y),
			   (int)(poly->vertex[i].x + poly->x),
			   (int)(poly->vertex[i].y + poly->y),
			   poly->color);
}


void CScreen::Line(int x0, int y0, int x1, int y1, xdCOLOR color)
{
	int i;
	int incx, incy;
	int xdist2, ydist2;
	int error;

	int xdist	= x1 - x0;
	int ydist	= y1 - y0;

	UCHAR *vid	= vidbuf + (y0 * pitch) + (x0 * bytespp);

	if(xdist >= 0)
	{
		incx = bytespp;
	}
	else
	{
		incx  = -bytespp;
		xdist = -xdist;
	}

	if(ydist >= 0)
	{
		incy = pitch;
	}
	else
	{
		incy  = -pitch;
		ydist = -ydist;
	}

	xdist2 = xdist << 1;
	ydist2 = ydist << 1;

	if(xdist > ydist)
	{
		error = ydist2 - xdist;

		for(i=0;i<=xdist;i++)
		{
			memcpy(vid, &color, bytespp);		// FIX: USE ASM!

			if(error >= 0)
			{
				error -= xdist2;
				vid   += incy;
			}

			error += ydist2;
			vid   += incx;
		}
	}
	else
	{
		error = xdist2 - ydist;

		for(i=0;i<=ydist;i++)
		{
			memcpy(vid, &color, bytespp);		// FIX: USE ASM!

			if(error >= 0)
			{
				error -= ydist2;
				vid   += incx;
			}

			error += xdist2;
			vid   += incy;
		}
	}
}


inline void CScreen::Blit(CScreen *screen, int x, int y, int colorfx)
{
	if(colorfx == BLIT_TRANSPARENT) colorfx = DDBLTFAST_SRCCOLORKEY;
	lpdds->BltFast(x, y, screen->lpdds, NULL, colorfx | DDBLTFAST_WAIT);
}


inline void CScreen::BlitScaled(CScreen *screen, int x, int y, int xwidth, int yheight, int colorfx)
{
	RECT destrect = {x, y, x + xwidth, y + yheight};

	lpdds->Blt(&destrect, screen->lpdds, NULL, colorfx | DDBLT_WAIT, &ddbltcolorfx);
}


inline void CScreen::BlitX(CScreen *screen, LPRECT dest, LPRECT src, int colorfx, int rot)
{
	if(rot)
	{
		ddbltcolorfx.dwRotationAngle = rot;
		colorfx |= DDBLT_ROTATIONANGLE;
	}

	lpdds->Blt(dest, screen->lpdds, src, colorfx | DDBLT_WAIT, &ddbltcolorfx);
}


BOOL CScreen::SetTransparency(DWORD low, DWORD high)
{
	DWORD colorrange = NULL;
	DDCOLORKEY colorkey;

	colorkey.dwColorSpaceLowValue	= low;
	colorkey.dwColorSpaceHighValue	= high;

	if(low != high)
		colorrange |= DDCKEY_COLORSPACE;
	
	if(FAILED(lpdds->SetColorKey(colorrange | DDCKEY_SRCBLT, &colorkey)))
		return FALSE;

	return TRUE;
}


BOOL CScreen::BitmapToSurface(GENERICBITMAP *bitmap)
{
	DWORD i;
	int bytesperline = (bitmap->bpp >> 3) * bitmap->width;
	LPBYTE lpsource = bitmap->buffer;
	LPBYTE lpdest;

	this->Lock();

	lpsource = bitmap->buffer;
	lpdest	 = vidbuf;

	for(i=0;i<bitmap->height;i++)
	{
		memcpy(lpdest, lpsource, bytesperline);

		lpdest	 += pitch;
		lpsource += bytesperline;
	}

	this->Unlock();

	return TRUE;
}


BOOL CScreen::Load(CXDraw *xdraw, char *filename)
{
	//int i;
	char *extension = NULL;
	char *filenamecopy = filename;
	GENERICBITMAP bitmap;
	
	bitmap.buffer = NULL;

	// NOTE: modified from PATH.C, Microsoft libraries
	for (extension = NULL; *filenamecopy; filenamecopy++)
	{         
		switch (*filenamecopy) 
		{         
			case '.':             
				extension = filenamecopy + 1; // remember after the last dot        
			break;
			  
			case '\\':         
			case ' ':			  // extensions can't have spaces             
				extension = NULL; // forget last dot, it was in a directory             
			break;         
		}     
	}      

	if(extension == NULL)
		return FALSE;

	//for(i=0;i<3;i++)
	//	extension[i] = toupper(extension[i]);

	//xdError(extension);

	//if(extension[0] == 'B' && 
	//   extension[1] == 'M' && 
	//   extension[2] == 'P')
	//{
		if(!LoadBMP(xdraw, filename, &bitmap))
			return FALSE;
	//}

	
	/*if(bitmap.buffer != NULL)
	{
		this->CreateScreen(xdraw, bitmap.width, bitmap.height, bitmap.bpp);
		this->BitmapToSurface(&bitmap);
		
		return TRUE;
	}*/

	return TRUE;
}


BOOL CScreen::LoadBMP(CXDraw *xdraw, char *filename, GENERICBITMAP *bitmap)
{
	HBITMAP	hBitmap;
    HDC		hdcImage;
    HDC		hdc;
    BITMAP	bm;
	DWORD	width, height;

	hBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBitmap == NULL)
       return FALSE;

    hdcImage = CreateCompatibleDC(NULL);
    SelectObject(hdcImage, hBitmap);

    GetObject(hBitmap, sizeof(bm), &bm);
    width  = bm.bmWidth;
    height = bm.bmHeight;
	bpp	   = bm.bmBitsPixel;

	this->CreateScreen(xdraw, width, height, bpp);

    this->lpdds->GetDC(&hdc);
    BitBlt(hdc, 0, 0, width, height, hdcImage, 0, 0, SRCCOPY);
    this->lpdds->ReleaseDC(hdc);
    DeleteDC(hdcImage);

	return TRUE;
}


BOOL CScreen::FlipBitmap(GENERICBITMAP *bitmap)
{
	DWORD i;
	int bytesperline = (bitmap->bpp >> 3) * bitmap->width;
	LPBYTE tempbuf = new UCHAR[bytesperline * bitmap->height];
	
	if(!tempbuf) 
		return FALSE;
	
	memcpy(tempbuf, bitmap->buffer, bytesperline * bitmap->height);
	
	for(i=0;i<bitmap->height;i++)
		memcpy(&bitmap->buffer[((bitmap->height-1) - i) * bytesperline],
			   &tempbuf[i*bytesperline], bytesperline);

	delete [] tempbuf;		 
	
	return TRUE;
}


inline void CScreen::Clear(xdCOLOR color)
{
	ddbltcolorfx.dwFillColor = color;
	
	lpdds->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltcolorfx);
}


// Puts a pixel on the screen!!!
inline void CScreen::PutPixel(int x, int y, xdCOLOR color)
{			
	/* NOTES FOR OPTIMIZATION: 
	 * 1) replace (vidbuf + (y * pitch)) with array of ptrs to each line
	 * 2) with no 24bpp, can optimize (x * bytespp) with a bit shift 
	 * 3) no memcpy - use another method (remember must be inline) */

	memcpy((vidbuf + (y * pitch) + (x * bytespp)), &color, bytespp);
/*LPVOID vidaddr = (vidbuf + (y * pitch) + (x * bytespp));
int bytes = bytespp;

	_asm
	{
		cld
		mov	EDI,vidaddr
		mov	EBX,color
		mov	ECX,bytes
		mov	EAX,ECX
		
		shr	ECX,2
		rep	movsd

	TestByte:
		and	EAX,001h
		jz	TestWord
		mov	ES:[EDI], BYTE DS:[ESI]
		inc	EDI
		inc	ESI

	TestWord:
		and	EAX,010h
		jz	End
		mov	ES:[EDI], WORD DS:[ESI]

	End:		
	}*/
}


// locks the display surface -- required for pxl writing
BOOL CScreen::Lock(void)
{
	DDSURFDESC ddsd;		// surface description

	INIT_DDOBJ(ddsd);

	// lock it + return a ptr to the surface and wait until it locks
	if(FAILED(lpdds->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
		return FALSE;

	//scrinfo.pitch	 = ddsd.lPitch;		// store pitch, ptr, rect
	//scrinfo.vidbuf   = ddsd.lpSurface;

	vidbuf = (LPBYTE)ddsd.lpSurface;	// get ptr to the surface

	return TRUE;
}


// Unlock() the screen when done writing to it
BOOL CScreen::Unlock()
{
	// unlock it with the stored rect (NULL if none) from the Lock()
	if(FAILED(lpdds->Unlock(NULL)))
		return FALSE;

	scrinfo.vidbuf	 = NULL;	// these are unneeded + unreliable now
	
	return TRUE;
}


// GDI text  -- VERY SLOW! REPLACE (use when screen Unlocked())
BOOL CScreen::Text(int x, int y, char *text, COLORREF color)
{
	HDC gdc;	// Graphics Device Context
	
	if(FAILED(lpdds->GetDC(&gdc))) return FALSE;	// get the window's

	SetTextColor(gdc, color);
	SetBkMode(gdc, TRANSPARENT);		// set background TRANSPARENT

	TextOut(gdc, x, y, text, strlen(text));	// print the text

	if(FAILED(lpdds->ReleaseDC(gdc))) return FALSE;	// release the DC

	return TRUE;
}


// initializes CScreen internal data
BOOL CScreen::InitScreenData(void)
{
	DDSURFDESC  ddsd;		// surface description
	DDPIXFORMAT ddpf;		// pixel format

	INIT_DDOBJ(ddpf);		// clear pix format struct

	if(FAILED(lpdds->GetPixelFormat(&ddpf)))
		return FALSE;

	if(ddpf.dwFlags & DDPF_PALETTEINDEXED8)	// is it a 8-bit mode?
		scrinfo.bpp = 8;
	else if(ddpf.dwFlags & DDPF_RGB)		// ... or higher?
		scrinfo.bpp = ddpf.dwRGBBitCount;
	else
		return(FALSE);

	gbpp = scrinfo.bpp;				// set global bpp for xdRGB (REPLACE)

	INIT_DDOBJ(ddsd);				// clear surface description
	if(FAILED(lpdds->GetSurfaceDesc(&ddsd)))
		return FALSE;
	
	scrinfo.width  = ddsd.dwWidth;	// set scrinfo width, height, pitch
	scrinfo.height = ddsd.dwHeight;
	width  = ddsd.dwWidth;
	height = ddsd.dwHeight;
	pitch  = ddsd.lPitch;

	bpp	= scrinfo.bpp;				// set "enhanced" bpp (has 15-bit pos)
	if(bpp == 15) bpp = 16;			// "unenhance" bpp

	bytespp  = bpp >> 3;			// set bytes per pixel (/8)

	if(!SetupFunc())				// setup func pointers (NOT NEEDED!)
		return FALSE;
	
	return TRUE;
}


// create a primary chain of video buffers
BOOL CScreen::CreatePrimary(CXDraw *xdraw, CScreen *dblbuf, CScreen *trpbuf)
{
	int numbuffers = 0;		// to determine back buffer number
	DDSURFDESC ddsd;		// surface description

	// dblbuf must be defined for trpbuf to be
	if(dblbuf != NULL) numbuffers = 1;	
	if(dblbuf != NULL && trpbuf != NULL) numbuffers = 2;

	INIT_DDOBJ(ddsd);		// clear surface description

	ddsd.dwFlags			= DDSD_CAPS;			// Capibilities filled
	ddsd.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE; // is a primary

	if(numbuffers > 0)	// if there is a double buffer (+ poss. triple)
	{
		ddsd.dwBackBufferCount	 = numbuffers;
		// the BackBufferCount field is filled
		ddsd.dwFlags			|= DDSD_BACKBUFFERCOUNT; 
		// is complex (eg a chain) and will flip the screens (in vidmem)
		ddsd.ddsCaps.dwCaps		|= DDSCAPS_COMPLEX | DDSCAPS_FLIP;
	}

	// creates the surface (or screen)
	if(FAILED(xdraw->lpdd->CreateSurface(&ddsd, &lpdds, NULL)))
		return FALSE;

	// set XDraw ptr to primary for page flips
	xdraw->lpddsprimary = lpdds;
	
	if(!this->InitScreenData())		// inits the CScreen fields
		return FALSE;

	if(numbuffers > 0)				// obtain the back buffer
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

		if(FAILED(lpdds->GetAttachedSurface(&ddsd.ddsCaps, &dblbuf->lpdds)))	
			return FALSE;
		if(!dblbuf->InitScreenData())
			return FALSE;
	}
	if(numbuffers > 1)				// obtain the triple buffer
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

		if(FAILED(dblbuf->lpdds->GetAttachedSurface(&ddsd.ddsCaps, &trpbuf->lpdds)))
			return FALSE;
		if(!trpbuf->InitScreenData())
			return FALSE;
	}

	return TRUE;
}


// setup pointers to functions
BOOL CScreen::SetupFunc(void)
{

	return TRUE;
}


// creates a generic off-surface screen
BOOL CScreen::CreateScreen(CXDraw *xdraw, DWORD width, DWORD height, DWORD bpp, UINT flags)
{
	DDSURFDESC ddsd;		// surface description
	DDCOLORKEY colorkey;	// color key

	INIT_DDOBJ(ddsd);

	ddsd.dwWidth			= width;
	ddsd.dwHeight			= height;
	ddsd.dwFlags			= DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwBackBufferCount	= 0;
	// a plain offscreen surface + memory storage flags
	ddsd.ddsCaps.dwCaps		= DDSCAPS_OFFSCREENPLAIN | flags;
	
	// create the surface
	if(FAILED(xdraw->lpdd->CreateSurface(&ddsd, &lpdds, NULL)))
		return FALSE;

	if(!this->InitScreenData())		// init the CScreen fields
		return FALSE;

	if(!this->SetTransparency(BLACK, BLACK))
		return FALSE;

	// set the source color key for the surface as black
	colorkey.dwColorSpaceLowValue  = BLACK;
	colorkey.dwColorSpaceHighValue = BLACK;
	lpdds->SetColorKey(DDCKEY_SRCBLT, &colorkey);
	
	return TRUE;
}


CScreen::CScreen(void)
{
	lpdds = NULL;
}


CScreen::~CScreen(void)
{
	if(lpdds)
		lpdds->Release();
}


inline UINT CScreen::Width(void)
{
	return width;
}


inline UINT CScreen::Height(void)
{
	return height;
}


// returns the screen information structure
inline xdSCRINFO CScreen::GetScrInfo(void)
{
	return scrinfo;
}


