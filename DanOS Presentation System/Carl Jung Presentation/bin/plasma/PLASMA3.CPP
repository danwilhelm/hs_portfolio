//#include "graphvga.h"
#include <stdio.h>
#include "text.h"
#include "modex.h"
#include "palette.h"
#include "fps.h"
#include "fixpoint.h"

#define WIDTH 160
#define HEIGHT 120

#define DEPTH 64

//#pragma inline

byte	image[WIDTH*HEIGHT];

byte far V1[64000];
byte far V2[64000];
byte far V3[64000];

word	Scr_Ofs[200];

void plasma();
void checker();

void main()
{
  create_sines();
  Xcreate_Scr_Ofs();
  Tcreate_Scr_Ofs();
  randomize();

  X320x120();
  random_palette(0);

  fps_start();
  plasma();
//  checker();
  fps_end();

  mode_80x25();
  credits("Plasma-X","V1.00","95-10-09");
  show_fps();
}

void Xput160x120()
{
  static int i=0;
  word offset;
  int x,y;

    i++;
    i&=1;
    XSetdst(i);


    Xplane(1,1,0,0);
    offset=XdstPage;
    for(y=0;y<WIDTH*HEIGHT;y+=2)
    {
      VidBuf[offset] = image[y];
      offset++;
    }
    Xplane(0,0,1,1);
    offset=XdstPage;
    for(y=1;y<WIDTH*HEIGHT;y+=2)
    {
      VidBuf[offset] = image[y];
      offset++;
    }
/*
    x2=0;
    for(x=0;x<WIDTH;x++)
    {
      offset=(x2>>2) + XdstPage;

      Xplane(1,1,0,0);

      for(y=0;y<HEIGHT;y++)
      {
	VidBuf[offset] = (xang1[x] + xang2[x] + yang1[y] + yang2[y]);

	offset += 80;
      }
      x2+=2;


      offset-=9600;
      x++;

      Xplane(0,0,1,1);

      for(y=0;y<HEIGHT;y++)
      {
	VidBuf[offset] = (xang1[x] + xang2[x] + yang1[y] + yang2[y]);

	offset += 80;
      }
      x2+=2;
    }
*/
    XSetvis(i);
//    rotatepal(0,255);
}

void calculate_image(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
  int x,y;
  word t1,t2,t3,t4;
  word offset=0;
  static byte roll=0;

  for(y=0;y<HEIGHT;y++)
  {
    // calculate this only once per line...
    t1=Scr_Ofs[y1+y]+x1;
    t2=Scr_Ofs[y2+y]+x2;
    t3=Scr_Ofs[y3+y]+x3;
    t4=Scr_Ofs[y4+y]+x4;
    for(x=0;x<WIDTH;x++)
    {
      // adding "reg, mem" takes 2 ticks, while "inc mem" takes 3,
      //  so the four [t?+x] additions are faster than simply incrementing
      //  t?

      // it may help a bit to unroll the loop...

      image[offset]=roll+
		   V1[t1+x]
		   +V1[t2+x]
		   -V3[t3+x]
		   -V2[t4+x]
		   ;
      offset++;
    }
  }

/*
  for(y=0;y<HEIGHT;y++)
    for(x=0;x<WIDTH;x++)
    {
      image[offset]=//roll+
		   V1[Scr_Ofs[y1+y]+x1+x]
		   +V1[Scr_Ofs[y2+y]+x2+x]
		   -V3[Scr_Ofs[y3+y]+x3+x]
		   -V2[Scr_Ofs[y4+y]+x4+x]
		   ;
      offset++;
    }
*/
/*
  for(y=0;y<HEIGHT;y++)
  {
    asm	push	ds

    asm	mov	dx, ds

    asm	mov	ax, [VGA]
    asm	mov	es, ax
    asm	mov	di, [XdstPage]
    asm mov	cx, WIDTH/2
    asm	mov	ax, seg [V1]
    asm	mov	ds, ax
    asm	xor	si, si
    _SI=XScr_Ofs[y1] + XScr_Ofs[y] + x1;
//   V1loop:
    asm	rep	movsw

    asm	mov	ds, dx
    asm	mov	cx, WIDTH/2
    asm	mov	ax, seg [V1]
    _SI=XScr_Ofs[y2] + XScr_Ofs[y] + x2;
    asm	mov	ds, ax
   V1loop:
    asm lodsw
    asm add	es:[di], ax
    asm inc	di
    asm	inc	di
    asm	loop	V1loop

    asm	mov	ds, dx
    asm	mov	cx, WIDTH/2
    asm	mov	ax, seg [V2]
    _SI=XScr_Ofs[y3] + XScr_Ofs[y] + x3;
    asm	mov	ds, ax
   V2loop:
    asm lodsw
    asm sub	es:[di], ax
    asm inc	di
    asm	inc	di
    asm	loop	V2loop

    asm	mov	ds, dx
    asm	mov	cx, WIDTH/2
    asm	mov	ax, seg [V3]
    _SI=XScr_Ofs[y4] + XScr_Ofs[y] + x4;
    asm	mov	ds, ax
   V3loop:
    asm lodsw
    asm sub	es:[di], ax
    asm inc	di
    asm	inc	di
    asm	loop	V3loop

    asm	pop	ds
  }
*/
  roll-=10;
}

void calculate_image2(int x1, int y1, int x2, int y2)
{
  int x,y;
  word t1, t2;
  word offset=0;
  static byte roll=0;

  for(y=0;y<HEIGHT;y++)
  {
    // calculate this only once per line...
    t1=Scr_Ofs[y1+y]+x1;
    t2=Scr_Ofs[y2+y]+x2;
    for(x=0;x<WIDTH;x++)
    {
      // adding "reg, mem" takes 2 ticks, while "inc mem" takes 3,
      //  so the four [t?+x] additions are faster than simply incrementing
      //  t?

      // it may help a bit to unroll the loop...

      image[offset]=roll+
		   V1[t1+x]
		   +V1[t2+x]
		   ;
      offset++;
    }
  }

  roll-=31;
}

void init_Vs()
{
  int x,y;
  word offset=0;

  FILE *fp;
  FILE *fp2;
  FILE *fp3;

  fp =fopen("v1.raw","rb");
  fp2=fopen("v2.raw","rb");
  fp3=fopen("v3.raw","rb");

  fread(&V1[0],64000,1,fp);
  fread(&V2[0],64000,1,fp2);
  fread(&V3[0],64000,1,fp3);

//  fclose(fp );
//  fclose(fp2);
//  fclose(fp3);
/*
  for(y=0;y<200;y++)
    for(x=0;x<320;x++)
    {
//      V1[offset]= sqrt((160-x)*(160-x) + (100-y)*(100-y)) *5;
      V2[offset]= (sin(V2[offset]/9.5)+1)*45;
//	V3[offset]= ((Cos(x))*(Sin(y))*32)*((Sin(x))*(Cos(y))*32);
//      V1[offset]=
//      V2[offset]= ((Sin(x*2)+1)*64)+((Sin(y*2)+1)*64);
      offset++;
    }
  fp=fopen("new.raw","wb");
  fwrite(&V2[0],64000,1,fp);

  fclose(fp);
*/
/*
  for(offset=0;offset<64000;offset++)
  {
    V1[offset]=offset&63;
    V2[offset]=offset;
  }
*/
  for(x=0;x<200;x++)
    Scr_Ofs[x]=x*320;
}

void plasma()
{
//  init_ang_arrays();
  init_Vs();

  double a1, b1, a2, b2;
  double a3, b3, a4, b4;
  int x1, y1, x2, y2;
  int x3, y3, x4, y4;

  char key=32;

  a1=rand()%360;
  b1=rand()%360;
  a2=rand()%360;
  b2=rand()%360;
  a3=rand()%360;
  b3=rand()%360;
  a4=rand()%360;
  b4=rand()%360;

  while(key!=0)
  {
    a1+=1.0;
    b1+=2.3;
    a2+=2.7;
    b2+=3.7;
    a3+=2.2;
    b3+=3.9;
    a4+=2.4;
    b4+=3.3;

    x1=((Cos(a1)+1)+(Sin(b3)+1))*40;
    y1=((Cos(b1)+1)+(Cos(a2)+1))*20;
    x2=(Cos(a2)+1)*80;
    y2=(Cos(b2)+1)*40;
    x3=(Cos(a3)+1)*80;
    y3=(Cos(b3)+1)*40;
    x4=(Cos(a4)+1)*80;
    y4=(Cos(b4)+1)*40;
//    shift_ang_arrays(4, 1, 3, 0);
    calculate_image(x1, y1, x2, y2, x3, y3, x4, y4);
    Xput160x120();
    frames++;

    if(kbhit())
    {
      key=getch();
      if(key==' ') random_palette(0);
      else key=0;
    }
  }
}

void checker()
{
  double a1, b1, a2, b2;
  int x1, y1, x2, y2;

  char key=32;

  for(x1=0;x1<200;x1++)
    Scr_Ofs[x1]=x1*320;

  FILE *fp;
  fp=fopen("checker.raw","rb");
  if(!fp) return;
  fread(&V1[0],64000,1,fp);
  fclose(fp);

  a1=rand()%360;
  b1=rand()%360;
  a2=rand()%360;
  b2=rand()%360;

  while(key!=0)
  {
    a1+=1.0;
    b1+=1.3;
    a2+=0.7;
    b2+=1.7;

    x1=(Cos(a1)+1)*80;
    y1=(Cos(b1)+1)*40;
    x2=(Cos(a2)+1)*80;
    y2=(Cos(b2)+1)*40;

    calculate_image2(x1, y1, x2, y2);
    Xput160x120();
    frames++;

    if(kbhit())
    {
      key=getch();
      if(key==' ') random_palette(0);
      else key=0;
    }
  }
}
