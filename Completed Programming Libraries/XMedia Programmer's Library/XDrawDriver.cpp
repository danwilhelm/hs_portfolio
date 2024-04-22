#include "XDraw.h"
#include "XDrawWin.h"

struct STAR
{
	int	 x;
	int	 y;
	int	 speed;
	BOOL down;
	UINT color;
};

#define MAX_STARS	1000
#define MAX_POLYS	10
#define MAX_VERT	6

int		 xres, yres;
CXDraw	 XDraw;
CScreen	 primary, secondary;
CScreen  bitmap;
CMusic	 music;
CPolygon poly[MAX_POLYS];
STAR	 star[MAX_STARS];
VERTEX2D vertices[MAX_VERT] = {{-40, 40}, {-20, -24}, {10, -40},
							   {40, 10} , {32, 36}  , {8, 50}};

int		chg = 0, dir = 2;
int		bitmapx = 0, bitmapy = 0;
int		sclx = 100, scldir = 1;
int		circx, circy, circrad = 200;
float	circdir;

// Init: when the app is started
int Init(void)
{
	PALCOLOR palette[256];
	
	xdSCRINFO ScrInfo;

	MyResources();
	CreateFullScreen("MyApp!!!");

	if(!XDraw.Init())
		xdError("DirectX 7.0 or higher is required.\nXDraw.Init() failed");

	if(!bitmap.Load(&XDraw, "thething.bmp"))
		xdError("primary.Load failed!");

	if(!music.Create(&XDraw, 10))
		xdError("music.Create failed!");

	if(!music.Load("Mission Impossible.mid", 0))
		xdError("music.Load failed!");

	if(!music.Play(0))
		xdError("music.Play failed!");

	ShowCursor(FALSE);

	if(!XDraw.SetMode(640, 480, 16))
		xdError("XDraw.SetMode() failed");

	if(!primary.CreatePrimary(&XDraw, &secondary))
		xdError("CreatePrimary() failed");

	ScrInfo = primary.GetScrInfo();
	xres = ScrInfo.width;
	yres = ScrInfo.height;

	for(int i=0;i<MAX_STARS;i++)
	{
		star[i].x		= rand()%xres;
		star[i].y		= rand()%yres;
		star[i].speed	= rand()%3 + 1;
		star[i].down	= rand()%2;
		star[i].color	= xdRGB(star[i].speed*85, star[i].speed*85, star[i].speed*85);
	}

	for(i=0;i<MAX_POLYS;i++)
	{
		poly[i].Create(vertices, MAX_VERT, xdRGB(rand()%255, rand()%255, rand()%255));
		poly[i].x = rand()%(xres - 40) + 20;
		poly[i].y = rand()%(yres - 40) + 20;

		if((poly[i].xv = rand()%2) == 0) poly[i].xv = -1;
		if((poly[i].yv = rand()%2) == 0) poly[i].yv = -1;

		poly[i].Scale(0.75, 0.75);
	}

	for(i=0;i<256;i++)
	{
		palette[i].red	 = i;
		palette[i].green = i;
		palette[i].blue	 = i;
	}

	XDraw.UsePalette(palette);

	return TRUE;
}


// Main: like a regular DOS main() function (also renders one frame)
int Main(void)
{
	int i;

	secondary.Clear(BLACK);

	XDraw.GetInput();

	if(!secondary.Lock())
		xdError("XDraw.Lock() failed");

	for(i=0;i<MAX_STARS;i++)
	{
		if(GetZAxis < 0 || GetYAxis < 0 || ButtonDown(MOUSE_CENTER))
		{
			star[i].y += star[i].speed;
			if(star[i].y > yres) star[i].y -= yres;
		}

		if(GetZAxis > 0 || GetYAxis > 0)
		{
			star[i].y -= star[i].speed;
			if(star[i].y < 0) star[i].y += yres;
		}

		if(GetXAxis < 0 || ButtonDown(MOUSE_LEFT))
		{
			star[i].x += star[i].speed;
			if(star[i].x > xres) star[i].x -= xres;
		}

		if(GetXAxis > 0 || ButtonDown(MOUSE_RIGHT))
		{
			star[i].x -= star[i].speed;
			if(star[i].x < 0) star[i].x += xres;
		}

		secondary.PutPixel(star[i].x, star[i].y, star[i].color);
	}

	if(!secondary.Unlock())
		xdError("CXDraw.Unlock() failed");

	if(ButtonDown(DIK_RIGHT))	bitmapx += 2;
	if(ButtonDown(DIK_LEFT))	bitmapx -= 2;
	if(ButtonDown(DIK_DOWN))	bitmapy += 2;
	if(ButtonDown(DIK_UP))		bitmapy -= 2;

	if(bitmapx < 0)				bitmapx = 0;
	if(bitmapx + sclx > xres)	bitmapx = xres - sclx;
	if(bitmapy < 0)				bitmapy = 0;
	if(bitmapy + sclx > yres)	bitmapy = yres - sclx;

	if(sclx + bitmapx + 10 > xres || sclx + bitmapy + 10 > yres)
		scldir = -1;

	if(sclx <= 0)
		scldir = 1;

	sclx += scldir;
		
	secondary.BlitScaled(&bitmap, bitmapx, bitmapy, sclx, sclx);

	if(!secondary.Lock())
		xdError("XDraw.Lock() failed");
	
	for(i=0;i<MAX_POLYS;i++)
	{
		poly[i].Rotate(6.28/180);
		poly[i].GetRect();

		if(poly[i].box.left <= 0)		poly[i].xv = 1;
		if(poly[i].box.right >= xres)	poly[i].xv = -1;
			
		if(poly[i].box.top  <= 0)		poly[i].yv = 1;
		if(poly[i].box.bottom >= yres)	poly[i].yv = -1; 

		secondary.Poly(&poly[i]);
	}
	
	for(i=0;i<yres;i++)
		secondary.PutPixel(i, yres-i, xdRGB(255-chg, 255-chg, 255-chg));
	for(i=yres;i>0;i--)
		secondary.PutPixel(yres-i, yres-i, xdRGB(255-chg, 255-chg, 255-chg));
	for(i=0;i<xres;i++)
		secondary.PutPixel(i, yres>>1, xdRGB(0, chg, 0));
	for(i=0;i<yres;i++)
		secondary.PutPixel(xres>>1, i, xdRGB(0, 0, chg));

	secondary.Line(0, 0, xres >> 1, 255 - chg, xdRGB(128, 255, 128));
	secondary.Line(xres, 0, xres >> 1, chg, xdRGB(255, 128, 255));
	secondary.Line(0, yres, xres >> 1, yres - (255-chg), xdRGB(128, 255, 255));
	secondary.Line(xres, yres, xres >> 1, yres - chg, xdRGB(255, 255, 128));

	circx = cos(circdir) * circrad;
	circy = sin(circdir) * circrad;
	secondary.Line(xres >> 1, yres >> 1, circx + (xres >> 1), circy + (yres >> 1), xdRGB(255, 0, 0));
	
	circdir += 0.025;
	if(circdir > 6.283) circdir = 0;

	chg += dir;
	if(chg <= 0)
	{
		dir = -dir;
		chg = 0;
	}
	if(chg >= 255) 
	{
		dir = -dir;
		chg = 255;
	}

	if(!secondary.Unlock())
		xdError("CXDraw.Unlock() failed");

	secondary.Text(xres - (17 << 3), yres - 20, "by Daniel Wilhelm", RGB(0, 255, 0));

	char buffer[100];
	sprintf(buffer, "%i", sclx);
	secondary.Text(0, 0, buffer, RGB(0, 0, 255));

	XDraw.Flip();

	Sleep(30);

	if(ButtonDown(DIK_ESCAPE))
	   SendMessage(win_hwnd,WM_CLOSE,0,0);

	return TRUE;
}


// Shutdown: when user exits the app
int Shutdown(void)
{
	return TRUE;
}

