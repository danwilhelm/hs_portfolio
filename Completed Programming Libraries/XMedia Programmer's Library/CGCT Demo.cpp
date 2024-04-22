#include "XMedia.h"
#include "XMediaWin.h"

const int XRES	= 640;
const int YRES	= 480;
const int BPP	= 16;
int guynum = 0, frames = 0;

CXMedia	 XMedia;
CSurface primary, secondary;
CSurface backlayer, rubbish;
CSurface bannerlayer, bannerlayer2;
CSurface guyanim[6], newguyanim[6];
CMusic	 music;
CPan	 backpan, bannerpan, bannerpan2, rubbishpan;

int Init(void)
{
	MyResources();
	CreateFullScreen("CGCT Promotion Demo");

	if(!XMedia.Init())
		xmError("DirectX 7.0 or higher is required.\nXDraw.Init() failed");

	ShowCursor(FALSE);

	if(!XMedia.SetMode(XRES, YRES, BPP))
		xmError("XDraw.SetMode() failed");

	if(!primary.CreatePrimary(&XMedia, &secondary))
		xmError("CreatePrimary() failed");

	if(!music.Create(&XMedia, 10))
		xmError("music.Create failed!");

	if(!music.Load("Mission Impossible.mid", 0))
		xmError("music.Load failed!");

	if(!music.Play(0))
		xmError("music.Play failed!");

	if(!backlayer.Load(&XMedia, "citybg2.bmp"))
		xmError("primary.Load failed!");

	if(!bannerlayer.Load(&XMedia, "banner.bmp"))
		xmError("primary.Load failed!");

	if(!bannerlayer2.Load(&XMedia, "banner2.bmp"))
		xmError("primary.Load failed!");

	if(!rubbish.Load(&XMedia, "rubbish.bmp"))
		xmError("primary.Load failed!");

	if(!guyanim[0].Load(&XMedia, "guy0.bmp"))
		xmError("No Load,");
	if(!guyanim[1].Load(&XMedia, "guy1.bmp"))
		xmError("No Load,");
	if(!guyanim[2].Load(&XMedia, "guy2.bmp"))
		xmError("No Load,");
	if(!guyanim[3].Load(&XMedia, "guy3.bmp"))
		xmError("No Load,");
	if(!guyanim[4].Load(&XMedia, "guy4.bmp"))
		xmError("No Load,");

	if(!newguyanim[0].Load(&XMedia, "newguy0.bmp"))
		xmError("No Load,");
	if(!newguyanim[1].Load(&XMedia, "newguy1.bmp"))
		xmError("No Load,");
	if(!newguyanim[2].Load(&XMedia, "newguy2.bmp"))
		xmError("No Load,");
	if(!newguyanim[3].Load(&XMedia, "newguy3.bmp"))
		xmError("No Load,");
	if(!newguyanim[4].Load(&XMedia, "newguy4.bmp"))
		xmError("No Load,");


	//if(!forelayer.Load(&XDraw, "northbg3.bmp"))
	//	xmError("primary.Load failed!");
	//forelayer.SetTransparency(xmRGB(2, 53, 202), xmRGB(2, 53, 202));

	RECT panspace = {0, 0, XRES, YRES};
	backpan.Create(&secondary, &backlayer, &panspace, 1, PAN_RIGHTTOLEFT | PAN_OPAQUE);

	RECT panspace2 = {66, 0, 600, 130};
	bannerpan.Create(&backlayer, &bannerlayer, &panspace2, 2, PAN_RIGHTTOLEFT | PAN_OPAQUE);

	RECT panspace3 = {0, YRES - 25, 640, YRES};
	bannerpan2.Create(&secondary, &bannerlayer2, &panspace3, 3, PAN_BOTTOMTOTOP | PAN_OPAQUE);

	RECT panspace4 = {0, 410, 640, YRES};
	rubbishpan.Create(&secondary, &rubbish, &panspace4, 5, PAN_RIGHTTOLEFT | PAN_OPAQUE);

	return TRUE;
}


int Main(void)
{
	backpan.Pan();
	bannerpan.Pan();
	rubbishpan.Pan();
	//bannerpan2.Pan();
	secondary.Blit(&guyanim[guynum]	, 138,175);
	secondary.Blit(&newguyanim[guynum], 351,175);
	
	if(++frames > 3)
	{
		if(++guynum > 4) 
			guynum = 0;
		frames = 0;
	}

	XMedia.Flip();

	if(!music.IsPlaying(0)) music.Play(0);

	XMedia.GetInput();

	if(ButtonDown(DIK_ESCAPE))
	   SendMessage(win_hwnd,WM_CLOSE,0,0);

	return TRUE;
}


int Shutdown(void)
{
	return TRUE;
}
