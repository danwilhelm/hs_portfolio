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

void WaitForKey(void)
{
    int i, done = 0;

	while(!done)
	    for(i=0;i<128;i++)
    		if(flagtable[i])
            	done = 1;
}

void InstallKeyboard(void)
{
    int i;
	__dpmi_paddr addr;

	for(i=0;i<128;i++)
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
	*(Buffer->buffer + y*Buffer->height	+ x) = color;
}


void VESAC_Put(short startx, short starty, PICBUF buffer)
{
	short y;
	char page, breaknum;
	short endx = startx	+ buffer.width + 1;
	long bufindex =	0;

    asm("cli");

	for(y=starty;y<starty+buffer.height+1;y++)	{
		page = vidpg[y];
		breaknum = vidbr[y];

		/* Line on no break or line all to left of break */
		if(breaknum	== 0 ||	(breaknum >	0 && vidx[breaknum]	> endx))  {
			(*VESA_SetPage)(page); //VESA_SetPage(page);
			memcpy(vidoff[y] + startx, buffer.buffer+bufindex, endx	- startx);
			bufindex +=	endx - startx;
		}

		/* Line all to right of break */
		else if(startx > vidx[breaknum])  {
			(*VESA_SetPage)(page+1); //VESA_SetPage(page+1);
			memcpy(vidoff[0] + startx -	vidx[breaknum],	buffer.buffer+bufindex,	endx - startx);
			bufindex +=	endx - startx;
		}

		/* Line intersects break */
		else  {
			(*VESA_SetPage)(page); //VESA_SetPage(page);
			memcpy(vidoff[y] + startx, buffer.buffer+bufindex, vidx[breaknum] -	startx);
			bufindex +=	vidx[breaknum] - startx;
			(*VESA_SetPage)(page+1); //VESA_SetPage(page+1);
			memcpy(vidoff[0], buffer.buffer+bufindex, endx - vidx[breaknum]);
			bufindex +=	endx - vidx[breaknum];
		}
	}

    asm("sti");
}

void VESAC_HorizLine(short startx, short y,	short endx,	char color)
{
	char page, breaknum;
	page = vidpg[y];
	breaknum = vidbr[y];

	/* Line on no break or line all to left of break */
	if(breaknum	== 0 ||	(breaknum >	0 && vidx[breaknum]	> endx))  {
		VESA_SetPage(page);
		memset(vidoff[y] + startx, color, endx - startx);
	}

	/* Line all to right of break */
	else if(startx > vidx[breaknum])  {
		VESA_SetPage(page+1);
		memset(vidoff[0] + startx -	vidx[breaknum],	color, endx	- startx);
	}

	/* Line intersects break */
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
	Buffer->buffer[y*Buffer->width+x] =	color;
}

void VESAC_VertLine(short x, short starty, short endy, char	color)
{
	char page, breaknum;
	short y;

	for(y=starty;y<=endy;y++)  {
		page = vidpg[y];
		breaknum = vidbr[y];

		/* No break or line passes to the left of the break */
		if(breaknum	== 0 ||	(breaknum >	0 && x < vidx[breaknum]))  {
			VESA_SetPage(page);
			*(vidoff[y]	+ x) = color;
			memset(vidoff[y] + x, color, 1);
		}

		/* Line passes to the right of the break */
		else  {
			VESA_SetPage(page +	1);
			*(vidoff[0]	+ (x - vidx[breaknum]))	= color;
			//memset(vidoff[0] + (x - vidx[breaknum]), color, 1);
		}
	}

}

void VESAC_FillY(short y, char color)
{
	char page, breaknum;
	page = vidpg[y];
	breaknum = vidbr[y];
	VESA_SetPage(page);

	if(breaknum	== 0)
		memset(vidoff[y], color, VESA_XRes);
	else  {
		memset(vidoff[y], color, vidx[breaknum]);
		VESA_SetPage(page+1);
		memset(vidoff[0], color, VESA_XRes - vidx[breaknum]);
	}
}

void VESAC_ClearScreen(char	color)
{
	char i;
	for(i=0;i<4;i++)
	{
		VESA_SetPage(i);
		memset(VidBuf, color, 65536);
	}

	VESA_SetPage(4);
	memset(VidBuf, color, 45056);
}

void VESAC_INTPage(short page)
{
   __dpmi_regs	regs;

	regs.x.ax =	0x4F05;
	regs.x.bx =	0;
	regs.x.dx =	page;
	__dpmi_int(0x10, &regs);
}

short VESA_Init(short mode)
{
	short y;
	int	runner = 0;
	char breaknum =	1, page	= 0;

	if(!VESA_GetInfo())
		return 0;
	if(!VESA_GetModeInfo(mode))
		return 0;
	if(!VESA_SetMode(mode))
		return 0;

	__djgpp_nearptr_enable();
	VidBuf = VESA_CalcVid();

	VESA_XRes =	VESAModeInfo.XRes;
	VESA_YRes =	VESAModeInfo.YRes;
	VESA_LFB  =	(char *)RM_TO_LINEAR(VESAModeInfo.PhysBasePtr) + __djgpp_conventional_base;
	VESA_BPP  =	VESAModeInfo.BitsPixl;

	LFB.width =	VESA_XRes;
	LFB.height = VESA_YRes;
	LFB.buffer = VESA_LFB;

	for(y=0;y<VESA_YRes;y++)
	{
		vidmem[y] =	y *	VESA_XRes;
		vidoff[y] =	(char *)(0xA0000 + runner +	__djgpp_conventional_base);
		if(runner +	VESA_XRes >	65536)
		{
			runner -= 65536;
			vidx[breaknum] = 65536 * (page + 1)	- vidmem[y];
			vidbr[y] = breaknum;
			breaknum++;
		}
		else
			vidbr[y] = 0;

		if(y !=	0 && vidbr[y-1]	>= 1)
			page++;
		vidpg[y] = page;
		runner += VESA_XRes;
	}

	VESA_GetPMInfo();

	if(VESAInfo.Ver	>= VESA_VER200)
		VESA_SetPage = VESAA_SetPage;
    else
    	VESA_SetPage = VESAC_INTPage;

	return VESAInfo.Ver;
}

char *VESA_CalcVid(void)
{
	return (char *)0xA0000 + __djgpp_conventional_base;
}



short VESA_GetInfo(void)
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

short VESA_GetModeInfo(short mode)
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

short VESA_GetPMInfo(void)
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


short VESA_SetMode(short mode)
{
	__dpmi_regs	regs;

	regs.x.ax =	0x4F02;
	regs.x.bx =	mode;
	__dpmi_int(0x10, &regs);
	if(regs.h.ah)
		return 0;
	else
		return 1;
}






