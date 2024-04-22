/* VESA.C -- VESA LIBRARY							*
 * by Daniel Wilhelm								*
 * NOW ONLY SUPPORTS 2.0+							*
 * WORK ON: Linear Framebuffer, VESA 1.0+ support 	
 * MUST COMPILE WITH ASMVESA.ASM!!!						*/

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <dpmi.h>
#include <go32.h>
#include <bios.h>
#include <sys/nearptr.h>
#include <sys/segments.h>
#include "keyboard.h"
#include "vesa.h"
#include "pcx.c"

#define	MASK_LINEAR(addr)	  (addr	& 0x000FFFFF)
#define	RM_TO_LINEAR(addr)	  (((addr &	0xFFFF0000)	>> 12) + (addr & 0xFFFF))
#define	RM_OFFSET(addr)		  (addr	& 0xF)
#define	RM_SEGMENT(addr)	  ((addr >>	4) & 0xFFFF)

#define	LFB_ENABLE			  0x4000
#define KB_INT				  9

extern void KbdHandler(void);
extern void	VESAA_SetAll(RGB Pal[],	UCHAR StartIndex, UCHAR	EndIndex);
extern void	VESAA_FillPage(UCHAR page, UCHAR color);
extern void	VESA_SetPalette(UCHAR index, RGB Pal);//UCHAR red, UCHAR green, UCHAR blue);

extern void	SetPixel(PICBUF	Buffer,	short x, short y, char color);
extern void	CopyToScreen(PICBUF	Buffer);

void VESAC_Put(short startx, short starty, PICBUF buffer);
void VESAC_Circle(short	xtrans,	short ytrans, short	radius,	float xstr,	float ystr,	char color);

void LFB_SetPixel(PICBUF *Buffer, int x, int y,	int	color);
void InstallKeyboard(void);
void RemoveKeyboard(void);
void WaitForKey(void);

inline void	SetPix(PICBUF *Buffer, USHORT x, USHORT	y, UCHAR color);

char flagtable[128];
__dpmi_paddr OldKbHandler;

void (*VESA_SetPage)(short);
char *VESA_LFB;
char VESA_BPP;
PICBUF LFB;

int	main(int argc, char	*argv[])
{
	int	x =	10,	y =	11,	i = 0;			// just a quick test driver...
	int	right =	-1,	down = -1;
	PICBUF PCXPic;
	RGB	Pal[255];

	if(!VESA_Init(0x101))	{			// set mode 640x480x8
		printf("Mode not supported!");
		exit(0);
	}

	Get_PCX("THINK.PCX", &PCXPic, Pal);

	for(i=0;i<255;i++)				// old format below (use new one!):
		VESA_SetPalette(i, Pal[i]); //Pal[i].red, Pal[i].green, Pal[i].blue);

	//*(VESA_LFB + 0) = 31;				// linear framebuffer test
	//for(i=0;i<VESA_YRes;i++)
	//  LFB_SetPixel(&LFB, i, i, 31);

    for(i=0;i<VESA_YRes;i++)			// draw test pattern...
	    VESAA_SetPixel(i,i, 31);
    for(i=0;i<VESA_YRes;i++)
    	VESAA_SetPixel(VESA_YRes-i,i, 31);

    for(i=0;i<VESA_YRes;i+=5)
    	VESAA_FillY(i, 20);

    InstallKeyboard();
    WaitForKey();

	while(!flagtable[SC_ENTER])  {		// wait + bounce pic around...
		VESAC_Put(x, y,	PCXPic);

		x += right;
		y += down;

		if(x <=	0 || x + PCXPic.width >= VESA_XRes)
			right =	-right;
		if(y <=	0 || y + PCXPic.height >= VESA_YRes)
			down = -down;
	}

    RemoveKeyboard();

	return 0;
}

void WaitForKey(void)			// just test the flagtable until a val chgs
{
    int i, done = 0;

	while(!done)
	    for(i=0;i<128;i++)
    		if(flagtable[i])
            	done = 1;
}

void InstallKeyboard(void)		// DPMI real-mode interrupt; install handler
{
    int i;
	__dpmi_paddr addr;

	for(i=0;i<128;i++)			// begin flagtable by initing to NULL
    	flagtable[i] = 0;

    addr.selector = _my_cs();
    addr.offset32 = (unsigned long)KbdHandler;

    __dpmi_get_protected_mode_interrupt_vector(KB_INT, &OldKbHandler);
    __dpmi_set_protected_mode_interrupt_vector(KB_INT, &addr);
}

void RemoveKeyboard(void)
{
    __dpmi_set_protected_mode_interrupt_vector(KB_INT, &OldKbHandler);

}


void LFB_SetPixel(PICBUF *Buffer, int x, int y,	int	color)
{
	*(Buffer->buffer + y*Buffer->height	+ x) = color;	// linear calc
}


void VESAC_Put(short startx, short starty, PICBUF buffer)
{
	short y;
	char page, breaknum;
	short endx = startx	+ buffer.width + 1;
	long bufindex =	0;

    asm("cli");				// NO INTERRUPTS can occur during this write!!!!

	for(y=starty;y<starty+buffer.height+1;y++)	{
		page = vidpg[y];		// get page of line
		breaknum = vidbr[y];	// get x-coor of page breaks (if any)

		// Line on no break or line all to left of break: write it all at once
		if(breaknum	== 0 ||	(breaknum >	0 && vidx[breaknum]	> endx))  {
			(*VESA_SetPage)(page);		// call the ptr-func asm setpage
			memcpy(vidoff[y] + startx, buffer.buffer+bufindex, endx	- startx);
			bufindex +=	endx - startx;
		}

		// Line all to right of break: write it all at once, page+1
		else if(startx > vidx[breaknum])  {
			(*VESA_SetPage)(page+1); // call the ptr-func asm setpage
			memcpy(vidoff[0] + startx -	vidx[breaknum],	buffer.buffer+bufindex,	endx - startx);
			bufindex +=	endx - startx;
		}

		// Line intersects break: write right half, switch page, write left
		else  {
			(*VESA_SetPage)(page); // call the ptr-func asm setpage
			memcpy(vidoff[y] + startx, buffer.buffer+bufindex, vidx[breaknum] -	startx);
			bufindex +=	vidx[breaknum] - startx;
			(*VESA_SetPage)(page+1); // call the ptr-func asm setpage
			memcpy(vidoff[0], buffer.buffer+bufindex, endx - vidx[breaknum]);
			bufindex +=	endx - vidx[breaknum];
		}
	}

    asm("sti");			// REENABLE INTERRUPTS!
}

void VESAC_HorizLine(short startx, short y,	short endx,	char color)
{
	char page, breaknum;
	page = vidpg[y];			// get page of start of line
	breaknum = vidbr[y];		// get info on any page breaks

	// Line on no break or line all to left of break: write it all at once
	if(breaknum	== 0 ||	(breaknum >	0 && vidx[breaknum]	> endx))  {
		VESA_SetPage(page);
		memset(vidoff[y] + startx, color, endx - startx);
	}

	// Line all to right of break: write it all at once on page+1
	else if(startx > vidx[breaknum])  {
		VESA_SetPage(page+1);
		memset(vidoff[0] + startx -	vidx[breaknum],	color, endx	- startx);
	}

	// Line intersects break: write right part, switch page, write left
	else  {
		VESA_SetPage(page);
		memset(vidoff[y] + startx, color, vidx[breaknum] - startx);
		VESA_SetPage(page+1);
		memset(vidoff[0], color, endx -	vidx[breaknum]);
	}
}

void VESAC_Circle(short	xtrans,	short ytrans, short	radius,	float xstr,	float ystr,	char color)
{
	short x, y;

    // uses the std circle function: NOT ACCURATE!!! WILL LEAVE HOLES!!!!!
	for(x=xtrans-radius;x<=xtrans+radius;x++)  {
		y =	ystr*sqrt(radius*radius	- xstr*(x-xtrans)*(x-xtrans)) +	ytrans;
		if(x >=	2 && x <= 638 && y >= 2	&& y <=	478)
			VESAA_SetPixel(x, y, color);
		y =	-(y	- ytrans) +	ytrans;
		if(x >=	2 && x <= 638 && y >= 2	&& y <=	478)
			VESAA_SetPixel(x, y, color);
	}
}

inline void	SetPix(PICBUF *Buffer, USHORT x, USHORT	y, UCHAR color)
{
	Buffer->buffer[y*Buffer->width+x] =	color;		// linear framebuffer
}

void VESAC_VertLine(short x, short starty, short endy, char	color)
{
	char page, breaknum;
	short y;

	for(y=starty;y<=endy;y++)  {		// write each pixel individually
		page = vidpg[y];
		breaknum = vidbr[y];

		// No break or line passes to the left of the break: just write it
		if(breaknum	== 0 ||	(breaknum >	0 && x < vidx[breaknum]))  {
			VESA_SetPage(page);
			*(vidoff[y]	+ x) = color;
			//memset(vidoff[y] + x, color, 1);		// old way; slower
		}

		// Line passes to the right of the break: incr page then write it
		else  {
			VESA_SetPage(page +	1);
			*(vidoff[0]	+ (x - vidx[breaknum]))	= color;
			//memset(vidoff[0] + (x - vidx[breaknum]), color, 1); // old;slow
		}
	}

}

void VESAC_FillY(short y, char color)	// fills a horiz scan line
{
	char page, breaknum;
	page = vidpg[y];			// get current page of line
	breaknum = vidbr[y];		// get if a break is on the line
	VESA_SetPage(page);

	if(breaknum	== 0)			// if no break, fill the line
		memset(vidoff[y], color, VESA_XRes);
	else  {						// if break, fill right, flip pg, fill left
		memset(vidoff[y], color, vidx[breaknum]);
		VESA_SetPage(page+1);
		memset(vidoff[0], color, VESA_XRes - vidx[breaknum]);
	}
}

void VESAC_ClearScreen(char	color)	// ONLY WORKS FOR 640x480x8!!!!!!!!
{
	char i;
	for(i=0;i<4;i++)		// blast the buffer w/65536 bytes per page
	{
		VESA_SetPage(i);
		memset(VidBuf, color, 65536);
	}

	VESA_SetPage(4);		// ... and 45056 on last (640*480)-(65536*4)
	memset(VidBuf, color, 45056);
}

void VESAC_INTPage(short page)	// the very slow way of flipping pages
{                               // USE THE FUNCTION PTR!!!!
   __dpmi_regs	regs;

	regs.x.ax =	0x4F05;
	regs.x.bx =	0;
	regs.x.dx =	page;
	__dpmi_int(0x10, &regs);
}

short VESA_Init(short mode)			// preinitialize lots of VESA stuff
{
	short y;
	int	runner = 0;
	char breaknum =	1, page	= 0;

	if(!VESA_GetInfo())				// get general card capabilities
		return 0;
	if(!VESA_GetModeInfo(mode))		// get info on particular mode
		return 0;
	if(!VESA_SetMode(mode))			// set the mode
		return 0;

	__djgpp_nearptr_enable();		// TO BYPASS P-MODE SECURITY -- NEEDED!
	VidBuf = VESA_CalcVid();		// calculates pointer to video buffer

    // GLOBALS
	VESA_XRes =	VESAModeInfo.XRes;
	VESA_YRes =	VESAModeInfo.YRes;
	VESA_LFB  =	(char *)RM_TO_LINEAR(VESAModeInfo.PhysBasePtr) + __djgpp_conventional_base;
	VESA_BPP  =	VESAModeInfo.BitsPixl;

	LFB.width =	VESA_XRes;
	LFB.height = VESA_YRes;
	LFB.buffer = VESA_LFB;

	for(y=0;y<VESA_YRes;y++)		// for every scan line ...
	{
		vidmem[y] =	y *	VESA_XRes;	// pointer to video memory from buffer
		vidoff[y] =	(char *)(0xA0000 + runner +	__djgpp_conventional_base); // pointer from all mem
		if(runner +	VESA_XRes >	65536)	// A PAGE BREAK FOUND!
		{
			runner -= 65536;		// start runner @ beg of new page
			vidx[breaknum] = 65536 * (page + 1)	- vidmem[y];	// store x-coor of break
			vidbr[y] = breaknum;	// break number
			breaknum++;
		}
		else
			vidbr[y] = 0;			// otherwise, no break

		if(y !=	0 && vidbr[y-1]	>= 1)	// new page
			page++;
		vidpg[y] = page;				// page #
		runner += VESA_XRes;
	}

	VESA_GetPMInfo();        			// get fast PM function ptrs

	if(VESAInfo.Ver	>= VESA_VER200)		// ONLY WORKS IF VESA >= 2.0
		VESA_SetPage = VESAA_SetPage;
    else
    	VESA_SetPage = VESAC_INTPage;

	return VESAInfo.Ver;
}

char *VESA_CalcVid(void)		// RECALC WHEN MALLOC CALLED!
{
	return (char *)0xA0000 + __djgpp_conventional_base;
}



short VESA_GetInfo(void)		// returns basic VESA info
{
	__dpmi_regs	regs;

	regs.x.ax =	0x4F00;
	regs.x.es =	RM_SEGMENT(__tb);
	regs.x.di =	RM_OFFSET(__tb);
	__dpmi_int(0x10, &regs);
	if(regs.h.ah)
		return 0;
	else  {
		dosmemget(MASK_LINEAR(__tb), sizeof(struct VESAINFO), &VESAInfo);
		return 1;
	}
}

short VESA_GetModeInfo(short mode)	// returns specific mode info
{
	__dpmi_regs	regs;

	regs.x.ax =	0x4F01;
	regs.x.cx =	mode;
	regs.x.es =	RM_SEGMENT(__tb);
	regs.x.di =	RM_OFFSET(__tb);
	__dpmi_int(0x10, &regs);
	if(regs.h.ah)
		return 0;
	else  {
		dosmemget(MASK_LINEAR(__tb), sizeof(struct VESAMODEINFO), &VESAModeInfo);
		return 1;
	}
}

short VESA_GetPMInfo(void)	// returns function ptrs to VESA 2.0+ functions
{
		__dpmi_regs	regs;

	if(VESAInfo.Ver	< 0x200)
		return 0;

	regs.x.ax =	0x4F0A;
	regs.x.bx =	0;
	__dpmi_int(0x10, &regs);

	if(regs.h.ah)
		return 0;
	else  {
		VESAPMInfo = malloc(regs.x.cx);
		dosmemget(regs.x.es*16 + regs.x.di,	regs.x.cx, VESAPMInfo);
		_go32_dpmi_lock_data(VESAPMInfo, regs.x.cx);
		VESA_PMPgFn	  =	(void *)VESAPMInfo + VESAPMInfo->SetWindow;
		VESA_SetPalFn =	(void *)VESAPMInfo + VESAPMInfo->SetPalette;
		return 1;
	}
}


short VESA_SetMode(short mode)		// set the video mode
{                                   // NEED SOMETHING BETTER! A MODE SCANNER?
	__dpmi_regs	regs;

	regs.x.ax =	0x4F02;
	regs.x.bx =	mode;
	__dpmi_int(0x10, &regs);
	if(regs.h.ah)
		return 0;
	else
		return 1;
}






