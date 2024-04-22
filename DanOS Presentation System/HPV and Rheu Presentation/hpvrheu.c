#include <time.h>
#include "vesa.c"

#define TICS_PER_MILLI	UCLOCKS_PER_SEC/1000
#define MAXANS			6

void SlideBounce(char *backgr, char *imga, char *imgb, int miny);
void DanOSLogo(void);
void ScreenSaver(char *filename);
void ShowSlide(char *filename);
void Quiz(char *backgrpic, char *anspic, short ansx, short ansy);
void setpal(RGB pal[]);
void wait(uclock_t millisec);
void execprog(char *filename);

RGB Pal[256];

// THE QUIZ UNDERLINE POSITIONS
short anspos[8] = {22,314, 337,314, 22,411, 337,411};

int main(void)
{
    uclock();
	VESA_Init(0x101);

    DanOSLogo();
    ShowSlide("HPVINTRO.PCX");
	Quiz("ques2.PCX", "ans1.PCX", 8,373);

    ShowSlide("HPVIMAGES.PCX");
    ShowSlide("HPVTREAT.PCX");
    SlideBounce("HPVPAP.PCX", "SMEAR1.PCX", "SMEAR2.PCX", 130);

    execprog("LIFE078");

    ShowSlide("COCIMAGES.PCX");
    ShowSlide("RHEUSYMPTOMS.PCX");
    ShowSlide("RHEUTREAT.PCX");
    ShowSlide("SIMDIF.PCX");

	return(0);
}

void SlideBounce(char *backgr, char *imga, char *imgb, int miny)
{
    int x, y, gutter = 13, direction = 1;
	PICBUF slide, slideb, slidec;

	Get_PCX(backgr, &slide, Pal);
    Get_PCX(imga, &slideb, Pal);
    Get_PCX(imgb, &slidec, Pal);

    setpal(Pal);

    x = gutter + slidec.width + gutter;
    y = ((VESA_YRes - miny) >> 1) - (slideb.height >> 1) + miny;

    VESAC_Put(0,0, slide);

    while(!_bios_keybrd(_KEYBRD_READY))
    {
        wait(10);
    	VESAC_Put(gutter,y, slideb);
        VESAC_Put(x,VESA_YRes - miny - y, slidec);

        y += direction;
        if(y + slideb.height > VESA_YRes)
        	direction = -direction;
        if(y < miny)
        	direction = -direction;
    }

    _bios_keybrd(_KEYBRD_READ);

    Remove_PCX(&slide);
    Remove_PCX(&slideb);
    Remove_PCX(&slidec);
}


void DanOSLogo(void)
{
    int i;
	PICBUF danOS;
    RGB rgb;

	Get_PCX("DANOS.PCX", &danOS, Pal);

    setpal(Pal);
    VESAC_Put(0,0, danOS);

    for(i=68;i<68+488;i++)
    {
    	VESAC_VertLine(i, 387, 416, 30);
    	wait(10);
    }

    rgb.red   = 63;
    rgb.blue  = 63;
    rgb.green = 63;
    for(i=0;i<63;i++)
    {
        rgb.red--;
        rgb.green--;
    	VESA_SetPalette(102, rgb);
        wait(50);
    }

    _bios_keybrd(_KEYBRD_READ);
    Remove_PCX(&danOS);
}

void ScreenSaver(char *filename)
{
    short x, y, down = 1, right = 1;
	PICBUF HPV;

    Get_PCX(filename, &HPV, Pal);
    setpal(Pal);

    for(y=0;y<VESA_YRes;y++)
    	VESAA_FillY(y, 215);

    x = (VESA_XRes >> 1) - (HPV.width  >> 1);
    y = (VESA_YRes >> 1) - (HPV.height >> 1);

    while(!_bios_keybrd(_KEYBRD_READY))
    {
    	VESAC_Put(x, y, HPV);

        x += right;
        y += down;

        if(x <= 0 || x + HPV.width  >= VESA_XRes)
        	right = -right;
        if(y <= 0 || y + HPV.height >= VESA_YRes)
        	down = -down;

        wait(5);
    }

    _bios_keybrd(_KEYBRD_READ);
    Remove_PCX(&HPV);
}

void ShowSlide(char *filename)
{
	PICBUF slide;

	Get_PCX(filename, &slide, Pal);

    setpal(Pal);
    VESAC_Put(0,0, slide);

    _bios_keybrd(_KEYBRD_READ);

    Remove_PCX(&slide);
}

void Quiz(char *backgrpic, char *anspic, short ansx, short ansy)
{
    int i = 0;
	PICBUF backgr, ans, underlinrd, underlinbl;

	// EXTRACT PICS AND SET PALETTE
    Get_PCX(backgrpic, &backgr, Pal);
    Get_PCX(anspic, &ans, Pal);
    Get_PCX("underline.pcx", &underlinrd, Pal);
    Get_PCX("underline2.pcx", &underlinbl, Pal);

    setpal(Pal);
    VESAC_Put(0,0, backgr);

    // UNDERLINE QUIZ CHOICES RED
    while(!_bios_keybrd(_KEYBRD_READY))
    {
        VESAC_Put(anspos[i],anspos[i+1], underlinrd);
    	wait(500);
        VESAC_Put(anspos[i],anspos[i+1], underlinbl);

        i = (i >= MAXANS) ? 0 : i+2;	// if i >= MAXANS, i=0; else, i+=2;
    }

    _bios_keybrd(_KEYBRD_READ);

    VESAC_Put(ansx,ansy, ans);
    while(!_bios_keybrd(_KEYBRD_READY))
    {
        VESAC_Put(anspos[4],anspos[5], underlinrd);
    	wait(100);
        VESAC_Put(anspos[4],anspos[5], underlinbl);
		wait(100);
    }

    _bios_keybrd(_KEYBRD_READ);

    Remove_PCX(&backgr);
    Remove_PCX(&ans);
    Remove_PCX(&underlinrd);
    Remove_PCX(&underlinbl);
}


void execprog(char *filename)
{
	system(filename);
    //VESA_Init(0x101);
    VESA_SetMode(0x101);
    _bios_keybrd(_KEYBRD_READ);
}

void wait(uclock_t millisec)
{
    uclock_t endtime = (millisec*TICS_PER_MILLI) + uclock();

    while(uclock() <= endtime);
}

void setpal(RGB pal[])
{
    int i;

	for(i=0;i<256;i++)
    	VESA_SetPalette(i, pal[i]);
}




