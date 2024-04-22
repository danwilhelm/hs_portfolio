#include "XMedia.h"
#include "XMediaWin.h"

const int XRES	= 640;
const int YRES	= 480;
const int BPP	= 32;

CXMedia  XMedia;
CSurface screen, bitmap;

int x;

RECT cliprects[] = {{0,0, 30,40}, {40,20, 60,40}, {100,10, 200,25}, 
					{100,30, 200,45}, {250,20, 300, 40}, 
					{400,0, 410,60}, {420,0, 430,60}, 
					{440,0, 450,60}, {460,0, 470,60},
					{500,0, 640,60}};

RECT cliprects2[] = {{0,0, XRES, YRES}};

int Init(void)
{
	MyResources();
	CreateFullScreen("XMedia Test");

	if(!XMedia.Create())
		xmError("DirectX 7.0 or higher is required.\nXMedia.Create() failed");

	if(!XMedia.SetMode(&screen, XRES, YRES, BPP))
		xmError("XMedia.SetMode() failed");

	if(!screen.Clip(cliprects, 10))
		xmError("No clip");

	if(!bitmap.Load(&XMedia, "thething.bmp"))
		xmError("No load");

	x = -bitmap.Width();

	return TRUE;
}

int Main(void)
{
	if(!XMedia.GetInput())
		xmError("Input retrieval failed!");

	if(ButtonDown(DIK_ESCAPE))
		SendMessage(win_hwnd,WM_CLOSE,0,0);

	screen.Clear();

	screen.Blit(&bitmap, x, 0);

	if(x++ > XRES)
	{
		x = -bitmap.Width();
		if(!screen.Clip(cliprects2, 1))
			xmError("No clip");
	}

	XMedia.Flip();

	return TRUE;
}

int Shutdown(void)
{
	return TRUE;
}