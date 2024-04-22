/************************************************************************
	CPan.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


#define PAN_LEFTTORIGHT		1
#define PAN_RIGHTTOLEFT		2
#define PAN_TOPTOBOTTOM		4
#define PAN_BOTTOMTOTOP		8
#define PAN_TRANSPARENT		16
#define PAN_OPAQUE			32

class CPan
{
	private:
		CSurface *src;
		CSurface *dest;

		RECT	panarea;
		int		movex, movey;
		DWORD	destwidth, destheight;
		DWORD	colorfx;

		UINT	x, y;

	public:
		CPan(void);
		
		void Create(CSurface *surf, CSurface *bitmap, LPRECT area, int speed = 1, DWORD style = PAN_RIGHTTOLEFT | PAN_TRANSPARENT);
		void Pan(void);
};