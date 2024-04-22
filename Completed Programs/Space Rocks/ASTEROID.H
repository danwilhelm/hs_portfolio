/*****************************************************
					ASTEROID.H
				  by Danny Wilhelm
				  	 03/09/99
*****************************************************/


#include <stdlib.h>
#include "dkwlib.cpp"

/********************************************
	 			STRUCTURES
*********************************************/

struct STAR
{
	int X;
	int Y;
	int Speed;
};                   

struct ASTEROID
{
	int X;
	int Y;
	int Speed;
	int ExplFrame;
	int FrameCounter;
	int Type;
}; 

/********************************************
	 		GLOBAL CONSTANTS
*********************************************/

#define FALSE			0				// Generic
#define TRUE			1 

#define ROMCHARW		8				// ROM Character dimensions
#define ROMCHARH		8
#define ROMSMALLW		5
#define ROMSMALLH		6

#define NUM_STARS		60				// Game Specific 
#define MAX_ASTEROIDS	20
#define EXPLFRAMES		5
#define ASTTYPES		5
#define LASERW			19 
#define LASERH			10
#define SHIPW			40
#define SHIPH			50
#define ROCKW			26
#define ROCKH			22
#define WEAPW			22 
#define WEAPH			10
#define WEAPON_SPEED    5 
#define MAX_SHIELDS		72    

#define CGCTLOGO		0
#define SPMENU  		1
#define CREDITS        	2
#define STILLBCK        3
#define STILLP1         4
#define STILLP2         5
#define STILLP3         6
#define STILLP4         7
#define STILLP5         8
#define STILLP6         9
#define STILLP7         10
#define STILLP8         11
#define LAS0            12
#define LAS1            13
#define LAS2            14
#define LAS3            15
#define LAS4            16
#define LAS5            17
#define LAS6            18
#define LAS7            19
#define SPWSHLD0        20
#define SPWSHLD1        21
#define ROCK1        	22
#define ROCK2           23
#define ROCK3           24
#define ROCK4           25
#define ROCK5           26
#define EXPL0           27
#define EXPL1           28
#define EXPL2           29
#define EXPL3           30
#define EXPL4           31 
#define HSCORETBL		32
#define ENTERNAME		33   

#define TIME_MASK		1000000000

/********************************************
	 			SUBROUTINES
*********************************************/

extern void Change_Time(unsigned int new_time); 
extern void DispBigChar(unsigned char Char, int X, int Y, int Color, int BackColor);
extern void DispBigText(char *String, int X, int Y, int Color, int BackColor);
extern void DispSmallChar(unsigned char Char, int X, int Y, int Color);
extern void DispSmallText(char String[], int X, int Y, int Color);
extern void Fire_Weapon(void);
extern void MoveStars(void);
extern void	Initialize(void);
extern void MoveAsteroids(void);
extern void	MoveLaser(void);
extern void Move_Player(void);
extern void Rand_Asteroid(ASTEROID &Ast, int AstNum);
extern void UpdateScore(long Change);

/********************************************
	 			GLOBAL VARIABLES
*********************************************/

ASTEROID	Asteroids[MAX_ASTEROIDS];
STAR 		Stars[NUM_STARS]; 
BUFPIC 		Asteroid[ASTTYPES], Ship[2], AstExpl[EXPLFRAMES], TradeBack;
BUFPIC		Laser, HScoreTable;
BUFPIC		StillPlanet;

int NumAsteroids = 5; 
int PlayerX   	 = 160 - (SHIPW >> 1);
int PlayerY   	 = 200 - SHIPH;
int WeaponX   	 = 0;
int WeaponY   	 = 0;
int WeaponOn  	 = FALSE;      
int PointsX	  	 = 0;
int PointsY   	 = 0;
char Points[] 	 = "";
int PointsLen	 = 3;
int PointsColor  = 20;
int PointsOn  	 = 0;
int Time	  	 = 0;
long Score 		 = TIME_MASK;
static char *StrScore	= (char *)malloc(8);
int MinX	 	 = 0;
int MaxX      	 = 320 - SHIPW;
int MinY		 = SCREEN_HEIGHT >> 1;
int MaxY		 = SCREEN_HEIGHT - SHIPH;
char *LevelString = "Level 1\0";
long TimeLeft	 = TIME_MASK;
static char *StrTimeLeft	= (char *)malloc(10);
char RLEDATName[] = "SPACERCK.DAT";



