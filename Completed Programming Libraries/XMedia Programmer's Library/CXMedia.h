/************************************************************************
	CXMedia.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


#include "XInput.h"

class CXMedia
{
	friend class CSurface;
	friend class CMusic;

	private:
		HRESULT InitDDraw(void);
		HRESULT InitDInput(int exclusions = 0);
		HRESULT InitDSound(void);
		HRESULT InitDMusic(void);
		
	public:
		LPDIRECTDRAW	lpdd;
		LPDSOUND		lpds;
		DMLOADER		*lpdml;
		LPDINPUT		lpdi;
		DMPERF			*lpdmp;

		LPDDSURFACE		lpddsprimary;
		LPDDSURFACE		lpddssecondary;
		LPDDPALETTE		lpddpal;

		LPDIDEVICE		lpdikey;
		LPDIDEVICE		lpdimouse;
		LPDIDEVICE		lpdijoy;

		DDBLTFX			ddbltcolorfx;
		RAWSTATE		rawstate;

		int				musicloaders;
		int				windowx, windowy;
		
	//public:
		HRESULT GetInput(void);
		void MapKey(int keynum, LPVOID loc);
		
		HRESULT ShiftPalette(int start = 0, int end = 255);
		HRESULT UsePalette(xmPALCOLOR *palette);
		inline LPDIRECTDRAW GetDirectDraw(void);
		CXMedia(void);
		Release(void);
		HRESULT Create(int exclusions = 0);
		HRESULT SetMode(CSurface *surface, DWORD width, DWORD height, DWORD bpp, int backbuffers = 1);
		inline void Flip(void);
};
