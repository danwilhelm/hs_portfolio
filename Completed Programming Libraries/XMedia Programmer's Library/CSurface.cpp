/************************************************************************
	CSurface.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


#include "CBitmap.h"
#include <fstream.h>


HRESULT CSurface::Clip(LPRECT rects, int numrects)
{
	int i;
	LPRGNDATA regions;

	if(!lpddclipper)
	{
		// Create a clipper object
		if(FAILED(xmedia->lpdd->CreateClipper(0, &lpddclipper, NULL)))
			return XMGENERIC;
	}
	else
	{
		// Detach current clipper
		if(FAILED(lpdds->SetClipper(NULL)))
			return XMGENERIC;
	}

	// allocate memory for the regions and copy the rects into it
	regions = (RGNDATA *)new char[sizeof(RGNDATAHEADER) + numrects * sizeof(RECT)];
	CopyMemory(regions->Buffer, rects, numrects * sizeof(RECT));

	// initialize the headers
	regions->rdh.dwSize   = sizeof(RGNDATAHEADER); 
	regions->rdh.iType	  = RDH_RECTANGLES;
	regions->rdh.nCount   = numrects;
	regions->rdh.nRgnSize = numrects * sizeof(RECT);
	
	regions->rdh.rcBound.top	= 0;
	regions->rdh.rcBound.bottom = 0;
	regions->rdh.rcBound.right	= 0;
	regions->rdh.rcBound.left	= 0;

	// find the max and min for the bounding rect of all rects
	for(i=0;i<numrects;i++)
	{
		if(rects[i].top > regions->rdh.rcBound.top)
			regions->rdh.rcBound.top = rects[i].top;

		if(rects[i].bottom < regions->rdh.rcBound.bottom)
			regions->rdh.rcBound.bottom = rects[i].bottom;

		if(rects[i].right > regions->rdh.rcBound.right)
			regions->rdh.rcBound.right = rects[i].right;

		if(rects[i].left < regions->rdh.rcBound.left)
			regions->rdh.rcBound.left = rects[i].left;
	}

	
	// set the clip list with above intialization data
	if(FAILED(lpddclipper->SetClipList(regions, 0)))
	{
		delete [] regions;
		return FALSE;
	}

	delete [] regions;

	// set the clipper object onto the surface
	if(FAILED(lpdds->SetClipper(lpddclipper)))
		return XMGENERIC;

	return XMSUCCESS;
}



inline void CSurface::Rect(int x0, int y0, int x1, int y1, xmCOLOR color)
{
	this->Line(x0, y0, x1, y0, color);
	this->Line(x1, y0, x1, y1, color);
	this->Line(x1, y1, x0, y1, color);
	this->Line(x0, y1, x0, y0, color);	
}


// FIXME: MISTAKE! MAKE AS AN ADD-ON IN CPolygon!
void CSurface::Poly(CPolygon *poly)
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


// uses Bresenham's line algorithm
void CSurface::Line(int x0, int y0, int x1, int y1, xmCOLOR color)
{
	int i;
	int incx, incy;
	int xdist2, ydist2;
	int error;

	int xdist	= x1 - x0;
	int ydist	= y1 - y0;

	UCHAR *vid	= vidbuf + (y0 * pitch) + (x0 * bytespp);	// calc vid offset

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

	if(xdist > ydist)						// the line is horizontal > vertical
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
	else									// the line is vertical > horizontal
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


inline void CSurface::Blit(CSurface *surf, int x, int y, int colorfx)
{
	RECT destrect = {x, y, x + surf->Width(), y + surf->Height()};
	
	lpdds->Blt(&destrect, surf->lpdds, NULL, colorfx | DDBLTFAST_WAIT, NULL);
}


inline void CSurface::BlitScaled(CSurface *surf, int x, int y, int xwidth, int yheight, int colorfx)
{
	RECT destrect = {x, y, x + xwidth, y + yheight};

	lpdds->Blt(&destrect, surf->lpdds, NULL, colorfx | DDBLT_WAIT, &ddbltcolorfx);
}


inline void CSurface::BlitX(CSurface *surf, LPRECT dest, LPRECT src, int colorfx, int rot)
{
	if(rot)	// for hardware ONLY rotation
	{
		ddbltcolorfx.dwRotationAngle = rot;
		colorfx |= DDBLT_ROTATIONANGLE;
	}

	lpdds->Blt(dest, surf->lpdds, src, colorfx | DDBLT_WAIT, &ddbltcolorfx);
}


HRESULT CSurface::SetTransparency(DWORD low, DWORD high)
{
	DWORD colorrange = NULL;
	DDCOLORKEY colorkey;

	colorkey.dwColorSpaceLowValue	= low;
	colorkey.dwColorSpaceHighValue	= high;

	if(low != high)						// a range only if values are dissimilar
		colorrange |= DDCKEY_COLORSPACE;
	
	if(FAILED(lpdds->SetColorKey(colorrange | DDCKEY_SRCBLT, &colorkey)))
		return XMGENERIC;

	return XMSUCCESS;
}


// TO BE USED FOR EXTERNAL BITMAP UNCOMPRESSERS ONLY!
// copies a bitmap to a surface to take advantage of acceleration
HRESULT CSurface::BitmapToSurface(xmGENERICBITMAP *bitmap)
{
	DWORD i;
	int bytesperline = (bitmap->bpp >> 3) * bitmap->width;
	LPBYTE lpsource = bitmap->buffer;
	LPBYTE lpdest;

	this->Lock();

	lpsource = bitmap->buffer;
	lpdest	 = vidbuf;

	for(i=0;i<bitmap->height;i++)		// copy it, line by line :)
	{
		memcpy(lpdest, lpsource, bytesperline);

		lpdest	 += pitch;
		lpsource += bytesperline;
	}

	this->Unlock();

	return XMSUCCESS;
}


HRESULT CSurface::Load(CXMedia *xmedia, char *filename)
{
	//int i;
	char *extension = NULL;
	char *filenamecopy = filename;
	xmGENERICBITMAP bitmap;
	
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
		return XMGENERIC;

	//for(i=0;i<3;i++)
	//	extension[i] = toupper(extension[i]);

	//xmError(extension);

	//if(extension[0] == 'B' && 
	//   extension[1] == 'M' && 
	//   extension[2] == 'P')
	//{
		if(LoadBMP(xmedia, filename, &bitmap))
			return XMGENERIC;
	//}

	
	/*if(bitmap.buffer != NULL)
	{
		this->CreateScreen(XMedia, bitmap.width, bitmap.height, bitmap.bpp);
		this->BitmapToSurface(&bitmap);
		
		return TRUE;
	}*/

	return XMSUCCESS;
}


// NOTE: uses standard Windows GDI and Windows SDK calls
HRESULT CSurface::LoadBMP(CXMedia *xmedia, char *filename, xmGENERICBITMAP *bitmap)
{
	HBITMAP	hBitmap;
    HDC		hdcImage;
    HDC		hdc;
    BITMAP	bm;
	DWORD	width, height;

	// load generic image
	hBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBitmap == NULL)
       return XMNOFILE;

    hdcImage = CreateCompatibleDC(NULL);	// get a DC
    SelectObject(hdcImage, hBitmap);

    GetObject(hBitmap, sizeof(bm), &bm);	// select the obj + get info
    width  = bm.bmWidth;
    height = bm.bmHeight;
	bpp	   = bm.bmBitsPixel;

	this->CreateSurface(xmedia, width, height, bpp);	// new screen!

    this->lpdds->GetDC(&hdc);
    BitBlt(hdc, 0, 0, width, height, hdcImage, 0, 0, SRCCOPY);	// copy it
    this->lpdds->ReleaseDC(hdc);
    DeleteDC(hdcImage);

	return XMSUCCESS;
}


// TO BE USED FOR EXTERNAL BITMAP UNCOMPRESSERS ONLY!
// some bitmaps must be flipped before being saved
HRESULT CSurface::FlipBitmap(xmGENERICBITMAP *bitmap)
{
	DWORD i;
	int bytesperline = (bitmap->bpp >> 3) * bitmap->width;
	LPBYTE tempbuf = new UCHAR[bytesperline * bitmap->height];
	
	if(!tempbuf) 
		return XMNOMEM;
	
	memcpy(tempbuf, bitmap->buffer, bytesperline * bitmap->height);
	
	for(i=0;i<bitmap->height;i++)
		memcpy(&bitmap->buffer[((bitmap->height-1) - i) * bytesperline],
			   &tempbuf[i*bytesperline], bytesperline);

	delete [] tempbuf;		 
	
	return XMSUCCESS;
}


// clears the screen a desired color
inline void CSurface::Clear(xmCOLOR color)
{
	ddbltcolorfx.dwFillColor = color;
	
	lpdds->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltcolorfx);
}


// Puts a pixel on the screen!!!
inline void CSurface::PutPixel(int x, int y, xmCOLOR color)
{			
	/* NOTES FOR OPTIMIZATION: 
	 * 1) replace (vidbuf + (y * pitch)) with array of ptrs to each line
	 * 2) with no 24bpp, can optimize (x * bytespp) with a bit shift 
	 * 3) no memcpy - use another method (remember must be inline) */

	memcpy((vidbuf + (y * pitch) + (x * bytespp)), &color, bytespp);

// FIXME: this might work, eventually...
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
HRESULT CSurface::Lock(void)
{
	DDSURFDESC ddsd;		// surface description

	INIT_DDOBJ(ddsd);

	// lock it + return a ptr to the surface and wait until it locks
	if(FAILED(lpdds->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
		return XMGENERIC;

	//scrinfo.pitch	 = ddsd.lPitch;		// store pitch, ptr, rect
	//scrinfo.vidbuf   = ddsd.lpSurface;

	vidbuf = (LPBYTE)ddsd.lpSurface;	// get ptr to the surface

	return XMSUCCESS;
}


// Unlock() the screen when done writing to it
HRESULT CSurface::Unlock()
{
	// unlock it with the stored rect (NULL if none) from the Lock()
	if(FAILED(lpdds->Unlock(NULL)))
		return XMGENERIC;

	scrinfo.vidbuf	 = NULL;	// these are unneeded + unreliable now
	
	return XMSUCCESS;
}


// GDI text  -- VERY SLOW! REPLACE (use when screen Unlocked())
HRESULT CSurface::Text(int x, int y, char *text, COLORREF color)
{
	HDC gdc;	// Graphics Device Context
	
	if(FAILED(lpdds->GetDC(&gdc))) return XMGENERIC;	// get the window's

	SetTextColor(gdc, color);
	SetBkMode(gdc, TRANSPARENT);		// set background TRANSPARENT

	TextOut(gdc, x, y, text, strlen(text));	// print the text

	if(FAILED(lpdds->ReleaseDC(gdc))) return XMGENERIC;	// release the DC

	return XMSUCCESS;
}


// initializes CSurface internal data
HRESULT CSurface::InitSurfaceData(void)
{
	DDSURFDESC  ddsd;		// surface description
	DDPIXFORMAT ddpf;		// pixel format

	INIT_DDOBJ(ddpf);		// clear pix format struct

	if(FAILED(lpdds->GetPixelFormat(&ddpf)))
		return XMGENERIC;

	if(ddpf.dwFlags & DDPF_PALETTEINDEXED8)	// is it a 8-bit mode?
		scrinfo.bpp = 8;
	else if(ddpf.dwFlags & DDPF_RGB)		// ... or higher?
		scrinfo.bpp = ddpf.dwRGBBitCount;
	else
		return XMGENERIC;

	gbpp = scrinfo.bpp;				// set global bpp for xmRGB (REPLACE)

	INIT_DDOBJ(ddsd);				// clear surface description
	if(FAILED(lpdds->GetSurfaceDesc(&ddsd)))
		return XMGENERIC;
	
	scrinfo.width  = ddsd.dwWidth;	// set scrinfo width, height, pitch
	scrinfo.height = ddsd.dwHeight;
	width  = ddsd.dwWidth;
	height = ddsd.dwHeight;
	pitch  = ddsd.lPitch;

	bpp	= scrinfo.bpp;				// set "enhanced" bpp (has 15-bit pos)
	if(bpp == 15) bpp = 16;			// "unenhance" bpp

	bytespp  = bpp >> 3;			// set bytes per pixel (/8)

	if(SetupFunc())				// setup func pointers (NOT NEEDED!)
		return XMGENERIC;

	// set the source color key for the surface as black
	this->SetTransparency(BLACK, BLACK);
	this->Clear(BLACK);
	
	return XMSUCCESS;
}


// create a primary chain of video buffers
HRESULT CSurface::CreatePrimary(CXMedia *xmediain, int backbuffers)
{
	DDSURFDESC ddsd;		// surface description
	HRESULT err;
	int tempbuffers = backbuffers;

	INIT_DDOBJ(ddsd);		// clear surface description

	ddsd.dwFlags			= DDSD_CAPS;			// Capibilities filled
	ddsd.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE; // is a primary

	if(backbuffers > 0)	// if there is a double buffer (+ poss. a triple)
	{
		ddsd.dwBackBufferCount = tempbuffers;
		// the BackBufferCount field is filled
		ddsd.dwFlags			|= DDSD_BACKBUFFERCOUNT; 
		// is complex (eg a chain) and will flip the screens (in vidmem)
		ddsd.ddsCaps.dwCaps		|= DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_VIDEOMEMORY;
	}

	xmedia = xmediain;

	// creates the surface
	err = xmedia->lpdd->CreateSurface(&ddsd, &xmedia->lpddsprimary, NULL);

	while(err != DD_OK)
	{
		// decrement the number of buffers every loop
		if(--tempbuffers < 1 && ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY)
		{
			// now try again, this time not in video memory
			ddsd.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
			tempbuffers = backbuffers;
		}

		// if < 1 and it isn't in video memory
		if(tempbuffers < 1 && !(ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY))
		{
			// try one final time with only a primary screen in video memory
			ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
			ddsd.dwBackBufferCount = tempbuffers;

			err = xmedia->lpdd->CreateSurface(&ddsd, &xmedia->lpddsprimary, NULL);

			if(err != DD_OK)
			{
				ddsd.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
						
				// and now (last chance) with only a primary in regular memory
				err = xmedia->lpdd->CreateSurface(&ddsd, &xmedia->lpddsprimary, NULL);
				
				if(err != DD_OK)
					return XMGENERIC;
			}

		}
		
		ddsd.dwBackBufferCount = tempbuffers;
		// creates the surface
		err = xmedia->lpdd->CreateSurface(&ddsd, &xmedia->lpddsprimary, NULL);
	};

	
	if(backbuffers > 0)				// obtain the back buffer(s)
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

		if(FAILED(xmedia->lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps, &xmedia->lpddssecondary)))
			return XMGENERIC;
		
		lpdds = xmedia->lpddssecondary;
	}
	else
	{
		lpdds = xmedia->lpddsprimary;

	}

	// clear all back buffers to black
	for(int i=0;i<=backbuffers;i++)
	{
		if(i == 0 && !win_windowed)
			this->Clear(BLACK);
		
		if(backbuffers) 
			xmedia->Flip();
	}
	
	if(this->InitSurfaceData())	// inits the CSurface fields
		return XMGENERIC;

	return XMSUCCESS;
}


// setup pointers to functions
HRESULT CSurface::SetupFunc(void)
{

	return XMSUCCESS;
}


// creates a generic off-surface screen
HRESULT CSurface::CreateSurface(CXMedia *xmediain, DWORD width, DWORD height, DWORD bpp, UINT flags)
{
	DDSURFDESC ddsd;		// surface description

	INIT_DDOBJ(ddsd);

	ddsd.dwWidth			= width;
	ddsd.dwHeight			= height;
	ddsd.dwFlags			= DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwBackBufferCount	= 0;
	// a plain offscreen surface + memory storage flags
	ddsd.ddsCaps.dwCaps		= DDSCAPS_OFFSCREENPLAIN | flags;
	
	xmedia = xmediain;

	// create the surface
	if(FAILED(xmedia->lpdd->CreateSurface(&ddsd, &lpdds, NULL)))
		return XMGENERIC;

	if(this->InitSurfaceData())		// init the CSurface fields
		return XMGENERIC;

	return XMSUCCESS;
}


CSurface::CSurface(void)
{
	lpdds		= NULL;
	lpddclipper = NULL;
	xmedia		= NULL;
}


CSurface::Release(void)
{
	if(lpddclipper)
		lpddclipper->Release();
	
	if(lpdds)
		lpdds->Release();
}


inline UINT CSurface::Width(void)
{
	return width;
}


inline UINT CSurface::Height(void)
{
	return height;
}


// returns the screen information structure
inline xmSCRINFO CSurface::GetScrInfo(void)
{
	return scrinfo;
}


