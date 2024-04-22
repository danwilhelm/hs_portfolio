/****************************************************************************
	Paraliens.h -- Authored by Daniel Wilhelm for the
	Computer Game Creation Team at Columbus North High School

	Last revision date: 12/04/00

	Notes to self: 1) Eliminate the STL!
****************************************************************************/

// XMedia standard includes
#include "XMedia.h"
#include "XMediaWin.h"

// from C++ STL
#include <list>			// able to use doubly linked lists
using namespace std;	// allows us to use STL! (note to self: get rid of these!)

#include "CAnimation.cpp"

typedef list<CAnimation> ANIMLIST;

/////////////////////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS
/////////////////////////////////////////////////////////////////////////////

// main game routines
void RenderWorld(void);			// renders all game objects
void RenderMenu(void);			// renders main menu
int GameInput(void);			// gets input from the user
void UpdateScore(int points);	// updates the score
void LoadWeapon(int weapon);	// initializes a weapon

// logic routines for collision detection and AI
int AlienLogic(CAnimation *aniptr);
int WeaponLogic(CAnimation *aniptr);
int ShipLogic(CAnimation *aniptr);
int CrossHairLogic(CAnimation *aniptr);
int ParachuteLogic(CAnimation *aniptr);


/////////////////////////////////////////////////////////////////////////////
// STRUCTURE DECLARATIONS
/////////////////////////////////////////////////////////////////////////////

// structure for storing music files
struct MUSIC_LIST
{
	CMusic	*musicptr;			// pointer to music storage
	char	filename[50];		// filename of music
};

// structure for storing sound files
struct SOUND_LIST
{
	CSound   *soundptr;			// pointer to sound storage
	char	 filename[50];		// filename of sound
};

// structure for storing graphics files
struct GRAPHICS_LIST
{
	CSurface *surfaceptr;		// pointer to surface to load into
	char	 filename[50];		// filename of image
};

// structure for preinitializing animation classes
struct ANIMATION_LIST
{
	CAnimation *animationptr;	// pointer to the animation that desires change
	
	CSurface *surfaceptr;		// pointer to surface containing animation
	CAnimation *attanimptr;		// ptr to surface containing attached animation
	int		 maxframes;			// max frames for animation
	
	int (*Logic)(CAnimation *aniptr);	// pointer to logic functioon
};


/////////////////////////////////////////////////////////////////////////////
// CONSTANT VALUES
/////////////////////////////////////////////////////////////////////////////

// a handy number to have around :) 
const float PI = 3.1415926;

// set the scale of ints so as not to use floats
const int WORLDSCALE	   = 512;
const int SHIFTSCALE	   = 9;		// 2^9 = 512

// max number of frames for each entity
const int ALIEN_FRAMES	   = 1;
const int WEAPON_FRAMES	   = 16;
const int CROSSHAIR_FRAMES = 1;
const int SHIP_FRAMES	   = 1;
const int MAX_FRAMES	   = 16;

// command to remove an object from a list
const int LIST_REMOVE	   = 1;

// alien states
const int ALIEN_DROPPING   = 0;
const int ALIEN_ONGROUND   = 1;
const int ALIEN_MAXY	   = 430;

// general screen defines
const int XRES			 = 640;
const int YRES			 = 480;
const int BPP			 = 16;
const int BACKBUFFERS	 = 1;

// positions of the turrets, angle settings for crosshair
const int TURRETX		 = (XRES / 2) - 25;
const int TURRETY		 = (YRES - 50);
const int CROSSRADIUS	 = 75;
const float CROSSANGINC	 = PI/90;  // two degrees
const float MAX_ANGLE	 = PI;
const float MIN_ANGLE	 = 0;
const int WEAPONSPEED	 = 8;
const int WEAPONX		 = 50;
const int WEAPONY		 = 50;
const int SHIPX			 = 150;
const int SHIPY			 = 100;
const int ALIENX		 = 50;
const int ALIENY		 = 50;

// maximum # of objects for static arrays
const int MAX_ALIENS	 = 11;
const int MAX_MUSIC		 = 2;
const int MAX_BACKS		 = 2;
const int MAX_WEAPONS	 = 7;
const int MAX_SHIPS		 = 4;
const int MAX_CROSSHAIRS = 1;
const int MAX_MENUBTNS	 = 5;
const int MAX_LEVELS	 = MAX_WEAPONS;

// title of window
const char WIN_TITLE[]	 = "Paraliens by the CNHS Computer Game Creation Team";

// game master states for state machine
const int STATE_PLAY		= 1;
const int STATE_NEWGAME		= 2;
const int STATE_EXIT		= 3;
const int STATE_MAINMENU	= 4;
const int STATE_OPTIONS		= 5;
const int STATE_HIGHSCORES	= 6;
const int STATE_CREDITS		= 7;
const int STATE_INTRO		= 8;

const int WEAPON_FIRE		= 0;
const int WEAPON_IMPACT		= 1;
const int WEAPON_LOAD		= 2;

const int LEVELSCORE[]	= {1000, 2000, 3000, 4000, 5000, 6000, 7000};
const int WEAPONCOST[]  = {-10, -20, -30, -40, -50, -60, -70};

const int SCORE_ALIEN		= 500;
const int SCORE_PARACHUTE	= 500;
const int SCORE_SHIP		= 1000;
const int SCORE_WEAPON		= -500;
const int STARTSCORE		= 1500;

const int SCOREX			= 300;
const int SCOREY			= 300;


/////////////////////////////////////////////////////////////////////////////
// DYNAMIC VALUES
/////////////////////////////////////////////////////////////////////////////

// current angle of crosshair
float crossangle  = PI/2;	// straight up (90 deg)
int   crossx	  = TURRETX;
int   crossy	  = TURRETY - (CROSSRADIUS << 1);

int gamestate     = STATE_INTRO;	// current state of state machine
int backgroundnum = 0;				// the current background
int crosshairnum  = 0;				// the current crosshair
int currweapon	  = 0;				// the current weapon
int currmusic	  = 0;

int shipprob	   = 200;
int dropalienprob  = 100;
int maxalienspeed  = 3;
int alienfallspeed = 6;

int keydelay    = 0;
int maxkeydelay = 10;
int shipcounter = 0;
int maxshipcounter = 100;

int score = 0;
int level = 0;
int groundaliens = 0;

char scorestr[10] = "0";
COLORREF scorecolor;

ANIMLIST renderlist;			// a list of all visible objects


/////////////////////////////////////////////////////////////////////////////
// XMEDIA VARIABLES FOR MULTIMEDIA
/////////////////////////////////////////////////////////////////////////////

// XMedia defines, music, and static images
CXMedia		xmedia;
CMusic		gamemusic[MAX_MUSIC], creditsmusic, intromusic;
CSurface	screen, primary;
CSurface	background[MAX_BACKS];
CSurface	mainmenubtn[MAX_MENUBTNS];
CSurface	mainmenu, credits;
CSurface	cgctlogo;

// images used for animations
CSurface alien[MAX_ALIENS][ALIEN_FRAMES];
CSurface weapon[MAX_WEAPONS][WEAPON_FRAMES];
CSurface ship[MAX_SHIPS][SHIP_FRAMES];
CSurface crosshair[MAX_CROSSHAIRS][CROSSHAIR_FRAMES];
CSurface parachute[1][1];

// the animations
CAnimation alienani[MAX_ALIENS];
CAnimation weaponani[MAX_WEAPONS];
CAnimation shipani[MAX_SHIPS];
CAnimation crosshairani[MAX_CROSSHAIRS];
CAnimation paraani[1];

// the sounds
CSound	weaponsnd[MAX_WEAPONS][10];
CSound	gameoversnd[10];
CSound	rantsnd[10];

/////////////////////////////////////////////////////////////////////////////
// LISTS OF INPUT FILES AND CLASS INITS
/////////////////////////////////////////////////////////////////////////////

// list of music files
MUSIC_LIST 
musiclist[]    = {{&gamemusic[0]	 , "./Music/BGM0.mid"},
				  {&gamemusic[1]	 , "./Music/BGM1.mid"}};

// list of sound files
SOUND_LIST
 soundlist[]	= {{&weaponsnd[0][0] , "./Sound/spfire0.wav"},
				   {&weaponsnd[0][1] , "./Sound/spimpact0.wav"},
				   {&weaponsnd[0][2] , "./Sound/spload0.wav"},
				   {&weaponsnd[1][0] , "./Sound/icfire0.wav"},
				   {&weaponsnd[1][1] , "./Sound/icimpact0.wav"},
				   {&weaponsnd[1][2] , "./Sound/icload0.wav"},
				   {&weaponsnd[1][3] , "./Sound/icload1.wav"},
				   {&weaponsnd[1][4] , "./Sound/icload2.wav"},
				   {&weaponsnd[1][5] , "./Sound/icload3.wav"},
				   {&weaponsnd[1][6] , "./Sound/icload4.wav"},
				   {&weaponsnd[2][0] , "./Sound/sdcfire0.wav"},
				   {&weaponsnd[2][1] , "./Sound/sdcimpact0.wav"},
				   {&weaponsnd[2][2] , "./Sound/sdcload0.wav"},
				   {&weaponsnd[3][0] , "./Sound/sfcfire0.wav"},
				   {&weaponsnd[3][1] , "./Sound/sfcimpact0.wav"},
				   {&weaponsnd[3][2] , "./Sound/sfcload0.wav"},
				   {&weaponsnd[4][0] , "./Sound/vwkfire0.wav"},
				   {&weaponsnd[4][1] , "./Sound/vwkimpact0.wav"},
				   {&weaponsnd[4][2] , "./Sound/vwkload0.wav"},
				   {&weaponsnd[5][0] , "./Sound/wefire0.wav"},
				   {&weaponsnd[5][1] , "./Sound/weimpact0.wav"},
				   {&weaponsnd[5][2] , "./Sound/weload0.wav"},
				   {&weaponsnd[5][3] , "./Sound/weload1.wav"},
				   {&weaponsnd[5][4] , "./Sound/weload2.wav"},
				   {&weaponsnd[6][0] , "./Sound/kpfire0.wav"},
				   {&weaponsnd[6][1] , "./Sound/kpimpact0.wav"},
				   {&weaponsnd[6][2] , "./Sound/kpload0.wav"},
				   {&gameoversnd[0]  , "./Sound/go0.wav"},
				   {&gameoversnd[1]  , "./Sound/go1.wav"},
				   {&gameoversnd[2]  , "./Sound/go2.wav"},
				   {&gameoversnd[3]  , "./Sound/go3.wav"},
				   {&gameoversnd[4]  , "./Sound/go4.wav"}};

// list of graphic files
GRAPHICS_LIST
 graphicslist[] = {{&alien[0][0]	 , "./Graphics/A-claw.bmp"},
				   {&alien[1][0]	 , "./Graphics/A-frog.bmp"},
				   {&alien[2][0]	 , "./Graphics/A-head.bmp"},
				   {&alien[3][0]	 , "./Graphics/A-hop.bmp"},
				   {&alien[4][0]	 , "./Graphics/A-werid.bmp"},
				   {&alien[5][0]	 , "./Graphics/A-blob.bmp"},
				   {&alien[6][0]	 , "./Graphics/A-thumb.bmp"},
   				   {&alien[7][0]	 , "./Graphics/A-hand.bmp"},
				   {&alien[8][0]	 , "./Graphics/A-fish.bmp"},
				   {&alien[9][0]	 , "./Graphics/A-eyewing.bmp"},
				   {&alien[10][0]	 , "./Graphics/A-ball.bmp"},
				   {&parachute[0][0] , "./Graphics/parachute.bmp"},
				   {&weapon[0][0]	 , "./Graphics/Sp1.bmp"},
   				   {&weapon[0][1]	 , "./Graphics/Sp2.bmp"},
				   {&weapon[0][2]	 , "./Graphics/Sp3.bmp"},
   				   {&weapon[0][3]	 , "./Graphics/Sp4.bmp"},
				   {&weapon[0][4]	 , "./Graphics/Sp5.bmp"},
   				   {&weapon[0][5]	 , "./Graphics/Sp6.bmp"},
				   {&weapon[0][6]	 , "./Graphics/Sp7.bmp"},
   				   {&weapon[0][7]	 , "./Graphics/Sp8.bmp"},
				   {&weapon[0][8]	 , "./Graphics/Sp9.bmp"},
   				   {&weapon[0][9]	 , "./Graphics/Sp10.bmp"},
				   {&weapon[0][10]	 , "./Graphics/Sp11.bmp"},
   				   {&weapon[0][11]	 , "./Graphics/Sp12.bmp"},
				   {&weapon[0][12]	 , "./Graphics/Sp13.bmp"},
   				   {&weapon[0][13]	 , "./Graphics/Sp14.bmp"},
				   {&weapon[0][14]	 , "./Graphics/Sp15.bmp"},
   				   {&weapon[0][15]	 , "./Graphics/Sp16.bmp"},
				   {&weapon[1][0]	 , "./Graphics/IMac.bmp"},
				   {&weapon[2][0]	 , "./Graphics/selfdoubt.bmp"},
				   {&weapon[3][0]	 , "./Graphics/skoolfud.bmp"},
				   {&weapon[4][0]	 , "./Graphics/beetleg.bmp"},
				   {&weapon[5][0]	 , "./Graphics/weapone.bmp"},
				   {&weapon[6][0]    , "./Graphics/AssultPenguin.bmp"},
				   {&ship[0][0]		 , "./Graphics/spaceship2.bmp"},
				   {&ship[1][0]		 , "./Graphics/BlueSS.bmp"},
				   {&ship[2][0]		 , "./Graphics/GreenSS.bmp"},
				   {&ship[3][0]		 , "./Graphics/OrangeSS.bmp"},
				   {&crosshair[0][0] , "./Graphics/crosshair.bmp"},
				   {&background[0]	 , "./Graphics/BackGround.bmp"},
				   {&background[1]	 , "./Graphics/Desertbackground.bmp"},
				   {&mainmenu		 , "./Graphics/titlemain.bmp"},
				   {&mainmenubtn[0]	 , "./Graphics/button_newg.bmp"},
				   {&mainmenubtn[1]	 , "./Graphics/button_optn.bmp"},
				   {&mainmenubtn[2]	 , "./Graphics/button_inst.bmp"},
				   {&mainmenubtn[3]	 , "./Graphics/button_exit.bmp"},
				   {&cgctlogo		 , "./Graphics/cgctlogo.bmp"}};


// list of animation intializers
ANIMATION_LIST
 animationlist[] = {{&alienani[0], &alien[0][0], NULL, 1, AlienLogic},
				    {&alienani[1], &alien[1][0], NULL, 1, AlienLogic},	
					{&alienani[2], &alien[2][0], NULL, 1, AlienLogic},
					{&alienani[3], &alien[3][0], NULL, 1, AlienLogic},
					{&alienani[4], &alien[4][0], NULL, 1, AlienLogic},
					{&alienani[5], &alien[5][0], NULL, 1, AlienLogic},
					{&alienani[6], &alien[6][0], NULL, 1, AlienLogic},
					{&alienani[7], &alien[7][0], NULL, 1, AlienLogic},
					{&alienani[8], &alien[8][0], NULL, 1, AlienLogic},
					{&alienani[9], &alien[9][0], NULL, 1, AlienLogic},
					{&alienani[10], &alien[10][0], NULL, 1, AlienLogic},
					{&paraani[0] , &parachute[0][0], NULL, 1, ParachuteLogic},
					{&weaponani[0], &weapon[0][0], NULL, 16, WeaponLogic},
					{&weaponani[1], &weapon[1][0], NULL, 1, WeaponLogic},
					{&weaponani[2], &weapon[2][0], NULL, 1, WeaponLogic},
					{&weaponani[3], &weapon[3][0], NULL, 1, WeaponLogic},
					{&weaponani[4], &weapon[4][0], NULL, 1, WeaponLogic},
					{&weaponani[5], &weapon[5][0], NULL, 1, WeaponLogic},
					{&weaponani[6], &weapon[6][0], NULL, 1, WeaponLogic},
					{&shipani[0], &ship[0][0], NULL, 1, ShipLogic},
					{&shipani[1], &ship[1][0], NULL, 1, ShipLogic},
					{&shipani[2], &ship[2][0], NULL, 1, ShipLogic},
					{&shipani[3], &ship[3][0], NULL, 1, ShipLogic},
					{&crosshairani[0], &crosshair[0][0], NULL, 1, CrossHairLogic}};

// list of clipping regions
RECT 
 cliplist[] =      {{0, 0, 640, 480}};


/////////////////////////////////////////////////////////////////////////////
// LENGTH OF FILE LISTS (NUM OF ENTITIES)
/////////////////////////////////////////////////////////////////////////////

// static lengths of all of the lists above (how many entities per list)
const int animationlistlen = sizeof(animationlist) / sizeof(ANIMATION_LIST);
const int musiclistlen	   = sizeof(musiclist) / sizeof(MUSIC_LIST);
const int soundlistlen	   = sizeof(soundlist) / sizeof(SOUND_LIST);
const int graphicslistlen  = sizeof(graphicslist) / sizeof(GRAPHICS_LIST);
const int cliplistlen	   = sizeof(cliplist) / sizeof(RECT);
