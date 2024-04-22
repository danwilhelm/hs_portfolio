/************************************************************************
	CPan.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	This code may be freely used for any purpose, commercial or private.

	The user must acknowledge, however, that the author is not responsible 
	for any damages the following code incurs on his or her system and is 
	not liable for any damages that the code brings about. Use this code 
	at your own risk!
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#define PAN_TRANSPARENT		1
#define PAN_OPAQUE		2

class CPan
{
	private:
		CSurface *src;
		CSurface *dest;

		RECT	panarea;
		int	xspeed, yspeed;
		DWORD	destwidth, destheight;
		DWORD	colorfx;

		UINT	x, y;

	public:
		CPan(void);
		
		void Create(CSurface *surf, CSurface *bitmap, LPRECT area, int xv = 1, int yv = 0, DWORD style = PAN_TRANSPARENT);
		void Pan(void);
};