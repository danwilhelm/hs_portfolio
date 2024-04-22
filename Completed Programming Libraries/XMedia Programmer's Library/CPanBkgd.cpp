/************************************************************************
	CPanBkgd.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

void CPanBkgd::Pan(void)
{
	BOOL panbreak = FALSE;
	
	x -= movex;
	y -= movey;

	if(x <= 0)				x += src->Width();
	if(x >= src->Width())	x -= src->Width();
	if(y <= 0)				y += src->Height();
	if(y >= src->Height())	y -= src->Height();

	int xbreak = src->Width() - x;
	int ybreak = src->Height() - y;

	if(xbreak >= destwidth)	 xbreak = destwidth;
	if(ybreak >= destheight) ybreak = destheight;

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

	if(ybreak != destwidth)
	{
		RECT src2  = {x, 0, x + xbreak, destheight - ybreak};
		RECT dest2 = {panarea.left, panarea.top + ybreak, panarea.left + xbreak, panarea.bottom};
		
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

	if(panbreak == FALSE)
	{
		RECT src1  = {x, y, x + destwidth, y + destheight};
		
		dest->BlitX(src, &panarea, &src1, colorfx);
	}	


	/*if(x + destwidth >= src->Width())
	{
		int xbreak = src->Width() - x;

		RECT src1  = {x, y, src->Width(), y + destheight};
		RECT src2  = {0, y, destwidth - xbreak, y + destheight};

		RECT dest1 = {panarea.left, panarea.top, panarea.left + xbreak, panarea.bottom};  
		RECT dest2 = {panarea.left + xbreak, panarea.top, panarea.right, panarea.bottom};

		dest->BlitX(src, &dest1, &src1, colorfx);
		dest->BlitX(src, &dest2, &src2, colorfx);

		dest->Blit(src, 0, 0, BLIT_OPAQUE);
		
		dest->Lock();
		dest->Rect(src2.left, src2.top, src2.right, src2.bottom, xdRGB(255, 0, 0));
		dest->Rect(src1.left, src1.top, src1.right, src1.bottom, xdRGB(0, 255, 0));
		dest->Unlock();

		char buffer[100];
		sprintf(buffer, "%i", x);
		dest->Text(0, 0, buffer, RGB(255, 0, 0));
		if(x < 0) Sleep(100000);

		panbreak = TRUE;
	}
	
	if(y + destheight >= src->Height())
	{
		int ybreak = (y + destheight) - src->Height();

		RECT src1  = {x, y, x + destwidth, src->Height()};
		RECT src2  = {x, 0, x + destwidth, ybreak};

		RECT dest1 = {panarea.left, panarea.top, panarea.right, panarea.top + destheight - ybreak};
		RECT dest2 = {panarea.left, panarea.top + destheight - ybreak, panarea.right, panarea.bottom};
	
		dest->BlitX(src, &dest1, &src1, colorfx);
		dest->BlitX(src, &dest2, &src2, colorfx);

		dest->Blit(src, 0, 0, BLIT_OPAQUE);
		
		dest->Lock();
		dest->Rect(src2.left, src2.top, src2.right, src2.bottom, xdRGB(255, 0, 0));
		dest->Rect(src1.left, src1.top, src1.right, src1.bottom, xdRGB(0, 255, 0));
		dest->Unlock();

		panbreak = TRUE;
	}
	
	if(panbreak == FALSE)
	{
		RECT src1  = {x, y, x + destwidth, y + destheight};
		
		dest->BlitX(src, &panarea, &src1, colorfx);

		dest->Blit(src, 0, 0, BLIT_OPAQUE);
		
		dest->Lock();
		dest->Rect(src1.left, src1.top, src1.right, src1.bottom, xdRGB(0, 255, 0));
		dest->Unlock();

		char buffer[100];
		sprintf(buffer, "%i", x);
		dest->Text(0, 0, buffer, RGB(255, 0, 0));
		if(x < 0) Sleep(1000000);

	}*/
	
/*	if(movex)
	{
		RECT src1  = {destwidth - posx, 0, destwidth, destheight};
		RECT src2  = {0, 0, destwidth - posx, destheight};

		RECT dest1 = {panarea.left, panarea.top, panarea.left + posx, panarea.bottom};
		RECT dest2 = {panarea.left + posx, panarea.top, panarea.right, panarea.bottom};

		dest->BlitX(src, &dest1, &src1, colorfx);
		dest->BlitX(src, &dest2, &src2, colorfx);

		posx += movex;
		if(posx <= 0)				posx += panarea.right;
		if(posx >= panarea.right)	posx -= panarea.right;
	}


	if(movey)
	{
		RECT src1  = {0, 0, destwidth, destheight - posy};
		RECT src2  = {0, destheight - posy, destwidth, destheight};

		RECT dest1 = {panarea.left, panarea.top + posy, panarea.right, panarea.bottom};
		RECT dest2 = {panarea.left, panarea.top, panarea.right, panarea.top + posy};

		dest->BlitX(src, &dest1, &src1, colorfx);
		dest->BlitX(src, &dest2, &src2, colorfx);

		posy += movey;
		if(posy <= 0)				posy += panarea.bottom;
		if(posy >= panarea.bottom)	posy -= panarea.bottom;
	}*/
}


void CPanBkgd::Create(CScreen *scrn, CScreen *bitmap, LPRECT area, int speed, DWORD style)
{
	src  = bitmap;
	dest = scrn;
	memcpy(&panarea, area, sizeof(RECT));

	movex = movey = 0;
	if(style & PAN_LEFTTORIGHT)	movex = speed;
	if(style & PAN_RIGHTTOLEFT) movex = -speed;
	if(style & PAN_TOPTOBOTTOM) movey = speed;
	if(style & PAN_BOTTOMTOTOP) movey = -speed;

	if(style & PAN_TRANSPARENT) colorfx = BLIT_TRANSPARENT;
	if(style & PAN_OPAQUE)		colorfx = BLIT_OPAQUE;

	destwidth  = panarea.right  - panarea.left;
	destheight = panarea.bottom - panarea.top;

	x = y = 0;
}


CPanBkgd::CPanBkgd(void)
{
	src	 = NULL;
	dest = NULL;
	ZeroMemory(&panarea, sizeof(RECT));
}

