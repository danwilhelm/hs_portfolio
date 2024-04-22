/************************************************************************
	CPan.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	This code may be freely used for any purpose, commercial or private.

	The user must acknowledge, however, that the author is not responsible 
	for any damages the following code incurs on his or her system and is 
	not liable for any damages that the code brings about. Use this code 
	at your own risk!
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

void CPan::Pan(void)
{
	BOOL panbreak = FALSE;	// if a break is found (either x or y)
	
	x -= xspeed;
	y -= yspeed;

	if(x <= 0)			x += src->Width();
	if(x >= src->Width())	x -= src->Width();
	if(y <= 0)			y += src->Height();
	if(y >= src->Height())	y -= src->Height();

	// calculate where the breaks are
	int xbreak = src->Width() - x;
	int ybreak = src->Height() - y;

	// if there is no break, then set the breaks to the extremes of the source
	if(xbreak >= destwidth)	 xbreak = destwidth;
	if(ybreak >= destheight) ybreak = destheight;

	// a vertical overlap
	if(xbreak != destwidth)
	{
		RECT src1  = {x, y, x + xbreak, y + ybreak};
		RECT src2  = {0, y, destwidth - xbreak, y + ybreak};

		RECT dest1 = {panarea.left, panarea.top, panarea.left + xbreak, panarea.top + ybreak};
		RECT dest2 = {panarea.left + xbreak, panarea.top, panarea.right, panarea.top + ybreak};

		dest->BlitX(src, &dest1, &src1, colorfx);
		dest->BlitX(src, &dest2, &src2, colorfx);

		panbreak = TRUE;
	}

	// a horizontal overlap
	if(ybreak != destwidth)
	{
		RECT src2  = {x, 0, x + xbreak, destheight - ybreak};
		RECT dest2 = {panarea.left, panarea.top + ybreak, panarea.left + xbreak, panarea.bottom};
		
		// for a 4-way break
		if(panbreak == TRUE)
		{
			RECT src1  = {0, 0, destwidth - xbreak, destheight - ybreak};
			RECT dest1 = {panarea.left + xbreak, panarea.top + ybreak, panarea.right, panarea.bottom};
		
			dest->BlitX(src, &dest1, &src1, colorfx);
		}
		else
		{
			RECT src1  = {x, y, x + destwidth, y + ybreak};
			RECT dest1 = {panarea.left, panarea.top, panarea.left + xbreak, panarea.top + ybreak};
		
			dest->BlitX(src, &dest1, &src1, colorfx);
		}

		dest->BlitX(src, &dest2, &src2, colorfx);
		panbreak = TRUE;
	}

	// if no break, just do a straight blit
	if(panbreak == FALSE)
	{
		RECT src1  = {x, y, x + destwidth, y + destheight};
		
		dest->BlitX(src, &panarea, &src1, colorfx);
	}	
}


void CPan::Create(CSurface *surf, CSurface *bitmap, LPRECT area, int xv, int yv, DWORD style)
{
	src  = bitmap;
	dest = surf;
	memcpy(&panarea, area, sizeof(RECT));

	// allow for easy word descriptions for the speed
	xspeed = yspeed = 0;
	xspeed = xv;
	yspeed = yv;

	// set-up transparency info
	if(style & PAN_TRANSPARENT) 	colorfx = BLIT_TRANSPARENT;
	if(style & PAN_OPAQUE)		colorfx = BLIT_OPAQUE;

	destwidth  = panarea.right  - panarea.left;
	destheight = panarea.bottom - panarea.top;

	x = y = 0;
}


CPan::CPan(void)
{
	src  = NULL;
	dest = NULL;
	ZeroMemory(&panarea, sizeof(RECT));
}

