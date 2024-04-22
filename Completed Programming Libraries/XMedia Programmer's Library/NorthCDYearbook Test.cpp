#include "XDraw.h"
#include "XDrawWin.h"

const int XRES	= 640;
const int YRES	= 480;
const int BPP	= 16;

CXDraw	 XDraw;
CScreen	 primary, secondary;
CScreen  backlayer;
CScreen	 forelayer;
CPanBkgd backpan, forepan, midpan, backpan2, forepan2;
//CMusic	 music;

int Init(void)
{
	MyResources();
	CreateFullScreen("CNHS CD-ROM Yearbook");

	if(!XDraw.Init())
		xmError("DirectX 7.0 or higher is required.\nXDraw.Init() failed");

	if(!backlayer.Load(&XDraw, "northbg2.bmp"))
		xmError("primary.Load failed!");

	if(!forelayer.Load(&XDraw, "northbg3.bmp"))
		xmError("primary.Load failed!");
	forelayer.SetTransparency(xdRGB(0, 49, 206), xmRGB(0, 49, 206));

	ShowCursor(FALSE);

	if(!XDraw.SetMode(XRES, YRES, BPP))
		xmError("XDraw.SetMode() failed");

	if(!primary.CreatePrimary(&XDraw, &secondary))
		xmError("CreatePrimary() failed");

	RECT panspace = {0, 0, XRES / 2, YRES / 2};
	backpan.Create(&secondary, &backlayer, &panspace, 1, PAN_TOPTOBOTTOM | PAN_OPAQUE);
	//midpan.Create (&secondary, &forelayer, &panspace, 5, PAN_TOPTOBOTTOM | PAN_TRANSPARENT);
	forepan.Create(&secondary, &forelayer, &panspace, 5, PAN_LEFTTORIGHT | PAN_TRANSPARENT);
	
	RECT panspace2 = {XRES / 2, YRES / 2, XRES - 1, YRES - 1};
	backpan2.Create(&secondary, &backlayer, &panspace2, 1, PAN_TOPTOBOTTOM | PAN_RIGHTTOLEFT | PAN_OPAQUE);
	forepan2.Create(&secondary, &forelayer, &panspace2, 5, PAN_RIGHTTOLEFT | PAN_TRANSPARENT);

	return TRUE;
}


int Main(void)
{
	secondary.Clear(BLACK);
	backpan.Pan();
	//midpan.Pan();
	forepan.Pan();
	backpan2.Pan();
	forepan2.Pan();

	XDraw.Flip();

	XDraw.GetInput();

	if(ButtonDown(DIK_ESCAPE))
	   SendMessage(win_hwnd,WM_CLOSE,0,0);

	return TRUE;
}


int Shutdown(void)
{
	return TRUE;
}
