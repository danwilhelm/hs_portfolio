/* XDrawWin.H -- XDraw Implementation Library		*
 * by Daniel Wilhelm								*/

#ifndef XDRAWWIN
#define XDRAWWIN

#define WIN32_LEAN_AND_MEAN			// no MFC -- just use the SDK

#include <windows.h>				// standard windows headers
#include <windowsx.h>

// some options (use w/CreateWindowed)
// class name unimportant
const LPCTSTR	WIN_CLASSNAME	= "WINCLASS1";
// for a normal windowed app (no resize)
const DWORD		WIN_NORMAL		= WS_OVERLAPPED | WS_VISIBLE;
// only a minimize button (no max)
const DWORD		WIN_MINONLY		= WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE;
// no title bar! just a black rectangle
const DWORD		WIN_NOTITLE		= WS_POPUP | WS_VISIBLE;

// GLOBALS
int		win_xres			= GetSystemMetrics(SM_CXSCREEN);
int		win_yres			= GetSystemMetrics(SM_CYSCREEN);

BOOL	win_classreg		= FALSE;	// registered the class?
BOOL	win_windowmade		= FALSE;	// created the window?
BOOL	win_windowed		= FALSE;	// is it windowed/full screen?

HWND      win_hwnd			= NULL;		// handle to window
HINSTANCE win_hinstance		= NULL;		// instance of app

int CreateWindowed(LPCTSTR title = "XDraw graphics library",
   				   int x = win_xres/6,
				   int y = win_yres/6,
				   int width = win_xres*2/3,
				   int height = win_yres*2/3,
				   DWORD style = WIN_NORMAL);

int CreateFullScreen(LPCTSTR title = "XDraw graphics library");

int MyResources(LPCTSTR icon = NULL, LPCTSTR smicon = NULL, 
				LPCTSTR cursor = NULL, LPCTSTR menu = NULL);
int Init(void);
int Shutdown(void);
int Main(void);

#include "XDrawWin.cpp"

#endif
