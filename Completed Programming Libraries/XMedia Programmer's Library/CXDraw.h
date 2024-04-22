#include "XInput.h"

class CXDraw
{
	friend class CScreen;
	friend class CMusic;

	private:
		BOOL InitDDraw(void);
		BOOL InitDInput(void);
		BOOL InitDSound(void);
		BOOL InitDMusic(void);
		
	protected:
		LPDIRECTDRAW	lpdd;
		LPDSOUND		lpds;
		LPDINPUT		lpdi;
		DMPERF			*lpdmp;

		LPDDSURFACE		lpddsprimary;
		LPDDPALETTE		lpddpal;

		LPDIDEVICE		lpdikey;
		LPDIDEVICE		lpdimouse;
		LPDIDEVICE		lpdijoy;

		DDBLTFX			ddbltcolorfx;
		RAWSTATE		rawstate;

		BOOL			musicloaders;
		
	public:
		BOOL GetInput(void);
		void MapKey(int keynum, LPVOID loc);
		
		BOOL ShiftPalette(int start = 0, int end = 255);
		BOOL UsePalette(PALCOLOR *palette);
		inline LPDIRECTDRAW GetDirectDraw(void);
		CXDraw(void);
		~CXDraw(void);
		BOOL Init(void);
		BOOL SetMode(DWORD width, DWORD height, DWORD bpp);
		inline void Flip(void);
};
